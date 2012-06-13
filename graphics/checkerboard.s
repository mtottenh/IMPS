start:	jal init 		-Initialise PPM
	jal pixels		- Print pixels
	halt			- Complete execution of program
init:	push 80			- Pushes P onto stack
	call print		- Prints P
	push 54			- Pushes 6 onto stack
	call print		- Prints 6
	push [spacechar]		- Pushes space_char (32) character onto stack
	call print		- Prints spacechar
	call print_width
	push [spacechar]
	call print		- Prints width
	call print_height		- Prints height
	push [spacechar]		- Pushes space_char (32) character onto stack
	call print		- Prints spacechar
	call print_colour		- Prints maxcolour
	push [spacechar]		- Pushes space_char character onto stack
	call print		- Prints spacechar
	jr $31			- Returns back to start label
pixels:	push $3			- Save state of R3	
	push $4			- Save state of R4
	addi $4 $3 4		- R4 = R3 + 4
start_l:	beq $4 $3 finish	- If R4 == R3 jump to finish
	push $1			- Save state of R1
	push $2			- Save state of R2
	addi $2 $1 16 		- R2 = R1 + 15 (inner loop start)
start_w:	call print_white	- Print white square four times (4 pixels)
	call print_white
	call print_white
	call print_white
	call print_black	- Print black square four times (4 pixels)
	call print_black
	call print_black
	call print_black
	addi $1 $1 1		- Increment R1
	bne $1 $2 start_w	- If R1 != R2, jump to start
	pop $2			- Restore state of R2
	pop $1			- Restore state of R1
	push $1			- Save state of R1
	push $2			- Save state of R2
	addi $2 $1 16 		- R2 = R1 + 16 (inner loop start)
start_b:	beq $1 $2 start_l		- If R1 == R2, jump to start
	call print_black	- Print black square four times (4 pixels)
	call print_black
	call print_black
	call print_black
	call print_white	- Print white square four times (4 pixels)
	call print_white
	call print_white
	call print_white
	addi $1 $1 1		- Increment R1
	bne $1 $2 start_b	- If R1 != R2 jump to start_b
	pop $2			- Restore state of R2
	pop $1			- Restore state of R1
	addi $3 $3 1		- Increment R3
	jmp start_l		- Jump to start label
finish:	pop $4			- Restore state of R4
	pop $3			- Restore state of R3
	jr $31			- Return to caller

print_white:	push 0		- Print 0x00 X 3
		call print
		push 0
		call print
		push 0
		call print
		ret

print_black: 	push [maxcolour]	- Print max_colour_val X 3
		call print
		push [maxcolour]
		call print
		push [maxcolour]
		call print
		ret

print:	pop $30			- Pop addr off stack and store in R30
	pop $29			- Pop pushed data
	push $30		- Push data back onto stack
	out $29			- Print out least significant byte of R30
	ret			- Return to caller

print_width: 	push 54
		call print
		push 52
		call print
		ret

print_height:	call print_width
		ret

print_colour:	push 50
		call print
		push 53
		call print
		push 53
		call print
		ret

spacechar:	.fill	32
p_char:		.fill	80
width: 		.fill	64
height: 	.fill	64
maxcolour: .fill 	255
