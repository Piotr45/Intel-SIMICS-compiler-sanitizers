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

void * memset ( void * ptr, int value, long num )
{
        for (unsigned char *d = ptr;
             d < (unsigned char *)(ptr + num);
             *d++ = (unsigned char)value);
        return ptr;
}

void * memcpy ( void * ptr, const void *src, long num )
{
        char *d = (char *)ptr;
        const char *s = (const char *)src;
        for (unsigned i = 0; i < num; ++i)
                *d++ = *s++;
        return ptr;
}

struct ns16550_regs {
        union {
                const volatile unsigned char rbr;
                volatile unsigned char thr;
        };
        volatile unsigned char ier;
        union {
                const volatile unsigned char iir;
                volatile unsigned char fcr;
        };

        volatile unsigned char lcr;
        volatile unsigned char mcr;
        const volatile unsigned char lsr;
        const volatile unsigned char msr;
        volatile unsigned char scr;
};

static struct ns16550_regs * const uart = (struct ns16550_regs *)0x10000000;

void uart_init(void)
{
        uart->ier = 0;
        uart->lcr = 0x80; /* DLAB mode */
        uart->lcr = 3; /* Leave DLAB-mode, 8n1 */
        uart->fcr = 1; /* enable FIFOs */
        uart->mcr = 0;
        (void)uart->rbr;
}

void uart_putc(unsigned char ch)
{
        while(!(uart->lsr & 0x20));
        uart->thr = ch;
}

int uart_is_available_char(void)
{
        return (uart->lsr & 0x01);
}

int uart_getc(void)
{
        return uart->rbr;
}

void uart_put_str(const char *str)
{
        while(*str) uart_putc(*str++);
}


