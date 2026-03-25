proc FloatFromASCII string
	push eax ecx esi
	fninit
	fild [FloatFromASCII.ten]
	fldz
	fldz
	mov esi, [string]
	cmp byte [esi], '-'
	jnz @F
	inc esi
@@:
	xor eax, eax
	align 4

; loop
.loop.integer_part:
	lodsb
	cmp al, '.'
	jz .mantisa
	test al, al
	jz .exit
	fmul st0, st2
	sub al, '0'
	cmp al, 9
	jbe @F
	sub al, 'A' - '9' - 1
@@:
	push eax
	fiadd dword [esp]
	add esp, 4
	jmp .loop.integer_part
; end loop

.mantisa:
	xor ecx, ecx
@@:
	inc ecx
	lodsb
	test al, al
	jnz @B
	cmp ecx, 10
	jbe @F
	mov ecx, 10
@@:
	std
	dec esi
	fxch st1
	align 4

; loop
.loop.mantisa_part:
	lodsb
	cmp al, '.'
	jz .exit
	sub al, '0'
	cmp al, 9
	jbe @F
	sub al, 'A' - '9' - 1
@@:
	push eax
	fiadd dword [esp]
	add esp, 4
	fdiv st0, st2
jmp .loop.mantisa_part
; end loop

.exit:
	cld
	faddp st1, st0
	ffree st1
	mov eax, [string]
	cmp byte [eax], '-'
	jnz @F
	fchs
@@:
	stc
	pop esi ecx eax

	ret
endp