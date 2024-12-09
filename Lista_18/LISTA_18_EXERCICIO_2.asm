.data
	baseStr: .asciiz "Base: "
    expoenteStr: .asciiz "Expoente: "
    resultadoStr: .asciiz "Resultado: "
.text

main:
    addi		$v0, $0, 4		# system call #4 - print string
    la		$a0, baseStr
    syscall						# execute
    addi		$v0, $0, 5		# system call #5 - input int
    syscall						# execute
	add $s0, $v0, $zero         # Armazenando base em $s0


    addi		$v0, $0, 4		# system call #4 - print string
    la		$a0, expoenteStr
    syscall						# execute
    addi		$v0, $0, 5		# system call #5 - input int
    syscall						# execute
    add $s1, $v0, $zero  # Armazenando expoente em $s1


    addi		$v0, $0, 4		# system call #4 - print string
    la		$a0, resultadoStr
    syscall						# execute

    # Chamando a function que calcula a potencia
    addi $v0, $zero, 1
    jal		potencia				# jump to potencia and save position to $ra

    add $a0, $zero, $v0
    addi		$v0, $0, 1		# system call #1 - print int
    syscall						# execute

    addi	$v0, $0, 10		# System call 10 - Exit
    syscall					# execute

potencia:
    # Salva o contexto
    addi $sp, $sp, -8
    sw $ra, 0($sp)
    sw $s1, 4($sp)

    # Caso base: expoente == 0
    beq $s1, $zero, end_potencia

    subi $s1, $s1, 1
    jal potencia

    # Calcula a potencia    
    mul $v0, $s0, $v0

end_potencia:
    # Restaura o contexto
    lw $ra, 0($sp)
    lw $s1, 4($sp)
    addi $sp, $sp, 8

    jr $ra