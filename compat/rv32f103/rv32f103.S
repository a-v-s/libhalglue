.equ GD32_MARCHID,    0x80000022		# Identifiers for GD32VF103
.equ GD32_MVENDORID,  0x0000031E		# Identifiers for GD32VF103

.equ CH32V0_MARCHID,    0xdc68d841		# Identifiers for CH32V003
.equ CH32V0_MVENDORID,  0x00000000		# Identifiers for CH32V003

.equ CH32V1_MARCHID,    0x00000000		# Identifiers for CH32V103
.equ CH32V1_MVENDORID,  0x01020304		# Identifiers for CH32V103

.equ CH32V2_MARCHID,    0xdc68d882
.equ CH32V2_MVENDORID,  0x00000000

.equ CH32V3_MARCHID,    0xdc68d881
.equ CH32V3_MVENDORID,  0x00000000



.extern _init_gd32
.extern _init_ch32

.section	.text,"ax",@progbits
#.globl _start
#.type _start,@function

.global Reset_Handler
.type Reset_Handler,@function


/*	We are running on GD32VF103 or CH32V103. 
 *  These designs alias either flash or ram to 0x0 depending on the
 *  BOOT pins. This start routine will detect the boot mode and jump
 *  to the real address space rather then the aliassed address space at 0x0
 */

/*  It seems the CH32V103 implementation required the use of 0x0 as the
 *  base rather then the real 0x8000000 address. Fortunately the GD32VF103
 *  allows flashing with the aliassed address.
 */

 /*
handle_reset:
	//csrc CSR_MSTATUS, MSTATUS_MIE 	# Disable interrupts

    la		a0,	handle_reset				# a0 = address of _start
    li		a1,	1					# a1 = 1
	slli	a1,	a1, 29				# a1 <<= 29
									# a1 is now 0x20000000
									# This is where RAM begins
									# We will jump to __start if we are
    bleu	a1, a0, _start0800		# booting from RAM
    srli	a1,	a1, 2				# Othwerwise, we set a1 to 0x8000000
    bleu	a1, a0, _start0800      # And jump if we are booting from flash
    la		a0,	_start0800			# Otherwise, we've booted from 0x0
    add		a0, a0, a1				# then we add 0x8000000 to _start0800
	jr      a0						# and jump there

//---------------------------------------------------------------------------
_start0800:
									# Jump here at the real address
*/

Reset_Handler:



.option push
.option	norelax
	csrs mstatus, 0
	la gp, __global_pointer$
    la tp, __tls_base
.option	pop
1:
	la sp, _estack
2:
	/* Load data section from flash to RAM */
	la a0, _data_lma
	la a1, _sdata
	la a2, _edata
	bgeu a1, a2, 2f
1:
	lw t0, (a0)
	sw t0, (a1)
	addi a0, a0, 4
	addi a1, a1, 4
	bltu a1, a2, 1b
2:
	/* clear bss section */
	la a0, _sbss
	la a1, _ebss
	bgeu a0, a1, 2f
1:
	sw zero, (a0)
	addi a0, a0, 4
	bltu a0, a1, 1b
2:
	/* Call global constructors */
	call __libc_init_array


identify_chip:
	csrr    a0, marchid				
	csrr    a1, mvendorid

try_gd32:							# Check if we are runnng on a GD32VF103
	li      a2, GD32_MARCHID		# If the marchid != GD32_MARCHID
	bne     a0, a2, try_ch32v1		# We go check if it is a CH32V103
	li		a2, GD32_MVENDORID
	bne     a1, a2, chip_not_supported
	j	_init_gd32					# GD32VF103 identified,

try_ch32v1:							# Check if we are runnng on a CH32V1xx
	li      a2, CH32V1_MARCHID
	bne     a0, a2, try_ch32v2
	li		a2, CH32V1_MVENDORID
	bne     a1, a2, chip_not_supported
	j	_init_ch32					# CH32VF103 identified,

try_ch32v2:							# Check if we are runnng on a CH32V2xx
	li      a2, CH32V2_MARCHID
	bne     a0, a2, try_ch32v3
	li		a2, CH32V2_MVENDORID
	bne     a1, a2, chip_not_supported
	j	_init_ch32					# CH32VF103 identified,

try_ch32v3:							# Check if we are runnng on a CH32V3xx
	li      a2, CH32V3_MARCHID
	bne     a0, a2, try_ch32v0
	li		a2, CH32V3_MVENDORID
	bne     a1, a2, chip_not_supported
	j	_init_ch32

try_ch32v0:							# Check if we are runnng on a CH32V0xx
	li      a2, CH32V0_MARCHID
	bne     a0, a2, chip_not_supported
	li		a2, CH32V0_MVENDORID
	bne     a1, a2, chip_not_supported
	j	_init_ch32

chip_not_supported:
	j chip_not_supported			# Endless loop for unknown chip

.global _init
_init:
	ret






