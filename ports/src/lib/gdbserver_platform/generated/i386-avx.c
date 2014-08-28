/* *INDENT-OFF* */ /* THIS FILE IS GENERATED */

/* A register protocol for GDB, the GNU debugger.
   Copyright 2001, 2002 Free Software Foundation, Inc.

   This file is part of GDB.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* This file was created with the aid of ``regdat.sh'' and ``/home/wangyuqing/Downloads/genode-master/ports/contrib/gdb-7.3.1/gdb/regformats/i386/i386-avx.dat''.  */

#include "server.h"
#include "regdef.h"

struct reg regs_i386_avx[] = {
  { "eax", 0, 32 },
  { "ecx", 32, 32 },
  { "edx", 64, 32 },
  { "ebx", 96, 32 },
  { "esp", 128, 32 },
  { "ebp", 160, 32 },
  { "esi", 192, 32 },
  { "edi", 224, 32 },
  { "eip", 256, 32 },
  { "eflags", 288, 32 },
  { "cs", 320, 32 },
  { "ss", 352, 32 },
  { "ds", 384, 32 },
  { "es", 416, 32 },
  { "fs", 448, 32 },
  { "gs", 480, 32 },
  { "st0", 512, 80 },
  { "st1", 592, 80 },
  { "st2", 672, 80 },
  { "st3", 752, 80 },
  { "st4", 832, 80 },
  { "st5", 912, 80 },
  { "st6", 992, 80 },
  { "st7", 1072, 80 },
  { "fctrl", 1152, 32 },
  { "fstat", 1184, 32 },
  { "ftag", 1216, 32 },
  { "fiseg", 1248, 32 },
  { "fioff", 1280, 32 },
  { "foseg", 1312, 32 },
  { "fooff", 1344, 32 },
  { "fop", 1376, 32 },
  { "xmm0", 1408, 128 },
  { "xmm1", 1536, 128 },
  { "xmm2", 1664, 128 },
  { "xmm3", 1792, 128 },
  { "xmm4", 1920, 128 },
  { "xmm5", 2048, 128 },
  { "xmm6", 2176, 128 },
  { "xmm7", 2304, 128 },
  { "mxcsr", 2432, 32 },
  { "ymm0h", 2464, 128 },
  { "ymm1h", 2592, 128 },
  { "ymm2h", 2720, 128 },
  { "ymm3h", 2848, 128 },
  { "ymm4h", 2976, 128 },
  { "ymm5h", 3104, 128 },
  { "ymm6h", 3232, 128 },
  { "ymm7h", 3360, 128 },
};

const char *expedite_regs_i386_avx[] = { "ebp", "esp", "eip", 0 };
const char *xmltarget_i386_avx = "i386-avx.xml";

void
init_registers_i386_avx ()
{
    set_register_cache (regs_i386_avx,
			sizeof (regs_i386_avx) / sizeof (regs_i386_avx[0]));
    gdbserver_expedite_regs = expedite_regs_i386_avx;
    gdbserver_xmltarget = xmltarget_i386_avx;
}
