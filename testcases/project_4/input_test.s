# Function declaration
main:

# Allocate stack
addiu $sp, $sp, -40
# Save registers
sw $fp, 0($sp)
sw $s0, 4($sp)
sw $s1, 8($sp)
sw $s2, 12($sp)
sw $s3, 16($sp)
sw $s4, 20($sp)
sw $s5, 24($sp)
sw $s6, 28($sp)
sw $s7, 32($sp)
sw $ra, 36($sp)
addiu $fp, $sp, 40

# Compound statement for function

# Compound Statement

# Local variable declaration
addiu $sp, $sp, -4

# Local variable declaration
addiu $sp, $sp, -4

# Local variable declaration
addiu $sp, $sp, -4

# Local stack cleanup
addiu $sp, $sp, 12

# Stack cleanup
lw $fp, 0($sp)
lw $s0, 4($sp)
lw $s1, 8($sp)
lw $s2, 12($sp)
lw $s3, 16($sp)
lw $s4, 20($sp)
lw $s5, 24($sp)
lw $s6, 28($sp)
lw $s7, 32($sp)
lw $ra, 36($sp)
addiu $sp, $sp, 40

# Return to caller
jr $ra

