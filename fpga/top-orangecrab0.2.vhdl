library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library work;
use work.wishbone_types.all;

entity toplevel is
    generic (
        MEMORY_SIZE        : integer  := 16384;
        RAM_INIT_FILE      : string   := "firmware.hex";
        RESET_LOW          : boolean  := true;
        CLK_INPUT          : positive := 100000000;
        CLK_FREQUENCY      : positive := 100000000;
        HAS_FPU            : boolean  := true;
        HAS_BTC            : boolean  := false;
        USE_LITEDRAM       : boolean  := true;
        NO_BRAM            : boolean  := true;
        SCLK_STARTUPE2     : boolean := false;
        SPI_FLASH_OFFSET   : integer := 4194304;
        SPI_FLASH_DEF_CKDV : natural := 1;
        SPI_FLASH_DEF_QUAD : boolean := true;
        LOG_LENGTH         : natural := 0;
        UART_IS_16550      : boolean  := true;
        HAS_UART1          : boolean  := false;
        USE_LITESDCARD     : boolean := true;
        USE_LITESCOPE      : boolean := true;
        ICACHE_NUM_LINES   : natural := 32;
        NGPIO              : natural := 0
        );
    port(
        ext_clk   : in  std_ulogic;
        ext_rst_n : in  std_ulogic;

        -- UART0 signals:
        pin_gpio_0 : out std_ulogic;
        pin_gpio_1 : in  std_ulogic;

        -- liteuart wishbone bridge
        pin_gpio_5 : out std_ulogic;
        pin_gpio_6 : in  std_ulogic;

        -- LEDs
        led0_b  : out std_ulogic;
        led0_g  : out std_ulogic;
        led0_r  : out std_ulogic;

        -- SPI
        spi_flash_cs_n   : out std_ulogic;
        spi_flash_mosi   : inout std_ulogic;
        spi_flash_miso   : inout std_ulogic;
        spi_flash_wp_n   : inout std_ulogic;
        spi_flash_hold_n : inout std_ulogic;

        -- SD card wires
        sdcard_data   : inout std_ulogic_vector(3 downto 0);
        sdcard_cmd    : inout std_ulogic;
        sdcard_clk    : out   std_ulogic;
        sdcard_cd     : in    std_ulogic;

        -- DRAM wires
        ddram_a       : out std_ulogic_vector(13 downto 0);
        ddram_ba      : out std_ulogic_vector(2 downto 0);
        ddram_ras_n   : out std_ulogic;
        ddram_cas_n   : out std_ulogic;
        ddram_we_n    : out std_ulogic;
        ddram_cs_n    : out std_ulogic;
        ddram_dm      : out std_ulogic_vector(1 downto 0);
        ddram_dq      : inout std_ulogic_vector(15 downto 0);
        ddram_dqs_p   : inout std_ulogic_vector(1 downto 0);
        ddram_clk_p   : out std_ulogic;
        -- only the positive differential pin is instantiated
        --ddram_dqs_n   : inout std_ulogic_vector(1 downto 0);
        --ddram_clk_n   : out std_ulogic;
        ddram_cke     : out std_ulogic;
        ddram_odt     : out std_ulogic;
        ddram_reset_n : out std_ulogic;

        ddram_gnd      : out std_ulogic_vector(1 downto 0);
        ddram_vccio    : out std_ulogic_vector(5 downto 0)
        );
end entity toplevel;

architecture behaviour of toplevel is

    -- Reset signals:
    signal soc_rst : std_ulogic;
    signal pll_rst : std_ulogic;

    -- Internal clock signals:
    signal system_clk        : std_ulogic;
    signal system_clk_locked : std_ulogic;

    -- External IOs from the SoC
    signal wb_ext_io_in        : wb_io_master_out;
    signal wb_ext_io_out       : wb_io_slave_out;
    signal wb_ext_is_dram_csr  : std_ulogic;
    signal wb_ext_is_dram_init : std_ulogic;
    signal wb_ext_is_sdcard    : std_ulogic;

    -- DRAM main data wishbone connection
    signal wb_dram_in          : wishbone_master_out;
    signal wb_dram_out         : wishbone_slave_out;

    -- DRAM control wishbone connection
    signal wb_dram_ctrl_out    : wb_io_slave_out := wb_io_slave_out_init;

    -- LiteSDCard connection
    signal ext_irq_sdcard      : std_ulogic := '0';
    signal wb_sdcard_out       : wb_io_slave_out := wb_io_slave_out_init;
    signal wb_sddma_out        : wb_io_master_out := wb_io_master_out_init;
    signal wb_sddma_in         : wb_io_slave_out;
    signal wb_sddma_nr         : wb_io_master_out;
    signal wb_sddma_ir         : wb_io_slave_out;
    -- for conversion from non-pipelined wishbone to pipelined
    signal wb_sddma_stb_sent   : std_ulogic;

    signal sdscope_data : std_ulogic_vector(3 downto 0);
    signal sdscope_data_oe : std_ulogic;
    signal sdscope_cmd  : std_ulogic;
    signal sdscope_cmd_oe  : std_ulogic;
    signal sdscope_clk   : std_ulogic;
    signal sdscope_extra   : std_ulogic_vector(7 downto 0);

    -- Control/status
    signal core_alt_reset : std_ulogic;

    -- Status LED
    signal led0_b_pwm : std_ulogic;
    signal led0_r_pwm : std_ulogic;
    signal led0_g_pwm : std_ulogic;

    -- Dumb PWM for the LEDs, those RGB LEDs are too bright otherwise
    signal pwm_counter  : std_ulogic_vector(8 downto 0);

    -- SPI flash
    signal spi_sck     : std_ulogic;
    signal spi_cs_n    : std_ulogic;
    signal spi_sdat_o  : std_ulogic_vector(3 downto 0);
    signal spi_sdat_oe : std_ulogic_vector(3 downto 0);
    signal spi_sdat_i  : std_ulogic_vector(3 downto 0);

    -- GPIO
    signal gpio_in     : std_ulogic_vector(NGPIO - 1 downto 0);
    signal gpio_out    : std_ulogic_vector(NGPIO - 1 downto 0);
    signal gpio_dir    : std_ulogic_vector(NGPIO - 1 downto 0);

    -- Fixup various memory sizes based on generics
    function get_bram_size return natural is
    begin
        if USE_LITEDRAM and NO_BRAM then
            return 0;
        else
            return MEMORY_SIZE;
        end if;
    end function;

    function get_payload_size return natural is
    begin
        if USE_LITEDRAM and NO_BRAM then
            return MEMORY_SIZE;
        else
            return 0;
        end if;
    end function;

    constant BRAM_SIZE    : natural := get_bram_size;
    constant PAYLOAD_SIZE : natural := get_payload_size;

    COMPONENT USRMCLK
        PORT(
            USRMCLKI : IN STD_ULOGIC;
            USRMCLKTS : IN STD_ULOGIC
        );
    END COMPONENT;
    attribute syn_noprune: boolean ;
    attribute syn_noprune of USRMCLK: component is true;

begin

    -- Main SoC
    soc0: entity work.soc
        generic map(
            MEMORY_SIZE        => BRAM_SIZE,
            RAM_INIT_FILE      => RAM_INIT_FILE,
            SIM                => false,
            CLK_FREQ           => CLK_FREQUENCY,
            HAS_FPU            => HAS_FPU,
            HAS_BTC            => HAS_BTC,
            HAS_DRAM           => USE_LITEDRAM,
            DRAM_SIZE          => 256 * 1024 * 1024,
            DRAM_INIT_SIZE     => PAYLOAD_SIZE,
            HAS_SPI_FLASH      => true,
            SPI_FLASH_DLINES   => 4,
            SPI_FLASH_OFFSET   => SPI_FLASH_OFFSET,
            SPI_FLASH_DEF_CKDV => SPI_FLASH_DEF_CKDV,
            SPI_FLASH_DEF_QUAD => SPI_FLASH_DEF_QUAD,
            LOG_LENGTH         => LOG_LENGTH,
            UART0_IS_16550     => UART_IS_16550,
            HAS_UART1          => HAS_UART1,
            HAS_SD_CARD        => USE_LITESDCARD,
            ICACHE_NUM_LINES   => ICACHE_NUM_LINES,

            -- matt - shrinking
            ICACHE_TLB_SIZE    => 4,
            DCACHE_TLB_SET_SIZE    => 4,
            DCACHE_TLB_NUM_WAYS    => 1,
            DCACHE_NUM_LINES   => 4,
            ICACHE_NUM_WAYS    => 1,
            DCACHE_NUM_WAYS    => 1,

            HAS_SHORT_MULT     => true,
            NGPIO              => NGPIO
            )
        port map (
            -- System signals
            system_clk        => system_clk,
            rst               => soc_rst,

            -- UART signals
            uart0_txd         => pin_gpio_0,
            uart0_rxd         => pin_gpio_1,

            --  for scope
            uart0_scope => sdscope_extra,

	    -- UART1 signals
	    --uart1_txd         => uart_pmod_tx,
	    --uart1_rxd         => uart_pmod_rx,

            -- SPI signals
            spi_flash_sck     => spi_sck,
            spi_flash_cs_n    => spi_cs_n,
            spi_flash_sdat_o  => spi_sdat_o,
            spi_flash_sdat_oe => spi_sdat_oe,
            spi_flash_sdat_i  => spi_sdat_i,

            -- GPIO signals
            gpio_in           => gpio_in,
            gpio_out          => gpio_out,
            gpio_dir          => gpio_dir,

            -- External interrupts
            ext_irq_sdcard    => ext_irq_sdcard,

            -- DRAM wishbone
            wb_dram_in           => wb_dram_in,
            wb_dram_out          => wb_dram_out,

            -- IO wishbone
            wb_ext_io_in         => wb_ext_io_in,
            wb_ext_io_out        => wb_ext_io_out,
            wb_ext_is_dram_csr   => wb_ext_is_dram_csr,
            wb_ext_is_dram_init  => wb_ext_is_dram_init,
            wb_ext_is_sdcard     => wb_ext_is_sdcard,

            -- DMA wishbone
            wishbone_dma_in      => wb_sddma_in,
            wishbone_dma_out     => wb_sddma_out,

            alt_reset            => core_alt_reset
            );

    -- SPI Flash
    --
    spi_flash_cs_n   <= spi_cs_n;
    spi_flash_mosi   <= spi_sdat_o(0) when spi_sdat_oe(0) = '1' else 'Z';
    spi_flash_miso   <= spi_sdat_o(1) when spi_sdat_oe(1) = '1' else 'Z';
    spi_flash_wp_n   <= spi_sdat_o(2) when spi_sdat_oe(2) = '1' else 'Z';
    spi_flash_hold_n <= spi_sdat_o(3) when spi_sdat_oe(3) = '1' else 'Z';
    spi_sdat_i(0)    <= spi_flash_mosi;
    spi_sdat_i(1)    <= spi_flash_miso;
    spi_sdat_i(2)    <= spi_flash_wp_n;
    spi_sdat_i(3)    <= spi_flash_hold_n;

    uclk: USRMCLK port map (
        USRMCLKI => spi_sck,
        USRMCLKTS => '0'
        );

    nodram: if not USE_LITEDRAM generate
        signal ddram_clk_dummy : std_ulogic;
    begin
        reset_controller: entity work.soc_reset
            generic map(
                RESET_LOW => RESET_LOW
                )
            port map(
                ext_clk => ext_clk,
                pll_clk => system_clk,
                pll_locked_in => system_clk_locked,
                ext_rst_in => ext_rst_n,
                pll_rst_out => pll_rst,
                rst_out => soc_rst
                );

        clkgen: entity work.clock_generator
            generic map(
                CLK_INPUT_HZ => CLK_INPUT,
                CLK_OUTPUT_HZ => CLK_FREQUENCY
                )
            port map(
                ext_clk => ext_clk,
                pll_rst_in => pll_rst,
                pll_clk_out => system_clk,
                pll_locked_out => system_clk_locked
                );

        led0_b_pwm <= '1';
        led0_r_pwm <= '1';
        led0_g_pwm <= '0';
        core_alt_reset <= '0';

    end generate;

    has_dram: if USE_LITEDRAM generate
        signal dram_init_done  : std_ulogic;
        signal dram_init_error : std_ulogic;
        signal dram_sys_rst    : std_ulogic;
        signal rst_gen_rst     : std_ulogic;
    begin

        -- Eventually dig out the frequency from
        -- litesdram generate.py sys_clk_freq
        -- but for now, assert it's 48Mhz for orangecrab
        assert CLK_FREQUENCY = 48000000;

        reset_controller: entity work.soc_reset
            generic map(
                RESET_LOW => RESET_LOW,
                PLL_RESET_BITS => 18,
                SOC_RESET_BITS => 1
                )
            port map(
                ext_clk => ext_clk,
                pll_clk => system_clk,
                pll_locked_in => system_clk_locked,
                ext_rst_in => ext_rst_n,
                pll_rst_out => pll_rst,
                rst_out => rst_gen_rst
                );

        -- Generate SoC reset
        soc_rst_gen: process(system_clk)
        begin
            if ext_rst_n = '0' then
                soc_rst <= '1';
            elsif rising_edge(system_clk) then
                soc_rst <= dram_sys_rst or not system_clk_locked;
            end if;
        end process;

        dram: entity work.litedram_wrapper
            generic map(
                DRAM_ABITS => 24,
                DRAM_ALINES => 14,
                DRAM_DLINES => 16,
                DRAM_PORT_WIDTH => 128,
                PAYLOAD_FILE => RAM_INIT_FILE,
                PAYLOAD_SIZE => PAYLOAD_SIZE,

                -- shrink
                NUM_LINES => 2,
                NUM_WAYS => 1
                )
            port map(
                clk_in          => ext_clk,
                rst             => pll_rst,
                system_clk      => system_clk,
                system_reset    => dram_sys_rst,
                core_alt_reset  => core_alt_reset,
                pll_locked      => system_clk_locked,

                wb_in           => wb_dram_in,
                wb_out          => wb_dram_out,
                wb_ctrl_in      => wb_ext_io_in,
                wb_ctrl_out     => wb_dram_ctrl_out,
                wb_ctrl_is_csr  => wb_ext_is_dram_csr,
                wb_ctrl_is_init => wb_ext_is_dram_init,

                init_done       => dram_init_done,
                init_error      => dram_init_error,

                ddram_a         => ddram_a,
                ddram_ba        => ddram_ba,
                ddram_ras_n     => ddram_ras_n,
                ddram_cas_n     => ddram_cas_n,
                ddram_we_n      => ddram_we_n,
                ddram_cs_n      => ddram_cs_n,
                ddram_dm        => ddram_dm,
                ddram_dq        => ddram_dq,
                ddram_dqs_p     => ddram_dqs_p,
                ddram_clk_p     => ddram_clk_p,
                -- only the positive differential pin is instantiated
                --ddram_dqs_n     => ddram_dqs_n,
                --ddram_clk_n     => ddram_clk_n,
                ddram_cke       => ddram_cke,
                ddram_odt       => ddram_odt,

                ddram_reset_n   => ddram_reset_n
                );

        ddram_gnd <= "00";
        -- for power consumption.
        -- https://github.com/orangecrab-fpga/orangecrab-hardware/issues/19#issuecomment-683479378
        ddram_vccio <= "111111";

        led0_b_pwm <= not dram_init_done;
        led0_r_pwm <= dram_init_error;
        led0_g_pwm <= dram_init_done and not dram_init_error;

    end generate;

    has_litescope : if USE_LITESCOPE generate
        component litescope port (
            clk_clksys  : in    std_ulogic;
            rst         : in    std_ulogic;
            uart_bridge_tx : out std_ulogic;
            uart_bridge_rx : in  std_ulogic;

            signals_data : in std_ulogic_vector(3 downto 0);
            signals_data_oe : in std_ulogic;
            signals_cmd : in std_ulogic;
            signals_cmd_oe : in std_ulogic;
            signals_clk : in std_ulogic;
            signals_extra : in std_ulogic_vector(7 downto 0)
            );
        end component;

    begin
        lscope : litescope
        port map(
                clk_clksys    => system_clk,
                rst           => soc_rst,
                uart_bridge_tx  => pin_gpio_5,
                uart_bridge_rx  => pin_gpio_6,

                signals_data => sdscope_data,
                signals_data_oe => sdscope_data_oe,
                signals_cmd => sdscope_cmd,
                signals_cmd_oe => sdscope_cmd_oe,
                signals_clk => sdscope_clk,
                signals_extra => sdscope_extra
        );
    end generate;



    -- SD card pmod
    has_sdcard : if USE_LITESDCARD generate
        component litesdcard_core port (
            clk           : in    std_ulogic;
            rst           : in    std_ulogic;
            -- wishbone for accessing control registers
            wb_ctrl_adr   : in    std_ulogic_vector(29 downto 0);
            wb_ctrl_dat_w : in    std_ulogic_vector(31 downto 0);
            wb_ctrl_dat_r : out   std_ulogic_vector(31 downto 0);
            wb_ctrl_sel   : in    std_ulogic_vector(3 downto 0);
            wb_ctrl_cyc   : in    std_ulogic;
            wb_ctrl_stb   : in    std_ulogic;
            wb_ctrl_ack   : out   std_ulogic;
            wb_ctrl_we    : in    std_ulogic;
            wb_ctrl_cti   : in    std_ulogic_vector(2 downto 0);
            wb_ctrl_bte   : in    std_ulogic_vector(1 downto 0);
            wb_ctrl_err   : out   std_ulogic;
            -- wishbone for SD card core to use for DMA
            wb_dma_adr    : out   std_ulogic_vector(29 downto 0);
            wb_dma_dat_w  : out   std_ulogic_vector(31 downto 0);
            wb_dma_dat_r  : in    std_ulogic_vector(31 downto 0);
            wb_dma_sel    : out   std_ulogic_vector(3 downto 0);
            wb_dma_cyc    : out   std_ulogic;
            wb_dma_stb    : out   std_ulogic;
            wb_dma_ack    : in    std_ulogic;
            wb_dma_we     : out   std_ulogic;
            wb_dma_cti    : out   std_ulogic_vector(2 downto 0);
            wb_dma_bte    : out   std_ulogic_vector(1 downto 0);
            wb_dma_err    : in    std_ulogic;
            -- connections to SD card
            sdcard_data   : inout std_ulogic_vector(3 downto 0);
            sdcard_cmd    : inout std_ulogic;
            sdcard_clk    : out   std_ulogic;
            sdcard_cd     : in    std_ulogic;
            irq           : out   std_ulogic;
            -- extra debug pads to avoid tristate problems
            sdpads_data     : out std_ulogic_vector(3 downto 0);
            sdpads_data_oe  : out std_ulogic;
            sdpads_cmd      : out std_ulogic;
            sdpads_cmd_oe   : out std_ulogic;
            sdpads_clk      : out std_ulogic
            );
        end component;

        signal wb_sdcard_cyc : std_ulogic;
        signal wb_sdcard_adr : std_ulogic_vector(29 downto 0);

    begin
        litesdcard : litesdcard_core
            port map (
                clk           => system_clk,
                rst           => soc_rst,
                wb_ctrl_adr   => wb_sdcard_adr,
                wb_ctrl_dat_w => wb_ext_io_in.dat,
                wb_ctrl_dat_r => wb_sdcard_out.dat,
                wb_ctrl_sel   => wb_ext_io_in.sel,
                wb_ctrl_cyc   => wb_sdcard_cyc,
                wb_ctrl_stb   => wb_ext_io_in.stb,
                wb_ctrl_ack   => wb_sdcard_out.ack,
                wb_ctrl_we    => wb_ext_io_in.we,
                wb_ctrl_cti   => "000",
                wb_ctrl_bte   => "00",
                wb_ctrl_err   => open,
                wb_dma_adr    => wb_sddma_nr.adr,
                wb_dma_dat_w  => wb_sddma_nr.dat,
                wb_dma_dat_r  => wb_sddma_ir.dat,
                wb_dma_sel    => wb_sddma_nr.sel,
                wb_dma_cyc    => wb_sddma_nr.cyc,
                wb_dma_stb    => wb_sddma_nr.stb,
                wb_dma_ack    => wb_sddma_ir.ack,
                wb_dma_we     => wb_sddma_nr.we,
                wb_dma_cti    => open,
                wb_dma_bte    => open,
                wb_dma_err    => '0',
                sdcard_data   => sdcard_data,
                sdcard_cmd    => sdcard_cmd,
                sdcard_clk    => sdcard_clk,
                sdcard_cd     => sdcard_cd,
                sdpads_cmd    => sdscope_cmd,
                sdpads_cmd_oe => sdscope_cmd_oe,
                sdpads_data   => sdscope_data,
                sdpads_data_oe=> sdscope_data_oe,
                sdpads_clk    => sdscope_clk,
                irq           => ext_irq_sdcard
                );

        -- Gate cyc with chip select from SoC
        wb_sdcard_cyc <= wb_ext_io_in.cyc and wb_ext_is_sdcard;

        wb_sdcard_adr <= x"0000" & wb_ext_io_in.adr(13 downto 0);

        wb_sdcard_out.stall <= not wb_sdcard_out.ack;

        -- Convert non-pipelined DMA wishbone to pipelined by suppressing
        -- non-acknowledged strobes
        process(system_clk)
        begin
            if rising_edge(system_clk) then
                wb_sddma_out <= wb_sddma_nr;
                if wb_sddma_stb_sent = '1' or
                    (wb_sddma_out.stb = '1' and wb_sddma_in.stall = '0') then
                    wb_sddma_out.stb <= '0';
                end if;
                if wb_sddma_nr.cyc = '0' or wb_sddma_ir.ack = '1' then
                    wb_sddma_stb_sent <= '0';
                elsif wb_sddma_in.stall = '0' then
                    wb_sddma_stb_sent <= wb_sddma_nr.stb;
                end if;
                wb_sddma_ir <= wb_sddma_in;
            end if;
        end process;

    end generate;

    -- Mux WB response on the IO bus
    wb_ext_io_out <= wb_sdcard_out when wb_ext_is_sdcard = '1' else
                     wb_dram_ctrl_out;

    leds_pwm : process(system_clk)
    begin
        if rising_edge(system_clk) then
            pwm_counter <= std_ulogic_vector(signed(pwm_counter) + 1);
            if pwm_counter(8 downto 4) = "00000" then
                led0_b <= led0_b_pwm;
                led0_r <= led0_r_pwm;
                led0_g <= led0_g_pwm;
            else
                led0_b <= '0';
                led0_r <= '0';
                led0_g <= '0';
            end if;
        end if;
    end process;

end architecture behaviour;
