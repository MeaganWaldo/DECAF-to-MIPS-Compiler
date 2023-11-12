#         PACKAGE foo 

.data	# Data section starts here, strings first 

_L0:	.asciiz	"Enter a number: "
_L1:	.asciiz	"\nThe number you entered: "
_L2:	.asciiz	"\nThe number you entered is greater than 50."
_L3:	.asciiz	"\nThe number you entered is less than or equal to 50."
_L4:	.asciiz	"\nj[k] is: "
_L5:	.asciiz	"\nw[x] is: "
_L6:	.asciiz	"\nx is: "
_L7:	.asciiz	"\nx = x + 1"
_L8:	.asciiz	"x = "
_L9:	.asciiz	"\n1 + 1 ="
_L10:	.asciiz	"\n1 - 1 = "
_L11:	.asciiz	"\n1 * 2 = "
_L12:	.asciiz	"\n8 / 2 = "
_NL: .asciiz "\n" # New line

.align 2 # start all of global variables aligned 

j:	.space 400	#global var
k:	.word 1	#global var with value


.text  # start of text(code) segment 

.globl main  # Force MIPS to start at main label 


main:		# METHOD LABEL

	subu $t0  $sp 524		#set up $t0 to be the new spot for SP
	sw $ra ($t0)		#Store the return address
	sw $sp 4($t0)		#Store the old stack pointer
	move $sp $t0		#set the stack pointer to the new value


	la $a0, _L0		#expr constant is a string
	li $v0 4		#set up write call
	syscall		#print a string
	li $v0, 4		#print NEWLINE
	la $a0, _NL		#print NEWLINE string location
	syscall		#call print a NEWLINE

	li $v0, 5		#read int from user
	syscall		#to read a number
	move $a0, $v0		#Make the read in number $a0

	sw $a0 416($sp)		#store RHS temporarily
	li $a0, 8		#offset for variable address
	add $a0, $a0, $sp		#Exact location for stack variable
	lw $a1, 416($sp)		#load back RHS into $a1
	sw $a1, ($a0)		#Store assign value

	la $a0, _L1		#expr constant is a string
	li $v0 4		#set up write call
	syscall		#print a string
	li $v0, 4		#print NEWLINE
	la $a0, _NL		#print NEWLINE string location
	syscall		#call print a NEWLINE

	li $a0, 8		#offset for variable address
	add $a0, $a0, $sp		#Exact location for stack variable
	lw $a0, ($a0)		#Load from memory a value
	li $v0 1		#set up write call
	syscall		#print a number
	li $v0, 4		#print NEWLINE
	la $a0, _NL		#print NEWLINE string location
	syscall		#call print a NEWLINE

	li $a0, 8		#offset for variable address
	add $a0, $a0, $sp		#Exact location for stack variable
	lw $a0, ($a0)		#Load from memory a value
	sw $a0, 428($sp)		#Store LHS from expr temporarily
	li $a0, 50		#expr constant value
	move $a1, $a0		#copy RHS to a1
	lw $a0, 428($sp)		#retrive LHS from storage
	sgt $a0, $a0, $a1		#EXPR Greater Than
	beq $a0, $0, _L13		#Branch out 
	la $a0, _L2		#expr constant is a string
	li $v0 4		#set up write call
	syscall		#print a string
	li $v0, 4		#print NEWLINE
	la $a0, _NL		#print NEWLINE string location
	syscall		#call print a NEWLINE

	j _L14		#Jump
_L13:			#Else part of if
	la $a0, _L3		#expr constant is a string
	li $v0 4		#set up write call
	syscall		#print a string
	li $v0, 4		#print NEWLINE
	la $a0, _NL		#print NEWLINE string location
	syscall		#call print a NEWLINE

_L14:			#End of if statement

	li $a0, 40		#expr constant value
	sw $a0 432($sp)		#store RHS temporarily
	li $a0, 8		#offset for variable address
	add $a0, $a0, $sp		#Exact location for stack variable
	lw $a1, 432($sp)		#load back RHS into $a1
	sw $a1, ($a0)		#Store assign value

	li $a0, 45		#expr constant value
	sw $a0 436($sp)		#store RHS temporarily
	la $a0, k		#Load global address of variable
	lw $a1, 436($sp)		#load back RHS into $a1
	sw $a1, ($a0)		#Store assign value

	li $a0, 42		#expr constant value
	sw $a0 440($sp)		#store RHS temporarily
	li $a0, 8		#offset for variable address
	add $a0, $a0, $sp		#Exact location for stack variable
	lw $a0, ($a0)		#Load from memory a value
	sll $t3, $a0, 2		#Shift left by 2
	la $a0, j		#Load global address of variable
	add $a0, $a0, $t3		#Array reference
	lw $a1, 440($sp)		#load back RHS into $a1
	sw $a1, ($a0)		#Store assign value

	li $a0, 42		#expr constant value
	sw $a0 444($sp)		#store RHS temporarily
	la $a0, k		#Load global address of variable
	lw $a0, ($a0)		#Load from memory a value
	sll $t3, $a0, 2		#Shift left by 2
	li $a0, 12		#offset for variable address
	add $a0, $a0, $sp		#Exact location for stack variable
	add $a0, $a0, $t3		#Array reference
	lw $a1, 444($sp)		#load back RHS into $a1
	sw $a1, ($a0)		#Store assign value

	la $a0, _L4		#expr constant is a string
	li $v0 4		#set up write call
	syscall		#print a string
	li $v0, 4		#print NEWLINE
	la $a0, _NL		#print NEWLINE string location
	syscall		#call print a NEWLINE

	li $a0, 8		#offset for variable address
	add $a0, $a0, $sp		#Exact location for stack variable
	lw $a0, ($a0)		#Load from memory a value
	sll $t3, $a0, 2		#Shift left by 2
	la $a0, j		#Load global address of variable
	add $a0, $a0, $t3		#Array reference
	lw $a0, ($a0)		#Load from memory a value
	li $v0 1		#set up write call
	syscall		#print a number
	li $v0, 4		#print NEWLINE
	la $a0, _NL		#print NEWLINE string location
	syscall		#call print a NEWLINE

	la $a0, _L5		#expr constant is a string
	li $v0 4		#set up write call
	syscall		#print a string
	li $v0, 4		#print NEWLINE
	la $a0, _NL		#print NEWLINE string location
	syscall		#call print a NEWLINE

	la $a0, k		#Load global address of variable
	lw $a0, ($a0)		#Load from memory a value
	sll $t3, $a0, 2		#Shift left by 2
	li $a0, 12		#offset for variable address
	add $a0, $a0, $sp		#Exact location for stack variable
	add $a0, $a0, $t3		#Array reference
	lw $a0, ($a0)		#Load from memory a value
	li $v0 1		#set up write call
	syscall		#print a number
	li $v0, 4		#print NEWLINE
	la $a0, _NL		#print NEWLINE string location
	syscall		#call print a NEWLINE

	la $a0, _L6		#expr constant is a string
	li $v0 4		#set up write call
	syscall		#print a string
	li $v0, 4		#print NEWLINE
	la $a0, _NL		#print NEWLINE string location
	syscall		#call print a NEWLINE

	li $a0, 8		#offset for variable address
	add $a0, $a0, $sp		#Exact location for stack variable
	lw $a0, ($a0)		#Load from memory a value
	li $v0 1		#set up write call
	syscall		#print a number
	li $v0, 4		#print NEWLINE
	la $a0, _NL		#print NEWLINE string location
	syscall		#call print a NEWLINE

_L15:			#Start of while
	li $a0, 8		#offset for variable address
	add $a0, $a0, $sp		#Exact location for stack variable
	lw $a0, ($a0)		#Load from memory a value
	sw $a0, 472($sp)		#Store LHS from expr temporarily
	li $a0, 45		#expr constant value
	move $a1, $a0		#copy RHS to a1
	lw $a0, 472($sp)		#retrive LHS from storage
	slt $a0, $a0, $a1		#EXPR Less Than
	beq $a0, $0, _L16		#While branch out
	la $a0, _L7		#expr constant is a string
	li $v0 4		#set up write call
	syscall		#print a string
	li $v0, 4		#print NEWLINE
	la $a0, _NL		#print NEWLINE string location
	syscall		#call print a NEWLINE

	li $a0, 8		#offset for variable address
	add $a0, $a0, $sp		#Exact location for stack variable
	lw $a0, ($a0)		#Load from memory a value
	sw $a0, 480($sp)		#Store LHS from expr temporarily
	li $a0, 1		#expr constant value
	move $a1, $a0		#copy RHS to a1
	lw $a0, 480($sp)		#retrive LHS from storage
	add $a0, $a0, $a1		#EXPR Add
	sw $a0 484($sp)		#store RHS temporarily
	li $a0, 8		#offset for variable address
	add $a0, $a0, $sp		#Exact location for stack variable
	lw $a1, 484($sp)		#load back RHS into $a1
	sw $a1, ($a0)		#Store assign value

	la $a0, _L8		#expr constant is a string
	li $v0 4		#set up write call
	syscall		#print a string
	li $v0, 4		#print NEWLINE
	la $a0, _NL		#print NEWLINE string location
	syscall		#call print a NEWLINE

	li $a0, 8		#offset for variable address
	add $a0, $a0, $sp		#Exact location for stack variable
	lw $a0, ($a0)		#Load from memory a value
	li $v0 1		#set up write call
	syscall		#print a number
	li $v0, 4		#print NEWLINE
	la $a0, _NL		#print NEWLINE string location
	syscall		#call print a NEWLINE

	j _L15		#Jump back to top of while
_L16:			#End of while

	la $a0, _L9		#expr constant is a string
	li $v0 4		#set up write call
	syscall		#print a string
	li $v0, 4		#print NEWLINE
	la $a0, _NL		#print NEWLINE string location
	syscall		#call print a NEWLINE

	li $a0, 1		#expr constant value
	sw $a0, 480($sp)		#Store LHS from expr temporarily
	li $a0, 1		#expr constant value
	move $a1, $a0		#copy RHS to a1
	lw $a0, 480($sp)		#retrive LHS from storage
	add $a0, $a0, $a1		#EXPR Add
	li $v0 1		#set up write call
	syscall		#print a number
	li $v0, 4		#print NEWLINE
	la $a0, _NL		#print NEWLINE string location
	syscall		#call print a NEWLINE

	la $a0, _L10		#expr constant is a string
	li $v0 4		#set up write call
	syscall		#print a string
	li $v0, 4		#print NEWLINE
	la $a0, _NL		#print NEWLINE string location
	syscall		#call print a NEWLINE

	li $a0, 1		#expr constant value
	sw $a0, 492($sp)		#Store LHS from expr temporarily
	li $a0, 1		#expr constant value
	move $a1, $a0		#copy RHS to a1
	lw $a0, 492($sp)		#retrive LHS from storage
	sub $a0, $a0, $a1		#EXPR Subtract
	li $v0 1		#set up write call
	syscall		#print a number
	li $v0, 4		#print NEWLINE
	la $a0, _NL		#print NEWLINE string location
	syscall		#call print a NEWLINE

	la $a0, _L11		#expr constant is a string
	li $v0 4		#set up write call
	syscall		#print a string
	li $v0, 4		#print NEWLINE
	la $a0, _NL		#print NEWLINE string location
	syscall		#call print a NEWLINE

	li $a0, 1		#expr constant value
	sw $a0, 504($sp)		#Store LHS from expr temporarily
	li $a0, 2		#expr constant value
	move $a1, $a0		#copy RHS to a1
	lw $a0, 504($sp)		#retrive LHS from storage
	mult $a0, $a1		#EXPR Multiply
	mflo $a0		#The low
	li $v0 1		#set up write call
	syscall		#print a number
	li $v0, 4		#print NEWLINE
	la $a0, _NL		#print NEWLINE string location
	syscall		#call print a NEWLINE

	la $a0, _L12		#expr constant is a string
	li $v0 4		#set up write call
	syscall		#print a string
	li $v0, 4		#print NEWLINE
	la $a0, _NL		#print NEWLINE string location
	syscall		#call print a NEWLINE

	li $a0, 8		#expr constant value
	sw $a0, 516($sp)		#Store LHS from expr temporarily
	li $a0, 2		#expr constant value
	move $a1, $a0		#copy RHS to a1
	lw $a0, 516($sp)		#retrive LHS from storage
	div $a0 $a1		#EXPR Divide
	mflo $a0		#The low
	li $v0 1		#set up write call
	syscall		#print a number
	li $v0, 4		#print NEWLINE
	la $a0, _NL		#print NEWLINE string location
	syscall		#call print a NEWLINE



	li $v0 0		#return  NULL zero (0)
	lw $ra ($sp)		#reset return address
	lw $sp 4($sp)		#reset stack pointer
	li $v0, 10		#Main function ends
	syscall		#MAIN FUNCTION EXITS

