; Copyright (c) 2024 Romain BAILLY
;
; Permission is hereby granted, free of charge, to any person obtaining a copy of
; this software and associated documentation files (the "Software"), to deal in
; the Software without restriction, including without limitation the rights to
; use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
; of the Software, and to permit persons to whom the Software is furnished to do
; so, subject to the following conditions:
;
; The above copyright notice and this permission notice shall be included in all
; copies or substantial portions of the Software.
;
; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
; IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
; FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
; AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
; LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
; OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
; SOFTWARE.


SECTION .text


;==================================================================================================


; rcx: count
; rdx: data
; r8:  histo
GLOBAL rmgr_histo_x86
rmgr_histo_x86:
	push  rbx
	push  rsi
	push  rdi
	mov   r9,  rcx
	mov   r10, rdx
histo_loop:
	mov   rax, [r10]
	mov   rbx, [r10+8]
	mov   rcx, [r10+16]
	mov   rdx, [r10+24]
	add   r10, 32
	movzx esi, al
	movzx edi, bl
	inc   dword[r8 + 4*rsi]
	inc   dword[r8 + 4*rdi + 1024]
	movzx esi, ah
	movzx edi, bh
	inc   dword[r8 + 4*rsi + 2048]
	inc   dword[r8 + 4*rdi + 3072]
	shr   rax, 16
	shr   rbx, 16
	movzx esi, cl
	movzx edi, dl
	inc   dword[r8 + 4*rsi + 4096]
	inc   dword[r8 + 4*rdi + 5120]
	movzx esi, ch
	movzx edi, dh
	inc   dword[r8 + 4*rsi + 6144]
	inc   dword[r8 + 4*rdi + 7168]
	shr   rcx, 16
	shr   rdx, 16
	movzx esi, al
	movzx edi, bl
	inc   dword[r8 + 4*rsi]
	inc   dword[r8 + 4*rdi + 1024]
	movzx esi, ah
	movzx edi, bh
	inc   dword[r8 + 4*rsi + 2048]
	inc   dword[r8 + 4*rdi + 3072]
	shr   rax, 16
	shr   rbx, 16
	movzx esi, cl
	movzx edi, dl
	inc   dword[r8 + 4*rsi + 4096]
	inc   dword[r8 + 4*rdi + 5120]
	movzx esi, ch
	movzx edi, dh
	inc   dword[r8 + 4*rsi + 6144]
	inc   dword[r8 + 4*rdi + 7168]
	shr   rcx, 16
	shr   rdx, 16
	movzx esi, al
	movzx edi, bl
	inc   dword[r8 + 4*rsi]
	inc   dword[r8 + 4*rdi + 1024]
	movzx esi, ah
	movzx edi, bh
	inc   dword[r8 + 4*rsi + 2048]
	inc   dword[r8 + 4*rdi + 3072]
	shr   rax, 16
	shr   rbx, 16
	movzx esi, cl
	movzx edi, dl
	inc   dword[r8 + 4*rsi + 4096]
	inc   dword[r8 + 4*rdi + 5120]
	movzx esi, ch
	movzx edi, dh
	inc   dword[r8 + 4*rsi + 6144]
	inc   dword[r8 + 4*rdi + 7168]
	shr   rcx, 16
	shr   rdx, 16
	movzx esi, al
	movzx edi, bl
	inc   dword[r8 + 4*rsi]
	inc   dword[r8 + 4*rdi + 1024]
	movzx esi, ah
	movzx edi, bh
	inc   dword[r8 + 4*rsi + 2048]
	inc   dword[r8 + 4*rdi + 3072]
	movzx eax, cl
	movzx ebx, dl
	movzx ecx, ch
	movzx edx, dh
	inc   dword[r8 + 4*rax + 4096]
	inc   dword[r8 + 4*rbx + 5120]
	inc   dword[r8 + 4*rcx + 6144]
	inc   dword[r8 + 4*rdx + 7168]

	dec   r9
	jnz   histo_loop

	pop   rdi
	pop   rsi
	pop   rbx
	ret


;==================================================================================================

%if 0

; rcx: count
; rdx: data
; r8:  histo
GLOBAL rmgr_histo_x86_bmi
rmgr_histo_x86_bmi:
	push  rbx
	push  rsi
	push  rdi
	push  r12
	push  r13
	push  r14
	push  r15
	mov   r9,  rcx
	mov   r10, rdx
	mov   r11, 0810h
	mov   r12, 0818h
	mov   r13, 0820h
	mov   r14, 0828h
	mov   r15, 0830h
histo_loop_bmi:
	mov   rax, [r10]
	mov   rbx, [r10+8]
	mov   rcx, [r10+16]
	mov   rdx, [r10+24]
	add   r10, 32
	movzx esi, al
	movzx edi, bl
	inc   dword[r8 + 4*rsi]
	inc   dword[r8 + 4*rdi + 1024]
	movzx esi, al
	movzx edi, bl
	inc   dword[r8 + 4*rsi + 2048]
	inc   dword[r8 + 4*rdi + 3072]
	movzx esi, ah
	movzx edi, bh
	inc   dword[r8 + 4*rsi + 4096]
	inc   dword[r8 + 4*rdi + 5120]
	movzx esi, ch
	movzx edi, dh
	inc   dword[r8 + 4*rsi + 6144]
	inc   dword[r8 + 4*rdi + 7168]
	bextr rsi, rax, r11
	bextr rdi, rbx, r11
	inc   dword[r8 + 4*rsi]
	inc   dword[r8 + 4*rdi + 1024]
	bextr rsi, rcx, r11
	bextr rdi, rdx, r11
	inc   dword[r8 + 4*rsi + 2048]
	inc   dword[r8 + 4*rdi + 3072]
	bextr rsi, rax, r12
	bextr rdi, rbx, r12
	inc   dword[r8 + 4*rsi + 4096]
	inc   dword[r8 + 4*rdi + 5120]
	bextr rsi, rcx, r12
	bextr rdi, rdx, r12
	inc   dword[r8 + 4*rsi + 6144]
	inc   dword[r8 + 4*rdi + 7168]
	bextr rsi, rax, r13
	bextr rdi, rbx, r13
	inc   dword[r8 + 4*rsi]
	inc   dword[r8 + 4*rdi + 1024]
	bextr rsi, rcx, r13
	bextr rdi, rdx, r13
	inc   dword[r8 + 4*rsi + 2048]
	inc   dword[r8 + 4*rdi + 3072]
	bextr rsi, rax, r14
	bextr rdi, rbx, r14
	inc   dword[r8 + 4*rsi + 4096]
	inc   dword[r8 + 4*rdi + 5120]
	bextr rsi, rcx, r14
	bextr rdi, rdx, r14
	inc   dword[r8 + 4*rsi + 6144]
	inc   dword[r8 + 4*rdi + 7168]
	bextr rsi, rax, r15
	bextr rdi, rbx, r15
	inc   dword[r8 + 4*rsi]
	inc   dword[r8 + 4*rdi + 1024]
	bextr rsi, rcx, r15
	bextr rdi, rdx, r15
	inc   dword[r8 + 4*rsi + 2048]
	inc   dword[r8 + 4*rdi + 3072]
	shr   rax, 56
	shr   rbx, 56
	shr   rcx, 56
	shr   rdx, 56
	inc   dword[r8 + 4*rax + 4096]
	inc   dword[r8 + 4*rbx + 5120]
	inc   dword[r8 + 4*rcx + 6144]
	inc   dword[r8 + 4*rdx + 7168]

	dec   r9
	jnz   histo_loop_bmi

	pop   r15
	pop   r14
	pop   r13
	pop   r12
	pop   rdi
	pop   rsi
	pop   rbx
	ret

%else

; rcx: count
; rdx: data
; r8:  histo
GLOBAL rmgr_histo_x86_bmi
rmgr_histo_x86_bmi:
	push  rbx
	push  rsi
	push  rdi
	push  r12
	push  r13
	push  r14
	push  r15
	mov   r9,  rcx
	mov   r10, rdx
	mov   r11, 0810h
	mov   r12, 0818h
	mov   r13, 0820h
	mov   r14, 0828h
	mov   r15, 0830h
histo_loop_bmi:
	mov   rax, [r10]
	mov   rbx, [r10+8]
	mov   rcx, [r10+16]
	mov   rdx, [r10+24]
	add   r10, 32
	movzx esi, al
	movzx edi, bl
	inc   dword[r8 + 4*rsi]
	inc   dword[r8 + 4*rdi + 1024]
	movzx esi, ah
	movzx edi, bh
	inc   dword[r8 + 4*rsi + 2048]
	inc   dword[r8 + 4*rdi + 3072]
	movzx esi, cl
	movzx edi, dl
	inc   dword[r8 + 4*rsi + 4096]
	inc   dword[r8 + 4*rdi + 5120]
	movzx esi, ch
	movzx edi, dh
	inc   dword[r8 + 4*rsi + 6144]
	inc   dword[r8 + 4*rdi + 7168]

	bextr rsi, rax, r11
	bextr rdi, rbx, r11
	shr   rcx, 16
	shr   rdx, 16
	inc   dword[r8 + 4*rsi]
	inc   dword[r8 + 4*rdi + 1024]
	movzx rsi, cl
	movzx rdi, dl
	inc   dword[r8 + 4*rsi + 2048]
	inc   dword[r8 + 4*rdi + 3072]
	bextr rsi, rax, r12
	bextr rdi, rbx, r12
	inc   dword[r8 + 4*rsi + 4096]
	inc   dword[r8 + 4*rdi + 5120]
	movzx esi, ch
	movzx edi, dh
	inc   dword[r8 + 4*rsi + 6144]
	inc   dword[r8 + 4*rdi + 7168]

	bextr rsi, rax, r13
	bextr rdi, rbx, r13
	shr   rcx, 16
	shr   rdx, 16
	inc   dword[r8 + 4*rsi]
	inc   dword[r8 + 4*rdi + 1024]
	movzx rsi, cl
	movzx rdi, dl
	inc   dword[r8 + 4*rsi + 2048]
	inc   dword[r8 + 4*rdi + 3072]
	bextr rsi, rax, r14
	bextr rdi, rbx, r14
	inc   dword[r8 + 4*rsi + 4096]
	inc   dword[r8 + 4*rdi + 5120]
	movzx esi, ch
	movzx edi, dh
	inc   dword[r8 + 4*rsi + 6144]
	inc   dword[r8 + 4*rdi + 7168]

	bextr rsi, rax, r15
	bextr rdi, rbx, r15
	shr   rcx, 16
	shr   rdx, 16
	inc   dword[r8 + 4*rsi]
	inc   dword[r8 + 4*rdi + 1024]
	movzx rsi, cl
	movzx rdi, dl
	inc   dword[r8 + 4*rsi + 2048]
	inc   dword[r8 + 4*rdi + 3072]
	shr   rax, 56
	shr   rbx, 56
	movzx ecx, ch
	movzx edx, dh
	inc   dword[r8 + 4*rax + 4096]
	inc   dword[r8 + 4*rbx + 5120]
	inc   dword[r8 + 4*rcx + 6144]
	inc   dword[r8 + 4*rdx + 7168]

	dec   r9
	jnz   histo_loop_bmi

	pop   r15
	pop   r14
	pop   r13
	pop   r12
	pop   rdi
	pop   rsi
	pop   rbx
	ret

%endif
