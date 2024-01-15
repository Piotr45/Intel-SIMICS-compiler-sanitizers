/*
  © 2023 Intel Corporation

  This software and the related documents are Intel copyrighted materials, and
  your use of them is governed by the express license under which they were
  provided to you ("License"). Unless the License provides otherwise, you may
  not use, modify, copy, publish, distribute, disclose or transmit this software
  or the related documents without Intel's prior written permission.

  This software and the related documents are provided as is, with no express or
  implied warranties, other than those that are expressly stated in the License.
*/

        .file	"start.s"

        .extern main
        .extern __stack_top
        .extern memcpy
        .extern memset
        .text
        .align  2
        .global _start
        .type   _start, @function
_start:
        .option push
        .option norelax
        la      gp, __global_pointer$
        .option pop

        csrr   t0, mhartid
        bnez   t0, _secondary_core

        la     sp, __stack_top

        la     a0, _sidata
        la     a1, _data_start
        la     a2, _data_size

        jal    x1, memcpy

        la     a0, _zero_start
        mv     a1, zero
        sub    a2, sp, a0

        jal    x1, memset

        la     t0, _mtvec
        csrw   mtvec, t0

        jal    x1, main

        j      .


        .align 4
_mtvec:
        csrrw t0, mscratch, t0
        csrr  t0, mcause
        blt   t0, zero, _irq

        /* unexpected trap */
        j     .

_irq:
        slli  t0, t0, 1
        srli  t0, t0, 1

        add   t0, t0, -7
        beqz  t0, _mtip

        /* ignore other irqs for now */

_trap_done:
        csrrw t0, mscratch, t0
        mret

_mtip:
        la    t0, 0x2004000

#if __riscv_xlen == 64
        add   sp, sp, -16
        sd    t1, 0(sp)
        sd    t2, 8(sp)

#elif __riscv_xlen == 32
        add   sp, sp, -8
        sw    t1, 0(sp)
        sw    t2, 4(sp)

#endif

        csrr  t1, mhartid
        slli  t1, t1, 8
        add   t0, t0, t1

        li    t2, -1

#if __riscv_xlen == 64

        sd    t2, 0(t0)

        ld    t1, 0(sp)
        ld    t2, 8(sp)
        add   sp, sp, 16

#elif __riscv_xlen == 32

        sw    t2, 0(t0)
        sw    t2, 4(t0)

        lw    t1, 0(sp)
        lw    t2, 4(sp)
        add   sp, sp, 8

#endif

        j     _trap_done

_secondary_core:
        wfi
        j     _secondary_core

        .size _start, .-_start

