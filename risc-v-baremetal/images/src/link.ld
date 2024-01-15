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

OUTPUT_ARCH("riscv")

ENTRY(_start)

MEMORY
{
	ROM : ORIGIN = 0x80000000, LENGTH = 256K
	RAM : ORIGIN = 0x80080000, LENGTH = 256K
}

SECTIONS
{
		 .text : { *(.text) } > ROM
		 . = ALIGN(0x1000);
		 __global_pointer$ = . + 0x800;
		 .rodata : { *(.rodata) } > ROM

         _sidata = LOADADDR(.data);

		 . = ALIGN(8);
		 .data :
		 {
		 	_data_start = .;
			*(.data)
			. = ALIGN(4);
			_data_end = .;
		 } > RAM AT > ROM

		 _data_size = SIZEOF(.data);

		 .bss :
		 { 	
			__bss_start = .;
			*(.bss)
			*(COMMON)
			. = ALIGN(4);
			__bss_end = .; 
		} > RAM
		 . += 0x4000;
		 . = ALIGN(16);
		 __stack_top = .;
}
