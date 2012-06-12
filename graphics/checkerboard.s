.fill 	space_char_char	32
.fill 	p_char 		80
.fill 	width		64
.fill 	height		64
.fill	max_colour_val 255

start:	jal init		- Initialise PPM
	jal pixels		- Print pixels
	halt			- Complete execution of program

init:	push p_char		- Pushes P onto stack ASCII VAL?
	call print		- Prints P
	push 6			- Pushes 6 onto stack
	call print		- Prints 6
	push space_char		- Pushes space_char (32) character onto stack
	call print		- Prints space_char
	push width		- Pushes width (64) onto stack
	call print 		- Prints width
	push height 		- Pushes height (64) onto stack
	call print		- Prints height
	push space_char		- Pushes space_char (32) character onto stack
	call print		- Prints space_char
	push max_colour_val	- Pushes max_colour_val (255) onto stack
	call print		- Prints max_colour_val
	push space_char		- Pushes space_char character onto stack
	call print		- Prints space_char
	jr  $31			- Returns back to start label
	
pixels:	push $3			- Save state of R3	
	push $4			- Save state of R4

	addi $4 $3 4		- R4 = R3 + 4
start:	beq $4 $3 finish	- If R4 == R3 jump to finish

	push $1			- Save state of R1
	push $2			- Save state of R2
	addi $2 $1 16 		- R2 = R1 + 15 (inner loop start)
start_w:call print_white	- Print white square four times (4 pixels)
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
start_b:beq $1 $2 start		- If R1 == R2, jump to start
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
	jmp start		- Jump to start label

finish:	pop $4			- Restore state of R4
	pop $3			- Restore state of R3
	jr $31			- Return to caller

print_white :	push 0		- Print 0x00 X 3
		call print
		push 0
		call print
		push 0
		call print
		ret

print_black: 	push max_colour_val	- Print max_colour_val X 3
		call print
		push max_colour_val
		call print
		push max_colour_val
		call print

print:	pop $30			- Pop value off stack and store in R30
	out $30			- Print out least significant byte of R30
	ret			- Return to caller

