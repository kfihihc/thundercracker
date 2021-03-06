/* -*- mode: C; c-basic-offset: 4; intent-tabs-mode: nil -*-
 *
 * Thundercracker cube firmware
 *
 * Micah Elizabeth Scott <micah@misc.name>
 * Copyright <c> 2011 Sifteo, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/*
 * Additional definitions specific to BG0, shared between BG0-derived modes.
 */
 
#ifndef _GRAPHICS_BG0_H
#define _GRAPHICS_BG0_H

#include "graphics.h"
#include "sensors.h"

/*
 * Shared state
 */

__sfr __at 0xDD x_bg0_first_w;          // Width of first displayed background tile, [1, 8]
__sfr __at 0xDE x_bg0_last_w;           // Width of last displayed background tile, [0, 7]
__sfr __at 0xC9 x_bg0_wrap;             // Load value for a dec counter to the next X map wraparound
extern uint8_t x_bg0_first_addr;        // Low address offset for first displayed tile

__sfr __at 0xA2 y_bg0_addr_l;           // Low part of tile addresses, inc by 32 each line
extern uint16_t y_bg0_map;              // Map address for the first tile on this line

/*
 * Shared code
 */
 
void vm_bg0_line(void);
void vm_bg0_setup(void);
void vm_bg0_next(void);
void vm_bg0_x_wrap_adjust(void) __naked;

/*
 * Macros
 */

// Load a 16-bit tile address from DPTR, and auto-increment
#pragma sdcc_hash +
#define ADDR_FROM_DPTR_INC() {                                  \
    __asm movx  a, @dptr                                        __endasm; \
    __asm mov   ADDR_PORT, a                                    __endasm; \
    __asm inc   dptr                                            __endasm; \
    __asm mov   CTRL_PORT, #CTRL_FLASH_OUT | CTRL_FLASH_LAT1    __endasm; \
    __asm movx  a, @dptr                                        __endasm; \
    __asm mov   ADDR_PORT, a                                    __endasm; \
    __asm inc   dptr                                            __endasm; \
    __asm mov   CTRL_PORT, #CTRL_FLASH_OUT | CTRL_FLASH_LAT2    __endasm; \
    }

// Add 2 to DPTR. (Can do this in 2 clocks with inline assembly)
#define DPTR_INC2() {                                           \
    __asm inc   dptr                                            __endasm; \
    __asm inc   dptr                                            __endasm; \
    }

// Called once per tile, to check for horizontal map wrapping
#define BG0_WRAP_CHECK() {                              \
        if (!--bg0_wrap)                                \
            DPTR -= _SYS_VRAM_BG0_WIDTH *2;             \
    }

#define ASM_X_WRAP_CHECK(lbl)                                   __endasm; \
    __asm djnz  r1, lbl                                         __endasm; \
    __asm lcall _vm_bg0_x_wrap_adjust                           __endasm; \
    __asm lbl:                                                  __endasm; \
    __asm

#define ASM_X_WRAP_CHECK_A(lbl)                                 __endasm; \
    __asm djnz  r1, lbl                                         __endasm; \
    __asm acall _vm_bg0_x_wrap_adjust                           __endasm; \
    __asm lbl:                                                  __endasm; \
    __asm

// Assembly macro wrappers
#define ASM_ADDR_INC4()                 __endasm; ADDR_INC4(); __asm
#define ASM_DPTR_INC2()                 __endasm; DPTR_INC2(); __asm
#define ASM_ADDR_FROM_DPTR_INC()        __endasm; ADDR_FROM_DPTR_INC(); __asm

#endif
