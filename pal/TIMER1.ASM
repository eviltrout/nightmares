;Assembly language functions extracted from TIMER.C
;and recoded in *real* assembly language.
;(Too many incompatibilities in all the different
;implementations of inline assembly language ;-)
;
;David Kirschbaum
;Toad Hall

	.MODEL	small
	.CODE
	PUBLIC	_initializetimer,_restoretimer,_readtimer

;void initializetimer(void)
_initializetimer	PROC	NEAR
;Old logic (very redundant):
;	mov	dx,43H		;/* outp(0x043,0x034); */
;	mov	al,34H
;	out	dx,al
;	jmp	short $+2
;
;	mov	dx,40H		;/* outp(0x040,0x000); */
;	xor	ax,ax
;	out	dx,al
;	jmp	short $+2
;
;	out	dx,al		;/* outp(0x040,0x000); */
;	ret

	mov	al,34H
	jmp	short Common	;common to return

_initializetimer	ENDP


;void restoretimer(void)

_restoretimer	PROC	NEAR
	mov	al,36H		;/* outp(0x043,0x036); */
Common:
	mov	dx,43H
	out	dx,al
	jmp	short $+2

	mov	dx,40H		;/* outp(0x040,0x000); */
	xor	ax,ax
	out	dx,al
	jmp	short $+2

	out	dx,al		;/* outp(0x040,0x000); */
	ret
_restoretimer	ENDP

;long readtimer(void)
_readtimer	PROC	NEAR
	push	si		;save these just in case
	push	di

	cli			;/* Disable interrupts */
	mov	dx,020h		;/* Address PIC ocw3   */
	mov	al,00Ah		;/* Ask to read irr    */
	out	dx,al
	mov	al,00h		;/* Latch timer 0 */
	out	043h,al
	in	al,dx		;/* Read irr      */
	mov	di,ax		;/* Save it in DI */
	in	al,040h		;/* Counter --> bx*/
	mov	bl,al		;/* LSB in BL     */
	in	al,040h
	mov	bh,al		;/* MSB in BH     */
	not	bx		;/* Need ascending counter */
	in	al,021h		;/* Read PIC imr  */
	mov	si,ax		;/* Save it in SI */

	mov	al,00FFh	;/* Mask all interrupts */
	out	021h,al
	mov	ax,040h		;/* read low word of time */
	mov	es,ax		;/* from BIOS data area   */
	mov	dx,es:[06Ch]	;here's the timer long.hi

	mov	ax,si		;/* Restore imr from SI   */
	out	021h,al

	sti			;/* Enable interrupts */
	mov	ax,di		;/* Retrieve old irr  */
	test	al,001h		;/* Counter hit 0?    */
	jz	Done		;/* Jump if not       */
	cmp	bx,0FFh		;/* Counter > 0x0FF?    */
	ja	Done		;/* Done if so        */
	 inc	dx		;/* Else count int req. */
Done:	mov	ax,bx		;/* set function result */

	pop	di		;restore
	pop	si
	ret
_readtimer	ENDP

	END
