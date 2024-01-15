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

#include "uart.h"
#include "timer.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#define MAX_SNAKE_LENGTH 128
#define BOARD_SIZE_X 79
#define BOARD_SIZE_Y 24

typedef unsigned short pos_t;

typedef struct snake
{
        unsigned char ok;
        unsigned char direction;
        unsigned char length;
        pos_t head;
        pos_t body[MAX_SNAKE_LENGTH];
} snake_t;

void output_num(unsigned int num)
{
        unsigned char buf[32];
        int index = 0;
        buf[index] = '0';

        while (num)
        {
                buf[index] = '0' + num % 10;
                num = num / 10;
                index += 1;
        }

        do
        {
                index -= 1;
                uart_putc(buf[index]);
        } while (index > 0);
}

void output_pos(pos_t pos)
{
        unsigned char x = pos & 0xFF;
        unsigned char y = (pos >> 8) & 0xFF;
        uart_putc(033);
        uart_putc('[');
        output_num(y);
        uart_putc(';');
        output_num(x);
        uart_putc('H');
}

void output_head(pos_t pos)
{
        output_pos(pos);
        uart_putc('@');
}

void output_body(pos_t pos)
{
        output_pos(pos);
        uart_putc('O');
}

void output_none(pos_t pos)
{
        output_pos(pos);
        uart_putc(' ');
}

void output_whole_snake(snake_t *snake)
{
        output_head(snake->head);
        for (int i = 0; i < snake->length; ++i)
        {
                output_body(snake->body[i]);
        }
}

pos_t next_step(pos_t pos, unsigned char direction)
{
        unsigned char x = pos & 0xFF;
        unsigned char y = (pos >> 8) & 0xFF;
        switch (direction)
        {
        case 0:
                x -= 1;
                break;
        case 1:
                y -= 1;
                break;
        case 2:
                x += 1;
                break;
        case 3:
                y += 1;
                break;
        }
        return (y << 8 | x);
}

int check_collition(const snake_t *snake, pos_t head)
{
        int x = head & 0xff;
        int y = head >> 8;

        if (x == 1 || x == BOARD_SIZE_X)
                return 1;
        if (y == 1 || y == BOARD_SIZE_Y)
                return 1;

        for (int i = 0; i < snake->length; ++i)
                if (head == snake->body[i])
                        return 1;
        return 0;
}

void advance_snake(snake_t *snake, int grow)
{
        pos_t new_head = next_step(snake->head, snake->direction);
        if (check_collition(snake, new_head))
        {
                snake->ok = 0;
        }
        else
        {
                output_head(new_head);
                output_body(snake->head);
                if (grow && snake->length < MAX_SNAKE_LENGTH)
                {
                        snake->length++;
                }
                else
                {
                        output_none(snake->body[snake->length]);
                }
                for (int i = snake->length; i > 0; --i)
                {
                        snake->body[i] = snake->body[i - 1];
                }
                snake->body[0] = snake->head;
                snake->head = new_head;
        }
}

static void
draw_horizontal_line(unsigned short line)
{
        output_pos(line << 8);
        uart_putc('+');
        for (int x = 2; x < BOARD_SIZE_X; ++x)
                uart_putc('-');
        uart_putc('+');
}

void draw_frame(void)
{
        uart_put_str("\033[2J");

        draw_horizontal_line(0);
        draw_horizontal_line(BOARD_SIZE_Y);
        for (short i = 2; i < BOARD_SIZE_Y; ++i)
        {
                output_pos(i << 8);
                uart_putc('|');
                output_pos(i << 8 | BOARD_SIZE_X);
                uart_putc('|');
        }
}

int run_a_game(void)
{
        snake_t s = {1, 0, 7, 0x0818, {0x0819, 0x081a, 0x081b, 0x071b, 0x061b, 0x051b, 0x041b, 0x41c}};
        int steps = 0;

        output_whole_snake(&s);

        while (s.ok)
        {
                wait_a_while((12000 - (steps * 10)) * 8);
                while (uart_is_available_char())
                {
                        int c = uart_getc();
                        if (c == 'x')
                        {
                                s.direction = (s.direction + 3) & 0x3;
                        }
                        else if (c == 'm')
                        {
                                s.direction = (s.direction + 1) & 0x3;
                        }
                }
                steps += 1;
                advance_snake(&s, (steps & 0xf) == 0);
        }
        return steps;
}

int main()
{
        init_timer();
        uart_init();

        draw_frame();

        output_pos(0x0520);
        uart_put_str(" S N A K E ");

        for (;;)
        {
                output_pos(0x0918);
                uart_put_str(" Control snake with 'x' and 'm' ");

                output_pos(0x0b18);
                uart_put_str(" Start new game with 's'  ");

                int c = 0;
                while (c != 's')
                {
                        while (!uart_is_available_char())
                                wait_a_while(10000);
                        c = uart_getc();
                }

                draw_frame();
                int steps = run_a_game();

                output_pos(0x0520);
                uart_put_str(" GAME OVER  ");

                output_pos(0x071f);
                uart_put_str(" SCORE: ");
                output_num(steps * 10);
                uart_putc(' ');
                int *cx = malloc(4*sizeof(int));
                *cx = 841;
                // int cx = 841;
                output_num(cx);
                uart_putc(' ');
                // }
        }

        uart_put_str(" S N A K E ......");

        return 0;
}
