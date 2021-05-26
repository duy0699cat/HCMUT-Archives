   .data

array:	.word   1,-2,3,-4,5,-6,7,-8,9,-10
arrend:

msg: .asciiz     "hello, the array is:1,-2,3,-4,5,-6,7,-8,9,-10 \n max number is: "

    .text

#   s0 : array count (length)
#   s1 : array pointer
#   s2 : array count - 1
#   s4 : recursive max
main:
    la      $s1,array               # get array address
    la      $s0,arrend              # get address of array end
    subu    $s0,$s0,$s1             # get byte length of array
    div     $s0,$s0,4               # count = len / 4
    sub    $s2,$s0,1               # save count - 1
    
    # get max
    addi      $a0,$0,0                   # i = 0
    jal     max              # get recursive max

    # show result
    la      $a0,msg
    move    $a1,$v0
    jal     showres

    # exit
    li      $v0,10
    syscall

max:
   
    sub   $sp,$sp,4
    sw      $ra,0($sp)
    blt     $a0,$s2,max_call # at the end of arr
    j       ends        

max_call:
    addi    $a0,$a0,1               # i++
    jal     max              # recursive call
    sub    $a0,$a0,1               #i--

ends:
    sll     $t2,$a0,2               # get byte offset for index i
    add     $t2,$s1,$t2             # get address of array[i]
    lw      $t2,0($t2)              # fetch array[i]
blt $t2,$v0,skip_swap
move $v0,$t2 # swap
skip_swap:  # restore stack
    lw      $ra,0($sp)
    add     $sp,$sp,4
    jr      $ra
    

showres:
    li      $v0,4                  
    syscall
    add    $a0,$a1,0               
    li      $v0,1                  
    syscall
    jr      $ra
