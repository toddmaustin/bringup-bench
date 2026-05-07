# Copyright lowRISC contributors.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0

.section .text.init

_start:
  .global _start

  .option push
  .option norelax
  la sp, _boot_stack
  .option pop
  li t0, (8*4096)
  add sp, sp, t0

  /* Route all traps/exceptions to _cva6_trap_entry in direct mode. */
  la t0, _cva6_trap_entry
  csrw mtvec, t0

  /* clear BSS */
  la x26, _bss_start
  la x27, _bss_end

  bge x26, x27, zero_loop_end

zero_loop:
  sw x0, 0(x26)
  addi x26, x26, 4
  ble x26, x27, zero_loop
zero_loop_end:

  /* jump to main program entry point (argc = argv = 0) */
  addi x10, x0, 0
  addi x11, x0, 0
  jal x1, main

  /* if we return from main, call exit() */
  tail _cva6_exit

/* Trap vector entry for CVA6.
 * Keep this tiny and always tail-call into C to print/terminate.
 */
_cva6_trap_entry:
  j _cva6_exc_handler

