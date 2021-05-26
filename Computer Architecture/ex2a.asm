.data 
array: .word 12,15,10,5,7,3,2,1,15,10,15,-5,-5,-5
arrend:
msg: .asciiz     "hello, the array is:12,15,10,5,7,3,2,1,15,10,15\nand the result is: "
msg2: .asciiz     "\ninput 1 integer: "
msg3: .asciiz     "\narray index: "
nline: .asciiz "\n"
.text 
.globl main

main:
la      $a0,msg
move    $a1,$v0
li      $v0,4                  
syscall
la      $a0,array               # get array address
la      $s0,arrend              # get address of array end
subu    $s0,$s0,$a0             # get byte length of array
div     $s0,$s0,4               # count = len / 4
sub    $a2,$s0,1               #index high= count - 1
addi $a1, $zero, 0 # pointer 2(index low = 0)
jal quicksort # Call quick sort

    li      $v0, 0
    la      $t1, array
loop1:
    bge     $t0, 10, exit

    # load word from addrs and goes to the next addrs
    lw      $t2, 0($t1)
    addi    $t1, $t1, 4

    # syscall to print value
    li      $v0, 1      
    move    $a0, $t2
    syscall
    # print(space)
    li      $a0, 32
    li      $v0, 11  
    syscall


    #increment 
    addi    $t0, $t0, 1
    j      loop1

 exit:
la      $a0,msg2
move    $a1,$v0
li      $v0,4                  
syscall
addi $v0, $zero, 5
syscall
la $s0, array # array address
addi $s1, $0, 11 # arraysize
add $s2,$0, $v0 
la      $a0,msg3
move    $a1,$v0
li      $v0,4                  
syscall
jal Binsearch

  li $v0, 10
  syscall

Binsearch:
  li $t0,0                           #first = 0
  move $t1, $s1                      #last = array size -1
  li $t6,1
  li $t7,4                           #checks for the last element in the array being the correct value
  multu $t7,$t1
  mflo  $t7
  add   $t7,$s0,$t7
  lw    $t5, ($t7)
  beq   $t5, $s2, last_num          # if  ==  then
  subu $t1, $s1, 1

Loop: bge $t0, $t1, DONE             # if ! (first < last)
  add $t2,$t0,$t1                    #first + last
  li $t3,2
  div $t2, $t3                       #$LO = middle index
  mflo $t3                           #$t3 = middle index
  beq $t2,$t6, special_case          #if first+last = 1 you want totake HI not LO
  li $t7,4                           #load the value 4 into t7
  multu $t7, $t3                     #multiply middle by 4 to get the offset
  mflo $t4                           #store the result in t4
  add $t4, $s0, $t4                  #t4 points to array[mid]
  lw $t5, ($t4)                      #load the value at array[mid] into t5
  beq $t5, $s2, return_mid           # if   t5 == s2  then return the index
  blt $t5, $s2, move_right           # if   t5 < s2 then move right
  subu $t1, $t3, 1                   #if this line is reached that means that none of the above
  j    Loop                          #conditions are true, so t5 > s2, last = mid -1
special_case:
    mfhi $t3                           #get the 1 value from hi
    li $t7,4                           #load the value 4 into t7
    multu $t7, $t3                     #multiply middle by 4 to get the offset
    mflo $t4                           #store the result in t4
    add $t4, $s0, $t4                  #t4 points to array[mid]
    lw $t5, ($t4)                      #load the value at array[mid] into t5
    beq $t5, $s2, return_mid           # if   t5 == s2  then return the index
return_mid:
    li $v0, 1
    add $a0, $t3, $zero              # add the middle index to a0
    syscall                          #print the index
    jr $ra
move_right:
  addi  $t0, $t3, 1                  # first = mid + 1
  j    Loop                          # jump to Loop
last_num:
  li $v0, 1
  add $a0, $t1, $zero
  syscall
  jr $ra
DONE:
  li $v0, 1
  li $a0, -1
  syscall

  jr $ra


swap:				#swap method

	addi $sp, $sp, -12	# Make stack room for three

	sw $a0, 0($sp)		# Store a0
	sw $a1, 4($sp)		# Store a1
	sw $a2, 8($sp)		# store a2

	sll $t1, $a1, 2	 	#offset pointer
	add $t1, $a0, $t1	#get pointer addr
	lw $s3, 0($t1)		#s3  = pointer value a1 ( arr[a] )

	sll $t2, $a2, 2		
	add $t2, $a0, $t2	
	lw $s4, 0($t2)		#s4 = arr[b]

	sw $s4, 0($t1)		#arr[a] = arr[b]
	sw $s3, 0($t2)		#arr[b] = t 


	addi $sp, $sp, 12	#Restoring the stack size
	jr $ra			#jump back to the caller
	


partition: 			

	addi $sp, $sp, -16	#room for 5

	sw $a0, 0($sp)		#store a0
	sw $a1, 4($sp)		#store a1
	sw $a2, 8($sp)		#store a2
	sw $ra, 12($sp)		#store return address
	
	move $s1, $a1		#s1 = low
	move $s2, $a2		#s2 = high

	sll $t1, $s2, 2		
	add $t1, $a0, $t1	
	lw $t2, 0($t1)		# t2 = arr[high] //pivot//

	addi $t3, $s1, -1 	#t3, i=low -1
	move $t4, $s1		#t4, j=low
	addi $t5, $s2, -1	#t5 = high - 1

	loop: 		
		bgt    $t4, $t5, endfor	#if j>high-1 then branch to endfor

		sll $t7, $t4, 2		
		add $t7, $t7, $a0	
		lw $t7, 0($t7)		#t7 = arr[j]

		blt    $t2, $t7,endfif	# if pivot < arr[j], endfif
		addi $t3, $t3, 1	#i=i+1

		move $a1, $t3		#a1 = i
		move $a2, $t4		#a2 = j
		jal swap		#swap(arr[i] & arr[j])
		
		addi $t4, $t4, 1	#j++
		j loop

	    endfif:
		addi $t4, $t4, 1	#j++
		j loop		#junp back to loop

	endfor:
		addi $a1, $t3, 1		#a1 = i+1
		move $a2, $s2			#a2 = high
		add $v0, $zero, $a1		#v0 = i+1;
		jal swap			#swap(arr, i + 1, high);

		lw $ra, 12($sp)			#return address
		addi $sp, $sp, 16		
		jr $ra				

quicksort:				#quicksort method

	addi $sp, $sp, -16		# Make room for 4

	sw $a0, 0($sp)			# arr
	sw $a1, 4($sp)			# low
	sw $a2, 8($sp)			# high
	sw $ra, 12($sp)			# return address

	
	bge $a1, $a2, endif		# if low >= high, endif

	jal partition			
	move $s0, $v0			# pivot, s0= v0

	lw $a1, 4($sp)			#a1 = low
	addi $a2, $s0, -1		#a2 = pivot -1
	jal quicksort			#call quicksort

	addi $a1, $s0, 1		#a1 = pivot + 1
	lw $a2, 8($sp)			#a2 = high
	jal quicksort			

 endif:

 	lw $a0, 0($sp)			#restore values
 	lw $a1, 4($sp)			
 	lw $a2, 8($sp)			
 	lw $ra, 12($sp)			#restore return address
 	addi $sp, $sp, 16		#restore the stack
 	jr $ra				#return to caller
