#include <setjmp.h>

#ifdef __x86_64__
    __attribute__((naked)) void longjmp(jmp_buf state, int value) {
        (void)state;
        (void)value;
        asm volatile(
            "movq  %rsi, %rax\n"
            "testq %rax, %rax\n"
            "setz  %al\n"
            "movq  0(%rdi),  %rbx\n"
            "movq  8(%rdi),  %rbp\n"
            "movq  16(%rdi), %r12\n"
            "movq  24(%rdi), %r13\n"
            "movq  32(%rdi), %r14\n"
            "movq  40(%rdi), %r15\n"
            "movq  48(%rdi), %rsp\n"
            "jmp  *56(%rdi)\n"
        );
    }

    __attribute__((naked)) int setjmp(jmp_buf state) {
        (void)state;
        asm volatile(
            "movq %rbx, 0(%rdi)\n"
            "movq %rbp, 8(%rdi)\n"
            "movq %r12, 16(%rdi)\n"
            "movq %r13, 24(%rdi)\n"
            "movq %r14, 32(%rdi)\n"
            "movq %r15, 40(%rdi)\n"
            "leaq 8(%rsp), %rdx\n"
            "movq %rdx, 48(%rdi)\n"
            "movq (%rsp), %rdx\n"
            "movq %rdx, 56(%rdi)\n"
            "movq $0, %rax\n"
            "ret\n"
        );
    }
#elif __i386__
    __attribute__((naked)) void longjmp(jmp_buf state, int value) {
        asm volatile("");
    }

    __attribute__((naked)) int setjmp(jmp_buf state) {
        asm volatile("");
    }
#endif
