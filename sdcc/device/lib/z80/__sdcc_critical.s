;--------------------------------------------------------------------------
;  __sdcc_critical.s
;
;  Copyright (C) 2020, Sergey Belyashov
;
;  This library is free software; you can redistribute it and/or modify it
;  under the terms of the GNU General Public License as published by the
;  Free Software Foundation; either version 2, or (at your option) any
;  later version.
;
;  This library is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
;  GNU General Public License for more details.
;
;  You should have received a copy of the GNU General Public License 
;  along with this library; see the file COPYING. If not, write to the
;  Free Software Foundation, 51 Franklin Street, Fifth Floor, Boston,
;   MA 02110-1301, USA.
;
;  As a special exception, if you link this library with other files,
;  some of which are compiled with SDCC, to produce an executable,
;  this library does not by itself cause the resulting executable to
;  be covered by the GNU General Public License. This exception does
;  not however invalidate any other reasons why the executable file
;   might be covered by the GNU General Public License.
;--------------------------------------------------------------------------

	.area   _CODE

	.globl ___sdcc_critical_enter
;
; NMOS Z80 compatible
; this function cannot be placed at 0x0000...0x00ff addresses
;
___sdcc_critical_enter::
	xor	a, a
	push	af
	pop	af
	ld	a, i
	di
	ret	pe	;enabled interrupts
	dec	sp
	dec	sp
	pop	af
	or	a, a	;A = 0 if interrupts disabled
	jr	NZ, 00100$
;interrupts disabled
	sub	a, a	;force P/V = 0
	ret
;interrupts enabled
00100$:
	xor	a, a	;force P/V = 1
	ret
