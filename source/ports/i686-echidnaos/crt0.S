    .section .text
    .globl _start
_start:
    xor  %ebp, %ebp // Mark the end of stack frames
    push $0         // envp which we dont have :)
    push $0x1010    // Argv from hardcoded memory
    push (0x1000)   // Argc from hardcoded memory
    xor  %eax, %eax // Per ABI and compatibility with icc
    call main       // Jump to main!
    add  $12, %esp  // Pop main's arguments
    push %eax       // Main return to exit argument
    call exit       // Terminate the program
