.data
myArray: .word 1 4 5 7 9 12 15 17 18 20 21 30 1 -5 4 7 21 8 1 2 4 1 4 5 2 1 4
last: #the address that comes after the array
arraySize: .word 11

.globl main
.text

main:
  la $s0, myArray # array address
  lw $s1, arraySize # arraysize
  li $s2, 20 #value youre searching for
  jal Binsearch # perform binary search

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

  jr $ra #return to the caller
