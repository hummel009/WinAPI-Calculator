proc FloatToASCII buffer, precision
locals
	status_original dw ?
	status_changed dw ?
	integer dd ?
	mantisa dd ?
	signed dd ?
endl
	push eax edi ecx
	mov eax, [precision]
	cmp eax, 51
	jb @F

	mov eax, 51
@@:
	mov [precision], eax
	fnstcw [status_original]
	mov ax, [status_original]
	or ax, 0000110000000000b
	mov [status_changed], ax
	fldcw [status_changed]
	xor eax, eax
	fst [signed]
	test [signed], 80000000h
	setnz al
	mov [signed], eax
	fld st0
	fld st0
	frndint
	fist [integer]
	fabs
	fsubp st1, st0
	mov edi, [buffer]
	push [signed]
	push edi
	push 10
	push [integer]
	call IntToASCII
	add edi, eax
	mov al, '.'
	stosb
	mov ecx, [precision]
	dec ecx

; loop
.loop:
	fimul [FloatToASCII.ten]
	fld st0
	frndint
	fist [mantisa]
	fsubp st1, st0
	push 0
	push edi
	push 10
	push [mantisa]
	call IntToASCII
	add edi, eax
	ftst
	fnstsw ax
	test ax, 0100000000000000b
	jz @F

	test ax, 0000010100000000b
	jz .finish
@@:
	loop .loop
; end loop

	fldcw [status_original]
	fimul [FloatToASCII.ten]
	fist [mantisa]
	push 0
	push edi
	push 10
	push [mantisa]
	call IntToASCII

.finish:
	fstp st0
	stc
	pop ecx edi eax

	ret
endp