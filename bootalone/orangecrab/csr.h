//--------------------------------------------------------------------------------
// Auto-generated by Migen (--------) & LiteX (--------) on 2021-10-21 15:39:27
//--------------------------------------------------------------------------------
#include <generated/soc.h>
#ifndef __GENERATED_CSR_H
#define __GENERATED_CSR_H
#include <stdint.h>
#include <system.h>
#ifndef CSR_ACCESSORS_DEFINED
#include <hw/common.h>
#endif /* ! CSR_ACCESSORS_DEFINED */
#ifndef CSR_BASE
#define CSR_BASE 0x0L
#endif

/* ddrctrl */
#define CSR_DDRCTRL_BASE (CSR_BASE + 0x0L)
#define CSR_DDRCTRL_INIT_DONE_ADDR (CSR_BASE + 0x0L)
#define CSR_DDRCTRL_INIT_DONE_SIZE 1
static inline uint32_t ddrctrl_init_done_read(void) {
	return csr_read_simple(CSR_BASE + 0x0L);
}
static inline void ddrctrl_init_done_write(uint32_t v) {
	csr_write_simple(v, CSR_BASE + 0x0L);
}
#define CSR_DDRCTRL_INIT_ERROR_ADDR (CSR_BASE + 0x4L)
#define CSR_DDRCTRL_INIT_ERROR_SIZE 1
static inline uint32_t ddrctrl_init_error_read(void) {
	return csr_read_simple(CSR_BASE + 0x4L);
}
static inline void ddrctrl_init_error_write(uint32_t v) {
	csr_write_simple(v, CSR_BASE + 0x4L);
}

/* ddrphy */
#define CSR_DDRPHY_BASE (CSR_BASE + 0x800L)
#define CSR_DDRPHY_DLY_SEL_ADDR (CSR_BASE + 0x800L)
#define CSR_DDRPHY_DLY_SEL_SIZE 1
static inline uint32_t ddrphy_dly_sel_read(void) {
	return csr_read_simple(CSR_BASE + 0x800L);
}
static inline void ddrphy_dly_sel_write(uint32_t v) {
	csr_write_simple(v, CSR_BASE + 0x800L);
}
#define CSR_DDRPHY_RDLY_DQ_RST_ADDR (CSR_BASE + 0x804L)
#define CSR_DDRPHY_RDLY_DQ_RST_SIZE 1
static inline uint32_t ddrphy_rdly_dq_rst_read(void) {
	return csr_read_simple(CSR_BASE + 0x804L);
}
static inline void ddrphy_rdly_dq_rst_write(uint32_t v) {
	csr_write_simple(v, CSR_BASE + 0x804L);
}
#define CSR_DDRPHY_RDLY_DQ_INC_ADDR (CSR_BASE + 0x808L)
#define CSR_DDRPHY_RDLY_DQ_INC_SIZE 1
static inline uint32_t ddrphy_rdly_dq_inc_read(void) {
	return csr_read_simple(CSR_BASE + 0x808L);
}
static inline void ddrphy_rdly_dq_inc_write(uint32_t v) {
	csr_write_simple(v, CSR_BASE + 0x808L);
}
#define CSR_DDRPHY_RDLY_DQ_BITSLIP_RST_ADDR (CSR_BASE + 0x80cL)
#define CSR_DDRPHY_RDLY_DQ_BITSLIP_RST_SIZE 1
static inline uint32_t ddrphy_rdly_dq_bitslip_rst_read(void) {
	return csr_read_simple(CSR_BASE + 0x80cL);
}
static inline void ddrphy_rdly_dq_bitslip_rst_write(uint32_t v) {
	csr_write_simple(v, CSR_BASE + 0x80cL);
}
#define CSR_DDRPHY_RDLY_DQ_BITSLIP_ADDR (CSR_BASE + 0x810L)
#define CSR_DDRPHY_RDLY_DQ_BITSLIP_SIZE 1
static inline uint32_t ddrphy_rdly_dq_bitslip_read(void) {
	return csr_read_simple(CSR_BASE + 0x810L);
}
static inline void ddrphy_rdly_dq_bitslip_write(uint32_t v) {
	csr_write_simple(v, CSR_BASE + 0x810L);
}
#define CSR_DDRPHY_BURSTDET_CLR_ADDR (CSR_BASE + 0x814L)
#define CSR_DDRPHY_BURSTDET_CLR_SIZE 1
static inline uint32_t ddrphy_burstdet_clr_read(void) {
	return csr_read_simple(CSR_BASE + 0x814L);
}
static inline void ddrphy_burstdet_clr_write(uint32_t v) {
	csr_write_simple(v, CSR_BASE + 0x814L);
}
#define CSR_DDRPHY_BURSTDET_SEEN_ADDR (CSR_BASE + 0x818L)
#define CSR_DDRPHY_BURSTDET_SEEN_SIZE 1
static inline uint32_t ddrphy_burstdet_seen_read(void) {
	return csr_read_simple(CSR_BASE + 0x818L);
}

/* sdram */
#define CSR_SDRAM_BASE (CSR_BASE + 0x1000L)
#define CSR_SDRAM_DFII_CONTROL_ADDR (CSR_BASE + 0x1000L)
#define CSR_SDRAM_DFII_CONTROL_SIZE 1
static inline uint32_t sdram_dfii_control_read(void) {
	return csr_read_simple(CSR_BASE + 0x1000L);
}
static inline void sdram_dfii_control_write(uint32_t v) {
	csr_write_simple(v, CSR_BASE + 0x1000L);
}
#define CSR_SDRAM_DFII_CONTROL_SEL_OFFSET 0
#define CSR_SDRAM_DFII_CONTROL_SEL_SIZE 1
static inline uint32_t sdram_dfii_control_sel_extract(uint32_t oldword) {
	uint32_t mask = ((1 << 1)-1);
	return ( (oldword >> 0) & mask );
}
static inline uint32_t sdram_dfii_control_sel_read(void) {
	uint32_t word = sdram_dfii_control_read();
	return sdram_dfii_control_sel_extract(word);
}
static inline uint32_t sdram_dfii_control_sel_replace(uint32_t oldword, uint32_t plain_value) {
	uint32_t mask = ((1 << 1)-1);
	return (oldword & (~(mask << 0))) | (mask & plain_value)<< 0 ;
}
static inline void sdram_dfii_control_sel_write(uint32_t plain_value) {
	uint32_t oldword = sdram_dfii_control_read();
	uint32_t newword = sdram_dfii_control_sel_replace(oldword, plain_value);
	sdram_dfii_control_write(newword);
}
#define CSR_SDRAM_DFII_CONTROL_CKE_OFFSET 1
#define CSR_SDRAM_DFII_CONTROL_CKE_SIZE 1
static inline uint32_t sdram_dfii_control_cke_extract(uint32_t oldword) {
	uint32_t mask = ((1 << 1)-1);
	return ( (oldword >> 1) & mask );
}
static inline uint32_t sdram_dfii_control_cke_read(void) {
	uint32_t word = sdram_dfii_control_read();
	return sdram_dfii_control_cke_extract(word);
}
static inline uint32_t sdram_dfii_control_cke_replace(uint32_t oldword, uint32_t plain_value) {
	uint32_t mask = ((1 << 1)-1);
	return (oldword & (~(mask << 1))) | (mask & plain_value)<< 1 ;
}
static inline void sdram_dfii_control_cke_write(uint32_t plain_value) {
	uint32_t oldword = sdram_dfii_control_read();
	uint32_t newword = sdram_dfii_control_cke_replace(oldword, plain_value);
	sdram_dfii_control_write(newword);
}
#define CSR_SDRAM_DFII_CONTROL_ODT_OFFSET 2
#define CSR_SDRAM_DFII_CONTROL_ODT_SIZE 1
static inline uint32_t sdram_dfii_control_odt_extract(uint32_t oldword) {
	uint32_t mask = ((1 << 1)-1);
	return ( (oldword >> 2) & mask );
}
static inline uint32_t sdram_dfii_control_odt_read(void) {
	uint32_t word = sdram_dfii_control_read();
	return sdram_dfii_control_odt_extract(word);
}
static inline uint32_t sdram_dfii_control_odt_replace(uint32_t oldword, uint32_t plain_value) {
	uint32_t mask = ((1 << 1)-1);
	return (oldword & (~(mask << 2))) | (mask & plain_value)<< 2 ;
}
static inline void sdram_dfii_control_odt_write(uint32_t plain_value) {
	uint32_t oldword = sdram_dfii_control_read();
	uint32_t newword = sdram_dfii_control_odt_replace(oldword, plain_value);
	sdram_dfii_control_write(newword);
}
#define CSR_SDRAM_DFII_CONTROL_RESET_N_OFFSET 3
#define CSR_SDRAM_DFII_CONTROL_RESET_N_SIZE 1
static inline uint32_t sdram_dfii_control_reset_n_extract(uint32_t oldword) {
	uint32_t mask = ((1 << 1)-1);
	return ( (oldword >> 3) & mask );
}
static inline uint32_t sdram_dfii_control_reset_n_read(void) {
	uint32_t word = sdram_dfii_control_read();
	return sdram_dfii_control_reset_n_extract(word);
}
static inline uint32_t sdram_dfii_control_reset_n_replace(uint32_t oldword, uint32_t plain_value) {
	uint32_t mask = ((1 << 1)-1);
	return (oldword & (~(mask << 3))) | (mask & plain_value)<< 3 ;
}
static inline void sdram_dfii_control_reset_n_write(uint32_t plain_value) {
	uint32_t oldword = sdram_dfii_control_read();
	uint32_t newword = sdram_dfii_control_reset_n_replace(oldword, plain_value);
	sdram_dfii_control_write(newword);
}
#define CSR_SDRAM_DFII_PI0_COMMAND_ADDR (CSR_BASE + 0x1004L)
#define CSR_SDRAM_DFII_PI0_COMMAND_SIZE 1
static inline uint32_t sdram_dfii_pi0_command_read(void) {
	return csr_read_simple(CSR_BASE + 0x1004L);
}
static inline void sdram_dfii_pi0_command_write(uint32_t v) {
	csr_write_simple(v, CSR_BASE + 0x1004L);
}
#define CSR_SDRAM_DFII_PI0_COMMAND_ISSUE_ADDR (CSR_BASE + 0x1008L)
#define CSR_SDRAM_DFII_PI0_COMMAND_ISSUE_SIZE 1
static inline uint32_t sdram_dfii_pi0_command_issue_read(void) {
	return csr_read_simple(CSR_BASE + 0x1008L);
}
static inline void sdram_dfii_pi0_command_issue_write(uint32_t v) {
	csr_write_simple(v, CSR_BASE + 0x1008L);
}
#define CSR_SDRAM_DFII_PI0_ADDRESS_ADDR (CSR_BASE + 0x100cL)
#define CSR_SDRAM_DFII_PI0_ADDRESS_SIZE 1
static inline uint32_t sdram_dfii_pi0_address_read(void) {
	return csr_read_simple(CSR_BASE + 0x100cL);
}
static inline void sdram_dfii_pi0_address_write(uint32_t v) {
	csr_write_simple(v, CSR_BASE + 0x100cL);
}
#define CSR_SDRAM_DFII_PI0_BADDRESS_ADDR (CSR_BASE + 0x1010L)
#define CSR_SDRAM_DFII_PI0_BADDRESS_SIZE 1
static inline uint32_t sdram_dfii_pi0_baddress_read(void) {
	return csr_read_simple(CSR_BASE + 0x1010L);
}
static inline void sdram_dfii_pi0_baddress_write(uint32_t v) {
	csr_write_simple(v, CSR_BASE + 0x1010L);
}
#define CSR_SDRAM_DFII_PI0_WRDATA_ADDR (CSR_BASE + 0x1014L)
#define CSR_SDRAM_DFII_PI0_WRDATA_SIZE 2
static inline uint64_t sdram_dfii_pi0_wrdata_read(void) {
	uint64_t r = csr_read_simple(CSR_BASE + 4116L);
	r <<= 32;
	r |= csr_read_simple(CSR_BASE + 0x1018L);
	return r;
}
static inline void sdram_dfii_pi0_wrdata_write(uint64_t v) {
	csr_write_simple(v >> 32, CSR_BASE + 0x1014L);
	csr_write_simple(v, CSR_BASE + 0x1018L);
}
#define CSR_SDRAM_DFII_PI0_RDDATA_ADDR (CSR_BASE + 0x101cL)
#define CSR_SDRAM_DFII_PI0_RDDATA_SIZE 2
static inline uint64_t sdram_dfii_pi0_rddata_read(void) {
	uint64_t r = csr_read_simple(CSR_BASE + 4124L);
	r <<= 32;
	r |= csr_read_simple(CSR_BASE + 0x1020L);
	return r;
}
#define CSR_SDRAM_DFII_PI1_COMMAND_ADDR (CSR_BASE + 0x1024L)
#define CSR_SDRAM_DFII_PI1_COMMAND_SIZE 1
static inline uint32_t sdram_dfii_pi1_command_read(void) {
	return csr_read_simple(CSR_BASE + 0x1024L);
}
static inline void sdram_dfii_pi1_command_write(uint32_t v) {
	csr_write_simple(v, CSR_BASE + 0x1024L);
}
#define CSR_SDRAM_DFII_PI1_COMMAND_ISSUE_ADDR (CSR_BASE + 0x1028L)
#define CSR_SDRAM_DFII_PI1_COMMAND_ISSUE_SIZE 1
static inline uint32_t sdram_dfii_pi1_command_issue_read(void) {
	return csr_read_simple(CSR_BASE + 0x1028L);
}
static inline void sdram_dfii_pi1_command_issue_write(uint32_t v) {
	csr_write_simple(v, CSR_BASE + 0x1028L);
}
#define CSR_SDRAM_DFII_PI1_ADDRESS_ADDR (CSR_BASE + 0x102cL)
#define CSR_SDRAM_DFII_PI1_ADDRESS_SIZE 1
static inline uint32_t sdram_dfii_pi1_address_read(void) {
	return csr_read_simple(CSR_BASE + 0x102cL);
}
static inline void sdram_dfii_pi1_address_write(uint32_t v) {
	csr_write_simple(v, CSR_BASE + 0x102cL);
}
#define CSR_SDRAM_DFII_PI1_BADDRESS_ADDR (CSR_BASE + 0x1030L)
#define CSR_SDRAM_DFII_PI1_BADDRESS_SIZE 1
static inline uint32_t sdram_dfii_pi1_baddress_read(void) {
	return csr_read_simple(CSR_BASE + 0x1030L);
}
static inline void sdram_dfii_pi1_baddress_write(uint32_t v) {
	csr_write_simple(v, CSR_BASE + 0x1030L);
}
#define CSR_SDRAM_DFII_PI1_WRDATA_ADDR (CSR_BASE + 0x1034L)
#define CSR_SDRAM_DFII_PI1_WRDATA_SIZE 2
static inline uint64_t sdram_dfii_pi1_wrdata_read(void) {
	uint64_t r = csr_read_simple(CSR_BASE + 4148L);
	r <<= 32;
	r |= csr_read_simple(CSR_BASE + 0x1038L);
	return r;
}
static inline void sdram_dfii_pi1_wrdata_write(uint64_t v) {
	csr_write_simple(v >> 32, CSR_BASE + 0x1034L);
	csr_write_simple(v, CSR_BASE + 0x1038L);
}
#define CSR_SDRAM_DFII_PI1_RDDATA_ADDR (CSR_BASE + 0x103cL)
#define CSR_SDRAM_DFII_PI1_RDDATA_SIZE 2
static inline uint64_t sdram_dfii_pi1_rddata_read(void) {
	uint64_t r = csr_read_simple(CSR_BASE + 4156L);
	r <<= 32;
	r |= csr_read_simple(CSR_BASE + 0x1040L);
	return r;
}

#endif
