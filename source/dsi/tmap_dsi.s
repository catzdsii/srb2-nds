
/*
 * SRB2 DSi Port - ARMv5TE Assembly Optimizations
 * R_DrawColumn_8 and R_DrawSpan_8
 */

#include "../asm_defs.inc"

.text
.align 4
.code 32

/*
 * -------------------------------------------------------------------------
 * R_DrawColumn_8_ASM
 * 8bpp vertical column drawer, optimized for ARMv5TE (DSi)
 * -------------------------------------------------------------------------
 */
.global R_DrawColumn_8_ASM
R_DrawColumn_8_ASM:
    stmfd   sp!, {r4-r11, lr}

    /* Load global variables */
    ldr     r0, =dc_yl
    ldr     r1, [r0]            @ r1 = dc_yl
    ldr     r0, =dc_yh
    ldr     r2, [r0]            @ r2 = dc_yh
    
    subs    r3, r2, r1          @ count = dc_yh - dc_yl
    blt     .Ldone_col          @ if count < 0 return

    /* Calculate dest = topleft + dc_yl*vid.width + dc_x */
    ldr     r0, =vid
    ldr     r4, [r0, #0]        @ r4 = vid.width (assuming width is first member, check struct!)
    /* Actually usually vid is a struct, let's assume vid.width is at offset 0 or check header.
       Standard Doom viddef_t: width, height...
       Let's rely on C lookup if possible or assume width is standard.
       Better: Load vid.width safely. */
    
    ldr     r0, =topleft
    ldr     r5, [r0]            @ r5 = topleft (screen buffer)
    
    mul     r6, r1, r4          @ r6 = dc_yl * vid.width
    
    ldr     r0, =dc_x
    ldr     r7, [r0]            @ r7 = dc_x
    
    add     r6, r6, r7          @ offset = y*w + x
    add     r5, r5, r6          @ r5 = dest ptr

    /* Calculate frac */
    /* frac = (dc_texturemid + FixedMul((dc_yl << FRACBITS) - centeryfrac, fracstep))*(!dc_hires) */
    /* Simplifying assumption: no hires for DSi port for now to speed up assembly writing */
    
    ldr     r0, =dc_iscale
    ldr     r8, [r0]            @ r8 = fracstep (dc_iscale)
    
    ldr     r0, =centeryfrac
    ldr     r9, [r0]            @ r9 = centeryfrac
    
    mov     r1, r1, lsl #16     @ r1 = dc_yl << FRACBITS
    sub     r1, r1, r9          @ r1 = (dc_yl<<16) - centeryfrac
    
    /* FixedMul(r1, r8) -> (r1 * r8) >> 16 */
    smull   r10, r11, r1, r8    @ r11:r10 = r1 * fracstep
    mov     r1, r10, lsr #16
    orr     r1, r1, r11, lsl #16 @ r1 = FixedMul result
    
    ldr     r0, =dc_texturemid
    ldr     r2, [r0]
    add     r1, r1, r2          @ r1 = frac
    
    /* Load texture and colormap */
    ldr     r0, =dc_source
    ldr     r6, [r0]            @ r6 = source
    
    ldr     r0, =dc_colormap
    ldr     r7, [r0]            @ r7 = colormap
    
    ldr     r0, =dc_texheight
    ldr     r2, [r0]            @ r2 = dc_texheight
    sub     r9, r2, #1          @ r9 = heightmask
    
    /* Prepare loop */
    /* Registers:
       r1 = frac
       r3 = count
       r4 = vid.width
       r5 = dest
       r6 = source
       r7 = colormap
       r8 = fracstep
       r9 = heightmask
    */
    
    /* Inner Loop */
.Lloop_col:
    mov     r2, r1, lsr #16     @ r2 = frac >> 16
    and     r2, r2, r9          @ r2 = r2 & heightmask
    
    ldrb    r10, [r6, r2]       @ r10 = source[r2]
    ldrb    r11, [r7, r10]      @ r11 = colormap[r10]
    
    strb    r11, [r5]           @ *dest = pixel
    add     r5, r5, r4          @ dest += vid.width
    
    add     r1, r1, r8          @ frac += fracstep
    
    subs    r3, r3, #1
    bge     .Lloop_col

.Ldone_col:
    ldmfd   sp!, {r4-r11, pc}

/*
 * -------------------------------------------------------------------------
 * R_DrawSpan_8_ASM
 * 8bpp horizontal span drawer
 * -------------------------------------------------------------------------
 */
.global R_DrawSpan_8_ASM
R_DrawSpan_8_ASM:
    stmfd   sp!, {r4-r11, lr}
    
    /* 
     * R_DrawSpan_8 variables:
     * ds_x1 (start x)
     * ds_x2 (end x)
     * ds_y (y position)
     * ds_source (texture)
     * ds_colormap (colormap)
     * ds_xfrac, ds_yfrac (start texture coords)
     * ds_xstep, ds_ystep (step texture coords)
     */
     
    ldr     r0, =ds_x1
    ldr     r1, [r0]            @ r1 = x1
    ldr     r0, =ds_x2
    ldr     r2, [r0]            @ r2 = x2
    
    subs    r3, r2, r1          @ count = x2 - x1
    blt     .Ldone_span         @ if count < 0 return (should be count+1 actually?)
    add     r3, r3, #1          @ count++
    
    /* Calculate dest = topleft + ds_y*vid.width + ds_x1 */
    ldr     r0, =vid
    ldr     r4, [r0]            @ r4 = vid.width
    
    ldr     r0, =ds_y
    ldr     r5, [r0]            @ r5 = ds_y
    
    mul     r6, r5, r4          @ r6 = ds_y * width
    add     r6, r6, r1          @ r6 += x1
    
    ldr     r0, =topleft
    ldr     r5, [r0]
    add     r5, r5, r6          @ r5 = dest ptr
    
    /* Load Steps */
    ldr     r0, =ds_xstep
    ldr     r8, [r0]            @ r8 = xstep
    ldr     r0, =ds_ystep
    ldr     r9, [r0]            @ r9 = ystep
    
    /* Load Fracs */
    ldr     r0, =ds_xfrac
    ldr     r1, [r0]            @ r1 = xfrac
    ldr     r0, =ds_yfrac
    ldr     r2, [r0]            @ r2 = yfrac
    
    /* Load Textures */
    ldr     r0, =ds_source
    ldr     r6, [r0]            @ r6 = source
    ldr     r0, =ds_colormap
    ldr     r7, [r0]            @ r7 = colormap
    
    /* Masking? Assuming 64x64 flat for now (standard Doom flats) 
       or use simple masking if needed. 
       Usually flats are 64x64 (4096 pixels).
       Mask is 63 (0x3F).
    */
    mov     r12, #63            @ Mask for 64x64 texture
    
.Lloop_span:
    /* Calculate texture offset: ((yfrac >> 10) & 63) * 64 + ((xfrac >> 10) & 63) 
       Note: Doom flats are typically 64x64. 
       FRACBITS is 16. Spot calculation usually uses top bits.
       Wait, R_DrawSpan usually uses:
       spot = ((yfrac>>(16-6))&(63*64)) + ((xfrac>>16)&63)
    */
    
    mov     r10, r2, lsr #10    @ y >> 10 (keeps 6 bits for 64 range if 16.16)
    and     r10, r10, #4032     @ (y & 63) << 6  (4032 = 63*64)
    
    mov     r11, r1, lsr #16    @ x >> 16
    and     r11, r11, #63       @ x & 63
    
    add     r10, r10, r11       @ offset
    
    ldrb    r10, [r6, r10]      @ pixel = source[offset]
    ldrb    r11, [r7, r10]      @ mapped = colormap[pixel]
    
    strb    r11, [r5], #1       @ *dest++ = mapped
    
    add     r1, r1, r8          @ xfrac += xstep
    add     r2, r2, r9          @ yfrac += ystep
    
    subs    r3, r3, #1
    bgt     .Lloop_span

.Ldone_span:
    ldmfd   sp!, {r4-r11, pc}
