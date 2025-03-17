# Mysterious Cipher (30 Points)(E)

---

This challenge is designed to test your ability to analyze and reverse engineer assembly code while understanding hidden computations.

```nasm
    .section .data
ciphertext:
    .asciz "ddbw"         # Hardcoded ciphertext (encrypted message)
num1:
    .long 2
num2:
    .long -5

    .section .text
    .globl main
main:
    # Load the address of ciphertext into %rdi for processing.
    lea ciphertext(%rip), %rdi
    movl num1(%rip), %eax
    movl num2(%rip), %ecx
    addl %ecx, %eax
    movb %al, %bl

decrypt_loop:
    movzbq (%rdi), %rax
    testb %al, %al
    je decrypt_done
    subb %bl, %al
    movb %al, (%rdi)
    inc %rdi
    jmp decrypt_loop

decrypt_done:
    # Reload the address of ciphertext to print the decrypted string.
    lea ciphertext(%rip), %rdi
    call puts             # Call external function 'puts' to display the message.

    # Exit the program.
    movl $0, %eax
    ret
```

## Instructions

---

1. **Determine the Key:**
    - Examine the data section and the key-computation instructions.
    - Explain how the key is computed and what its final value is.
2. **Recover the Plaintext:**
    - The ciphertext stored in the data section is `ddbw`.
    - Work out what each character becomes after decryption, and determine the original plaintext message.
3. **Submit a single pdf file:**
    - Named `secret-[Your Student ID].pdf` (e.g., `secret-6XXXXXX.pdf`)
    - explain everything you learned/discovered from this assembly code and how you solved the mystery !!!
    - Submit by pushing the pdf file to the corresponding folder on your GitHub repository.

## **Grading Criteria**

---

- **Correctness (40%):** Accurate identification of the key and correct plaintext recovery.
- **Explanation (40%):** Clear reasoning and detailed reference to the assembly instructions.
- **Clarity (20%):** Well-organized, easy-to-follow report.
