# Copyright lowRISC contributors.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0

.section .text.init

default_exc_handler:
  jal x0, _cva6_exc_handler

reset_handler:
  /* set all registers to zero */
  mv  x1, x0
  mv  x2, x1
  mv  x3, x1
  mv  x4, x1
  mv  x5, x1
  mv  x6, x1
  mv  x7, x1
  mv  x8, x1
  mv  x9, x1
  mv x10, x1
  mv x11, x1
  mv x12, x1
  mv x13, x1
  mv x14, x1
  mv x15, x1
  mv x16, x1
  mv x17, x1
  mv x18, x1
  mv x19, x1
  mv x20, x1
  mv x21, x1
  mv x22, x1
  mv x23, x1
  mv x24, x1
  mv x25, x1
  mv x26, x1
  mv x27, x1
  mv x28, x1
  mv x29, x1
  mv x30, x1
  mv x31, x1

  /* fall thru to stack initilization */
_start:
  .global _start

  .option push
  .option norelax
  la sp, _boot_stack
  .option pop
  li t0, (8*4096)
  add sp, sp, t0

  /* Route all traps/exceptions to our machine-mode handler. */
  .option push
  .option norelax
  la t0, default_exc_handler
  .option pop
  csrw mtvec, t0

  /* clear BSS */
  la x26, _bss_start
  la x27, _bss_end

  bge x26, x27, zero_loop_end

zero_loop:
  sd x0, 0(x26)
  addi x26, x26, 8
  blt x26, x27, zero_loop
zero_loop_end:

  /* jump to main program entry point (argc = argv = 0) */
  addi x10, x0, 0
  addi x11, x0, 0
  jal x1, main

  /* if we return from main, call exit() */
  tail _cva6_exit
