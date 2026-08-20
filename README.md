# atarist-programs-lab

A collection of Atari ST programs, focused on real-time 3D rendering written in
C for the original ST hardware. This repo holds the source, the reference
examples used while learning the toolchain, and a set of PC-originated programs
that are being ported over to the ST.

## Toolchain / ST tools setup

These programs are built with the classic **Pure C** toolchain for the Atari ST,
optionally on top of the **GODLIB** support library. The compiler archives live
in [ST_SOFTWARE/](ST_SOFTWARE/):

- [ST_SOFTWARE/PURE_C.ZIP](ST_SOFTWARE/PURE_C.ZIP) — Pure C, the freeware
  C compiler and IDE for the Atari ST (successor to Turbo C). It provides the
  compiler, linker, `PCSTART.O` startup module, and the standard libraries
  (`PCSTDLIB.LIB`, `PCTOSLIB.LIB`, `PCFLTLIB.LIB`).
- [ST_SOFTWARE/GODLIB.ZIP](ST_SOFTWARE/GODLIB.ZIP) — GODLIB, a demo/game
  development library for the Atari ST that wraps low-level hardware access
  (video, VBL, keyboard/IKBD, memory, graphics, etc.).

### Setup

1. Run an Atari ST (real hardware or an emulator such as Hatari/Steem).
2. Unzip `PURE_C.ZIP` and install the Pure C IDE on the ST.
3. If a program uses GODLIB, unzip `GODLIB.ZIP` and make its `INCLUDE\GODLIB`
   headers and source available to the project.
4. Open the relevant `.PRJ` project file in the Pure C IDE and build it. Output
   is a `.TOS` / `.PRG` executable you can run under TOS.

### Project files

Builds are driven by Pure C `.PRJ` project files, which list the output binary,
compiler/assembler options, and the modules to compile and link. See
[rendering/RENDER.PRJ](rendering/RENDER.PRJ) for a minimal TOS example, and
[ref-src/SCREEN/SCREEN.PRJ](ref-src/SCREEN/SCREEN.PRJ) for a GODLIB-based one.

Compiled artifacts (`*.O`, `*.TOS`, `*.PRG`, `*.PI1`, etc.) are ignored via
[.gitignore](.gitignore) and are not tracked.

## Repository layout

- [ST_SOFTWARE/](ST_SOFTWARE/) — the Pure C and GODLIB toolchain archives.
- [ref-src/](ref-src/) — small reference programs (`SCREEN`, `SPRITE`) used to
  learn the toolchain and GODLIB APIs.
- [rendering/](rendering/) — the main 3D rendering project. Includes
  [rendering/cube.c](rendering/cube.c), [rendering/polar_curves.c](rendering/polar_curves.c),
  and the plain-TOS double-buffering base in [rendering/BASE_SC.C](rendering/BASE_SC.C),
  plus shared headers in [rendering/INCLUDE/](rendering/INCLUDE/) and 3D data in
  [rendering/MODELS/](rendering/MODELS/) (e.g. the Utah teapot).
- [rendering/to_port/](rendering/to_port/) — see below.

## `to_port` — PC code awaiting a port

The programs in [rendering/to_port/](rendering/to_port/) are **originally from the
PC**. They were written against the [Allegro](https://liballeg.org/) game
library (note the `#include <allegro5/...>` headers) and target desktop
resolutions and APIs, so they do **not** build on the Atari ST as-is.

They are kept here as the source material for the rendering work: each will be
**ported to the Atari ST** — replacing Allegro's windowing, input, and drawing
primitives with the ST equivalents (TOS/GODLIB video, IKBD input, and custom
line/triangle rasterization) — and then folded into the main `rendering`
project alongside `cube.c` and `polar_curves.c`.

Contents include the Utah teapot rendering stages (`teapot_01.c` … `teapot_08.c`,
`teapot.c`) and standalone primitive tests (`test_lines.c`, `test_triangle.c`,
`test_triangle_color.c`, `test_bezier.c`).
