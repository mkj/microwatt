#!/usr/bin/env python3
# This variable defines all the external programs that this module
# relies on.  lxbuildenv reads this variable in order to ensure
# the build will finish without exiting due to missing third-party
# programs.
LX_DEPENDENCIES = []

# Import lxbuildenv to integrate the deps/ directory
#import lxbuildenv

# Disable pylint's E1101, which breaks completely on migen
#pylint:disable=E1101

import argparse
import os
import yaml

from math import log2

#from migen import *
from migen import Module, Signal, Instance, ClockDomain, If
from migen.genlib.resetsync import AsyncResetSynchronizer
from migen.genlib.misc import WaitTimer
from migen.fhdl.specials import TSTriple
from migen.fhdl.bitcontainer import bits_for
from migen.fhdl.structure import ClockSignal, ResetSignal, Replicate, Cat

# from litex.build.sim.platform import SimPlatform
from litex.build.lattice import LatticePlatform
from litex.build.generic_platform import Pins, IOStandard, Misc, Subsignal
from litex.soc.integration.soc_core import SoCCore
from litex.soc.integration.builder import Builder
from litex.soc.interconnect import wishbone, stream
from litex.soc.interconnect.csr import AutoCSR, CSRStatus, CSRStorage

from litescope import LiteScopeAnalyzer

_connectors = []

class _CRG(Module):
    def __init__(self, platform):
        clk = platform.request("clk")
        rst = platform.request("rst")

        self.clock_domains.cd_sys = ClockDomain()
        self.comb += self.cd_sys.clk.eq(clk.clksys)

        self.comb += [
            ResetSignal("sys").eq(rst),
        ]

class BaseSoC(SoCCore):

    def __init__(self, platform, io, sys_freq, output_dir="build", **kwargs):
        # Disable integrated RAM as we'll add it later
        self.integrated_sram_size = 0

        self.output_dir = output_dir

        platform.add_extension(io)
        self.submodules.crg = _CRG(platform)

        SoCCore.__init__(self, platform, sys_freq,
            cpu_type=None,
            integrated_rom_size=0x0,
            integrated_sram_size=0x0,
            integrated_main_ram_size=0x0,
            csr_address_width=14, csr_data_width=32,
            with_uart=False, with_timer=False)

        self.add_uartbone(name="uart_bridge")
        #self.add_jtagbone()

        self.submodules.analyzer = LiteScopeAnalyzer(platform.request("signals"),
            depth        = 2048,
            clock_domain = "sys",
            csr_csv      = "analyzer.csv")

_io = [
    # Wishbone
    ("clk", 0,
        Subsignal("clksys", Pins(1)),
    ),
    ("rst", 0, Pins(1)),
    ("uart_bridge", 0,
        Subsignal("tx", Pins(1)),
        Subsignal("rx", Pins(1)),
    ),
    ("signals", 0,
        Subsignal("signals", Pins(12)),
    ),
]

def generate(core_config, output_dir, csr_csv):

    toolchain = core_config["toolchain"]
    if toolchain == "trellis":
        platform = LatticePlatform(core_config["device"], [], toolchain=toolchain)
    else:
        raise ValueError(f"Unknown config toolchain {toolchain}")

    soc = BaseSoC(platform, _io, core_config["sys_freq"],
                            cpu_type=None, cpu_variant=None,
                            output_dir=output_dir)
    builder = Builder(soc, output_dir=output_dir,
                           csr_csv=csr_csv,
                           compile_software=False)
    vns = builder.build(run=False, build_name='litescope')
    soc.do_exit(vns)

def main():
    parser = argparse.ArgumentParser(description="Build standalone litescope")
    parser.add_argument('--dir', metavar='DIRECTORY',
                                 default='build',
                                 help='Output directory (default: %(default)s)' )
    parser.add_argument('--csr', metavar='CSR',
                                 default='csr.csv',
                                 help='csr file (default: %(default)s)')
    parser.add_argument('config', type=argparse.FileType('r'),
                                 help='Input platform config file')
    args = parser.parse_args()

    core_config = yaml.load(args.config.read(), Loader=yaml.Loader)
    output_dir = args.dir
    generate(core_config, output_dir, args.csr)

    print(
"""Build complete.  Output files:
    {}/gateware/litescope.v               Source Verilog file.
""".format(output_dir))

if __name__ == "__main__":
    main()
