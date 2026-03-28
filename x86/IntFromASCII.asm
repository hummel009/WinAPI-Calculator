proc IntFromASCII string, radix
	push ebx ecx edx esi edi
	xor ebx, ebx
	mov ecx, [radix]
	mov esi, [string]
	mov edi, 1
	dec [string]
	cld
	cmp ecx, 16
	ja .error
@@:
	lodsb
	test al, al
	jnz @B
	sub esi, 2
	std

; loop
.loop:
	xor eax, eax
	lodsb
	sub al, '0'
	cmp al, 9
	jbe @F

	sub al, 'A' - '9' - 1
@@:
	cmp al, 0Fh
	ja .error

	imul edi
	imul edi, ecx
	add ebx, eax
	cmp edi, 10000000h
	ja .error

	cmp esi, [string]
	jnz .loop
; end loop

	mov esi, [string]
	cmp byte [esi], '-'
	jnz @F

	neg eax
@@:
	mov eax, ebx
	stc

.theend:
	cld
	pop edi esi edx ecx ebx

	ret

.error:
	clc
	jmp .theend
endp