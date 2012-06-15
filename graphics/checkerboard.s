	call init 		- Initialise PPM
	call pixels		- Print pixels
	halt			- Complete execution of program

init:	push [pchar]		- Print [pchar]
	call print		
	push [6char]		- Print [6char]
	call print		
	push [spacechar]	- Print [spacechar]
	call print	
	call print_width	- Print width
	push [spacechar]	- Print [spacechar]
	call print		
	call print_height	- Prints height
	push [spacechar]	- Print [spacechar]
	call print		
	call print_colour	- Prints maxcolour
	push [spacechar]	- Print [spacechar]
	call print		
	ret			- Returns back to start label

pixels:		push $1			- Saves state of registers
		push $2
		mov $2 , 4		- R2 = 4, R1 = 0
		mov $1 , 0
pixels1:	call p_white_block	- Print a row of white and black blocks
		call p_black_block	- Print a row of black and white blocks
		inc $1			- R1++
		bne $1 $2 pixels1	- If R1 != R2 jump to pixels1
		pop $2			- Restore registers
		pop $1
		ret

p_white_block:	push $1			- Save state of registers
		push $2
		mov $2 , 8		- R2 = 8
		mov $1 , 0		- R1 = 0
p_white_block1:	call p_white_start	- Print a line of white and black pixels
		inc $1			- R1++
		bne $1 $2 p_white_block1 - If R1 != R2 jump to p_white_block1
		pop $2			- Restore registers
		pop $1
		ret

p_black_block:	push $1			- Save state of registers
		push $2
		mov $2 , 8		- R2 = 8
		mov $1 , 0		- R1 = 0
p_black_block1:	call p_black_start	- Print a line of black and white pixels
		inc $1			- R1++
		bne $1 $2 p_black_block1 - If R1 != R2 jump to p_black_block1
		pop $2			- Restore registers
		pop $1
		ret

p_black_start: 	push $1			- Save state of registers
		push $2
		mov $2 , 4		- R2 = 4
		mov $1 , 0		- R1 = 0
p_black_start1:	push $3			- Save state of registers
		push $4
		mov $4 , 8		- R4 = 8 
		mov $3 , 0		- R3 = 0
p_black_start2:	call print_black	- Print a black pixel
		inc $3			- R3++
		bne $3 $4 p_black_start2 - If R3 != R4 jump to p_black_start2
		pop $4			- Restore registers
		pop $3
		push $3			- Save state of registers
		push $4
		mov $4 , 8		- R4 = 8
		mov $3 , 0		- R3 = 0
p_black_start3:	call print_white	- Print a white pixel
		inc $3
		bne $3 $4 p_black_start3 - If R3 != R4 jump to p_black_start3
		pop $4			- Restore registers
		pop $3
		inc $1			- R1++
		bne $1 $2 p_black_start1 - If R1 != R2 jump to p_black_start1
		pop $2			- Restore registers
		pop $1
		ret

p_white_start: 	push $1			- Save state of registers
		push $2			
		mov $2 , 4		- R2 = 4
		mov $1 , 0		- R1 = 0
p_white_start1:	push $3			- Save state of registers
		push $4
		mov $4 , 8		- R4 = 8
		mov $3 , 0		- R3 = 0
p_white_start2:	call print_white	- Print a white pixel
		inc $3			- R3++
		bne $3 $4 p_white_start2 - If R3 != R4 jump to p_white_start2
		pop $4			- Restore state of registers
		pop $3
		push $3			- Save state of registers
		push $4
		mov $4 , 8		- R4 = 8
		mov $3 , 0		- R3 = 0
p_white_start3:	call print_black	- Print a black pixel
		inc $3			- R3++
		bne $3 $4 p_white_start3 - If R3 != R4 jump to p_white_start3
		pop $4			- Restore registers
		pop $3
		inc $1			- R1++
		bne $1 $2 p_white_start1 - If R1 != R2 jump to p_white_start1 
		pop $2			- Restore registers
		pop $1
		ret

print_black:	push 0			- Print 0x00 X 3
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

print:	pop $30			- Pop off the address
	pop $29			- Pop off the data
	push $30		- Push the address for the return later
	out $29			- Print least 8 bits of R29
	ret			- Return

print_width: 	push [6char]		- Print 64
		call print
		push [4char]
		call print
		ret

print_height:	call print_width 	- Print 64 
		ret

print_colour:	push [2char]		- Print 255
		call print
		push [5char]
		call print
		push [5char]
		call print
		ret

spacechar:	.fill	32
pchar:		.fill	80
6char:		.fill 	54
5char:		.fill	53
4char:		.fill	52
2char:		.fill	50
width: 		.fill	64
height: 	.fill	64
maxcolour: 	.fill 	255
