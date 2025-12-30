	idnt	"prototest.c"
	opt	0
	opt	NQLPSMRBT
	section	"CODE",code
	public	_main
	cnop	0,4
_main
	movem.l	l7,-(a7)
	moveq	#0,d0
	lea	l5,a1
	move.l	_SysBase,a6
	jsr	-552(a6)
	move.l	d0,a0
	move.l	a0,_DOSBase
	beq	l4
	moveq	#14,d3
	move.l	#l6,d2
	move.l	_DOSBase,a6
	jsr	-60(a6)
	move.l	d0,d1
	move.l	_DOSBase,a6
	jsr	-48(a6)
	move.l	_DOSBase,a1
	move.l	_SysBase,a6
	jsr	-414(a6)
l4
	moveq	#0,d0
l1
l7	reg	a6/d2/d3
	movem.l	(a7)+,a6/d2/d3
l9	equ	12
	rts
	cnop	0,4
l6
	dc.b	72
	dc.b	101
	dc.b	108
	dc.b	108
	dc.b	111
	dc.b	32
	dc.b	119
	dc.b	111
	dc.b	114
	dc.b	108
	dc.b	100
	dc.b	33
	dc.b	10
	dc.b	0
	cnop	0,4
l5
	dc.b	100
	dc.b	111
	dc.b	115
	dc.b	46
	dc.b	108
	dc.b	105
	dc.b	98
	dc.b	114
	dc.b	97
	dc.b	114
	dc.b	121
	dc.b	0
	public	_SysBase
	public	_DOSBase
	section	"BSS",bss
	cnop	0,4
_DOSBase
	ds.b	4
