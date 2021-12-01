#include <setjmp.h>
#include <stdnoreturn.h>

extern jmp_buf JMP_BUF;

_Noreturn void longjump_test(jmp_buf env, const int value);
