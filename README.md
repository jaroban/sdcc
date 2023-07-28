# SDCC - free open source, retargettable, optimizing ISO C compiler

Welcome to SDCC, free open source, retargettable, optimizing ISO C
compiler suite that targets a growing list of processors including the
Intel MCS-51 based microprocessors (8031, 8032, 8051, 8052, etc.), Maxim
(formerly Dallas) DS80C390 variants, Freescale (formerly Motorola) HC08
based (hc08, s08), Zilog Z80 based MCUs (Z80, Z80N, Z180, SM83 (e.g. Game Boy),
Rabbit 2000, Rabbit 2000A/3000, Rabbit 3000A, TLCS-90), STMicroelectronics STM8,
Padauk PDK14 and PDK15 and MOS 6502.
Work is in progress on supporting the Padauk PDK13 target.
There are unmaintained Microchip PIC16 and PIC18 targets.
It can be retargeted for other microprocessors.

See `share/sdcc/doc/INSTALL.txt` for the installation instructions for
the binary kits.

See the `share/sdcc/doc` directory for more documentation.

See http://sdcc.sourceforge.net/ for the latest information on sdcc.


## Licenses

SDCC compiler suite is a collection of several components derived from
different sources with different FOSS licenses. See the `sdccman.pdf`
document, chapter "SDCC Suite Licenses" for details.

The code or object files generated by SDCC suite are not licensed, so
they can be used in FLOSS or proprietary (closed source) applications.

The great majority of sdcc run-time libraries are licensed under the
GPLv2+LE which allows linking of sdcc run time libraries with
proprietary (closed source) applications.

Exception are pic device libraries and header files which are derived
from Microchip header (.inc) and linker script (.lkr) files. Microchip
requires that "The header files should state that they are only to be
used with authentic Microchip devices" which makes them incompatible
with the GPL. Pic device libraries and header files are located at
non-free/lib and non-free/include directories respectively. Sdcc should
be run with the `--use-non-free` command line option in order to include
the potentially non-free header files and libraries.

However: Many think that the Microchip requirement is not legally enforceable,
arguing that the header files only contain noncopyrightable facts.

See:
   - http://www.gnu.org/copyleft/gpl.html
   - http://sourceforge.net/apps/trac/sdcc/wiki/SDCC%20Library%20Licenses


## Support

SDCC compiler suite is a collaborative effort between a group of
volunteers. Please feel free to report bugs via the Sourceforge bug
tracker, or to ask questions on the user mailing list. See:

   - http://sourceforge.net/tracker/?atid=100599&group_id=599&func=browse
   - http://sourceforge.net/mail/?group_id=599

Notes:
   * The ds400, pic14 and pic16 are currently experimental, but work is in
     progress. Please check http://sdcc.sourceforge.net/snap.php for snapshots.
   * The TININative, avr and xa51 targets are no longer maintained.


## Roadmap

   - COPYING    - GPL v2
   - debugger/  - sdcdb debugger
   - device/    - Device libraries and examples
   - doc/       - sdcc and target specific documentation
   - sdas/      - Assemblers and linkers, derived from asxxxx
   - sim/       - ucsim simulator
   - src/       - Source to the C compiler and targets
   - support/   - Libraries and apps that may not be provided by your system.


## Authors and interested persons

   * Sandeep Dutta        <sandeep AT users.sourceforge.net>
   * Jean Loius-VERN      <jlvern AT writeme.com>
   * Daniel Drotos        <drdani AT mazsola.iit.uni-miskolc.hu>
   * Kevin Vigor          <kevin AT vigor.nu>
   * Johan Knol           <johan.knol AT iduna.nl>
   * Scott Dattalo        <scott AT dattalo.com>
   * Karl Bongers         <karl AT turbobit.com>
   * Bernhard Held        <bernhard AT bernhardheld.de>
   * Frieder Ferlemann    <frieder.ferlemann AT web.de>
   * Jesus Calvino-Fraga  <jesusc AT ece.ubc.ca>
   * Borut Razem          <borut.razem AT siol.net>
   * Vangelis Rokas       <vrokas AT otenet.gr>
   * Erik Petrich         <epetrich AT ivorytower.norman.ok.us>
   * Paul Stoffregen      <paul AT pjrc.com>
   * Michael Hope         <michaelh AT juju.net.nz> <nz_michaelh AT yahoo.com>
   * Maarten Brock        <sourceforge.brock AT dse.nl>
   * Raphael Neider       <RNeider AT web.de>
   * Hans-Juergen Dorn    <hans.dorn AT apl-landau.de>
   * Klaus Flittner       <klaus_flittner AT gmx.de>
   * Philipp Klaus Krause <pkk AT spth.de>
   * Leland Morrison      <enigmalee.AT.sourceforget.net>
   * Molnar Karoly        <molnarkaroly AT users.sf.net>
   * Ben Shi              <powerstudio1st AT 163.com>

SDCC includes code from:
   * Alan Baldwin <baldwin AT shop-pdp.kent.edu>
      Initial version of ASXXXX and  ASLINK. 
   * John Hartman <noice AT noicedebugger.com>
      Porting ASXXXX and ASLINK for 8051.
   * Dmitry S. Obukhov <dso AT usa.net>
      malloc and serial I/O routines.
   * Pascal Felber
      Some of the Z80 related files are borrowed from the Gameboy
      Development Kit (GBDK).
   * The GCC development team http://gcc.gnu.org/
      for GNU C preprocessor, the basis of sdcpp preprocessor and
      gcc test suite, partially included into the SDCC regression test suite
   * The GNU Binutils development team http://www.gnu.org/software/binutils/
      for GNU Binutils, the basis of sdbinutils
   * Boost Community http://www.boost.org/
      for Boost C++ libraries used in sdcc compiler
   * Timo Bingmann http://idlebox.net/2007/stx-btree/
      for TX B+ Tree C++ Template Classes used in sdcc compiler
   * Malini Dutta <malini AT mediaone.net>
      Sandeep's wife, for her patience and support.
