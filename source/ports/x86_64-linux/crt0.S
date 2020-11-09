    .section .text
    .globl _start
_start:
    xor  %ebp, %ebp            // Mark the end of stack frames
    mov  (%rsp), %edi          // Get argc from the stack
    lea  8(%rsp), %rsi         // Take the address of argv from the stack
    lea  16(%rsp,%rdi,8), %rdx // Take the address of envp from the stack
    xor  %eax, %eax            // Per ABI and compatibility with icc
    call main                  // Jump to main!
    mov  %eax, %edi            // Main return to exit argument
    xor  %eax, %eax            // Per ABI and compatibility with icc
    call exit                  // Terminate the program
