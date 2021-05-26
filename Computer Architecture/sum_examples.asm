# recursive sum
#
#@+
# // original function:
# public int recursiveSum(int i, int[] array)
# {
#
#     if (i == (array.length - 1))
#         return array[i];
#
#     return array[i] + recursiveSum(i + 1,array);
# }
#
# // function as it could be implemented:
# public int recursiveSum(int i, int[] array)
# {
#     int ret;
#
#     if (i == (array.length - 1))
#         ret = 0;
#     else
#         ret = recursiveSum(i + 1,array);
#
#     return array[i] + ret;
# }
#
# // function as it _was_ be implemented:
# public int[] array; // in a global register
#
# public int recursiveSum(int i)
# {
#     int ret;
#
#     if (i == (array.length - 1))
#         ret = 0;
#     else
#         ret = recursiveSum(i + 1);
#
#     return array[i] + ret;
# }
#@-

    .data

sdata:

array:
    .word   1,2,3,4,5,6,7,8,9,10
    .word   11,12,13,14,15,16,17,18,19,20
    .word   11,22,23,24,25,26,27,28,29,30

arrend:

msg_simple: .asciiz     "simple sum is: "
msg_recur1: .asciiz     "recursive sum (method 1) is: "
msg_recur2: .asciiz     "recursive sum (method 2) is: "
msg_match:  .asciiz     "difference is: "
msg_nl:     .asciiz     "\n"
    .text
    .globl  main

# main -- main program
#
# registers:
#   s0 -- array count (i.e. number of integers/words)
#   s1 -- array pointer
#   s2 -- array count - 1
#
#   s3 -- simple sum
#   s4 -- recursive sum
main:
    la      $s1,array               # get array address
    la      $s0,arrend              # get address of array end
    subu    $s0,$s0,$s1             # get byte length of array
    srl     $s0,$s0,2               # count = len / 4
    subi    $s2,$s0,1               # save count - 1

    # get simple sum for reference
    jal     sumsimple               # get simple sum
    move    $s3,$v0                 # save for compare

    # show the simple sum results
    la      $a0,msg_simple
    move    $a1,$v0
    jal     showsum

    # get recursive sum (method 1)
    li      $a0,0                   # i = 0
    jal     sumrecurs1              # get recursive sum
    move    $s4,$v0                 # save for compare

    # show the recursive sum results
    la      $a0,msg_recur1
    move    $a1,$v0
    jal     showsum

    # get recursive sum (method 2)
    li      $a0,0                   # i = 0
    jal     sumrecurs2              # get recursive sum

    # show the recursive sum results
    la      $a0,msg_recur2
    move    $a1,$v0
    jal     showsum

    # show the difference in values between simple and method 1
    subu    $a1,$s4,$s3             # difference of values
    la      $a0,msg_match
    jal     showsum

    # exit the program
    li      $v0,10
    syscall

# sumsimple -- compute simple sum by looping through array
#
# RETURNS:
#   v0 -- sum
#
# registers:
#   t0 -- array count
#   t1 -- array pointer
sumsimple:
    move    $t0,$s0                 # get array count
    move    $t1,$s1                 # get array address

    li      $v0,0                   # sum = 0
    j       sumsimple_test

sumsimple_loop:
    lw      $t2,0($t1)              # get array[i]
    add     $v0,$v0,$t2             # sum += array[i]

    addi    $t1,$t1,4               # advance pointer to array[i + 1]
    subi    $t0,$t0,1               # decrement count

sumsimple_test:
    bgtz    $t0,sumsimple_loop      # are we done? if no, loop

    jr      $ra                     # return

# sumrecurs1 -- compute recursive sum
#
# RETURNS:
#   v0 -- sum
#
# arguments:
#   a0 -- array index (i)
#   s1 -- array pointer
#
# NOTES:
# (1) in the mips ABI, the second argument is normally passed in a1 [which can
#     be trashed] but we are using s1 as a "global" register
# (2) this saves an extra [and unnecessary push/pop to stack] as s1 _must_ be
#     preserved
# (3) we do, however, preserve the array index (a0) on the stack
sumrecurs1:
    # save return address and argument on a stack frame we setup
    subiu   $sp,$sp,8
    sw      $ra,0($sp)
    sw      $a0,4($sp)

    blt     $a0,$s2,sumrecurs1_call # at the end? if no, fly
    li      $v0,0                   # yes, simulate call return of 0
    j       sumrecurs1_done         # skip to function epilog

sumrecurs1_call:
    addi    $a0,$a0,1               # bump up the index
    jal     sumrecurs1              # recursive call

    # get back the index we were called with from our stack frame
    # NOTES:
    # (1) while we _could_ just subtract one from a0 here because of the way
    #     sumrecurs is implemented, we _don't_ because, in general, under the
    #     standard mips ABI the called function is at liberty to _trash_ it
    # (2) the index value restored in the epilog of our recursive function
    #     call is _not_ _our_ value "i", but "i + 1", so we need to get our
    #     "i" value from _our_ stack frame
    # (3) see sumrecurs2 for the faster method where we "cheat" and violate
    #     the ABI
    lw      $a0,4($sp)

sumrecurs1_done:
    sll     $t2,$a0,2               # get byte offset for index i
    add     $t2,$s1,$t2             # get address of array[i]
    lw      $t2,0($t2)              # fetch array[i]
    add     $v0,$t2,$v0             # sum our value and callee's

    # restore return address from stack
    lw      $ra,0($sp)
    lw      $a0,4($sp)
    addiu   $sp,$sp,8
    jr      $ra

# sumrecurs2 -- compute recursive sum
#
# RETURNS:
#   v0 -- sum
#
# arguments:
#   a0 -- array index (i)
#   s1 -- array pointer
#
# NOTES:
# (1) in the mips ABI, the second argument is normally passed in a1 [which can
#     be trashed] but we are using s1 as a "global" register
# (2) this saves an extra [and unnecessary push/pop to stack] as s1 _must_ be
#     preserved
# (3) we do, however, preserve the array index (a0) on the stack
sumrecurs2:
    # save _only_ return address on a stack frame we setup
    subiu   $sp,$sp,4
    sw      $ra,0($sp)

    blt     $a0,$s2,sumrecurs2_call # at the end? if no, fly
    li      $v0,0                   # yes, simulate call return of 0
    j       sumrecurs2_done         # skip to function epilog

sumrecurs2_call:
    addi    $a0,$a0,1               # bump up the index
    jal     sumrecurs2              # recursive call
    subi    $a0,$a0,1               # bump down the index

sumrecurs2_done:
    sll     $t2,$a0,2               # get byte offset for index i
    add     $t2,$s1,$t2             # get address of array[i]
    lw      $t2,0($t2)              # fetch array[i]
    add     $v0,$t2,$v0             # sum our value and callee's

    # restore return address from stack
    lw      $ra,0($sp)
    addiu   $sp,$sp,4
    jr      $ra

# showsum -- output a message
#
# arguments:
#   a0 -- message
#   a1 -- number value
showsum:
    li      $v0,4                   # puts
    syscall

    move    $a0,$a1                 # get number to print
    li      $v0,1                   # prtint
    syscall

    la      $a0,msg_nl
    li      $v0,4                   # puts
    syscall

    jr      $ra