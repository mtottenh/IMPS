start:	jal init 		-Initialise PPM
	call pixels		- Print pixels
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

pixels:		push $1
		push $2
		addi $2 $1 4
pixel1:	jal p_white_row
		jal p_black_row
		addi $1 $1 1
		bne $1 $2 pixel1
		pop $2
		pop $1
		ret

p_white_row:	push $1
		push $2
		addi $2 $1 8
p_white_row1:	call p_white_start
		addi $1 $1 1
		bne $1 $2 p_white_row1
		pop $2
		pop $1
		jr $31

p_black_row:	push $1
		push $2
		addi $2 $1 8
p_black_row1:	call p_black_start
		addi $1 $1 1
		bne $1 $2 p_black_row1
		pop $2
		pop $1
		jr $31

p_black_start: 	push $1
		push $2
		addi $2 $1 4	- R2 = R1 + 4
p_black_start1:	push $3
		push $4
		addi $4 $3 8	- R4 = R3 + 8
p_black_start2:	call print_black
		addi $3 $3 1
		bne $3 $4 p_black_start2
		pop $4
		pop $3
		push $3
		push $4
		addi $4 $3 8
p_black_start3:	call print_white
		addi $3 $3 1
		bne $3 $4 p_black_start3
		pop $4
		pop $3
		addi $1 $1 1
		bne $1 $2 p_black_start1
		pop $2
		pop $1
		ret  
p_white_start: 	push $1
		push $2
		addi $2 $1 4	- R2 = R1 + 4
p_white_start1:	push $3
		push $4
		addi $4 $3 8	- R4 = R3 + 8
p_white_start2:	call print_white
		addi $3 $3 1
		bne $3 $4 p_white_start2
		pop $4
		pop $3
		push $3
		push $4
		addi $4 $3 8
p_white_start3:	call print_black
		addi $3 $3 1
		bne $3 $4 p_white_start3
		pop $4
		pop $3
		addi $1 $1 1
		bne $1 $2 p_white_start1
		pop $2
		pop $1
		ret

print_black:	push 0		- Print 0x00 X 3
		call print
		push 0
		call print
		push 0
		call print
		ret

print_white: 	push [maxcolour]	- Print max_colour_val X 3
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
