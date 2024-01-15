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

#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

inline void set_mstatus_bit(unsigned long mask)
{
#if __riscv_xlen == 64
        unsigned long long status;
        __asm__ volatile ("csrr %0, mstatus" : "=r" (status));
        status |= mask;
        __asm__ volatile ("csrw mstatus, %0" :: "r" (status));

#elif __riscv_xlen == 32

        unsigned long status;
        __asm__ volatile ("csrr %0, mstatus" : "=r" (status));
        status |= mask;
        __asm__ volatile ("csrw mstatus, %0" :: "r" (status));
#endif
}

inline unsigned int get_mie(void)
{
        unsigned int vec;
        __asm__ volatile ("csrr %0, mie" : "=r" (vec));
        return vec;
}

inline void set_mie(unsigned int vec)
{
        __asm__ volatile ("csrw mie, %0" :: "r" (vec));
}

inline unsigned int get_hartid(void)
{
        unsigned int id;
        __asm__ volatile ("csrr %0, mhartid" : "=r" (id));
        return id;
}

unsigned long long get_mtime(void)
{
        return *(unsigned long long *)0x200bff8;
}

void set_mtimecmp(unsigned long long timecmp)
{
        unsigned long long  * ptr
                = (unsigned long long *)0x2004000;
        ptr[get_hartid()] = timecmp;
}

void init_timer(void)
{
        set_mtimecmp(-1LL);
        set_mie(get_mie() | 0x80);
        set_mstatus_bit(0x8);
}

void wait_a_while(unsigned long wait)
{
        unsigned long long when = get_mtime() + wait;
        set_mtimecmp(when);

        while (get_mtime() < when) {
                __asm__ volatile ("wfi");
        }
}

#endif /* TIMER_H_INCLUDED */
