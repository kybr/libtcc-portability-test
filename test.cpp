#include <cstdlib>
#include <iostream>
#include <cassert>

#include "libtcc.h"

void handle_error(void *opaque, const char *msg) {
    fprintf(reinterpret_cast<FILE*>(opaque), "%s\n", msg);
}

/* this function is called by the generated code */
int add(int a, int b)
{
    return a + b;
}

/* this strinc is referenced by the generated code */
const char hello[] = "Hello World!";

char my_program[] = R"(
#include <tcclib.h>
extern int add(int a, int b);
#ifdef _WIN32
 __attribute__((dllimport))
#endif
extern const char hello[];
int fib(int n) {
    if (n <= 2)
        return 1;
    else
        return fib(n-1) + fib(n-2);
}

int foo(int n) {
    printf("%s\n", hello);
    printf("fib(%d) = %d\n", n, fib(n));
    printf("add(%d, %d) = %d\n", n, 2 * n, add(n, 2 * n));
    return add(fib(n), 2 * fib(2));
}
)";

int main(int argc, char **argv) {
    TCCState *s;
    int i;
    int (*func)(int);

    s = tcc_new();
    if (!s) {
        fprintf(stderr, "Could not create tcc state\n");
        exit(1);
    }

    assert(tcc_get_error_func(s) == NULL);
    assert(tcc_get_error_opaque(s) == NULL);

    tcc_set_error_func(s, stderr, handle_error);

    assert(tcc_get_error_func(s) == handle_error);
    assert(tcc_get_error_opaque(s) == stderr);

    /* if tcclib.h and libtcc1.a are not installed, where can we find them */
    for (i = 1; i < argc; ++i) {
        char *a = argv[i];
        if (a[0] == '-') {
            if (a[1] == 'B')
                tcc_set_lib_path(s, a+2);
            else if (a[1] == 'I')
                tcc_add_include_path(s, a+2);
            else if (a[1] == 'L')
                tcc_add_library_path(s, a+2);
        }
    }

    /* MUST BE CALLED before any compilation */
    tcc_set_output_type(s, TCC_OUTPUT_MEMORY);

    if (tcc_compile_string(s, my_program) == -1)
        return 1;

    /* as a test, we add symbols that the compiled program can use.
       You may also open a dll with tcc_add_dll() and use symbols from that */
    tcc_add_symbol(s, "add", (const void*)add);
    tcc_add_symbol(s, "hello", hello);
    //candidate function not viable: no known conversion from 'int (int, int)' to 'const void *' for 3rd argument

    /* relocate the code */
    if (tcc_relocate(s, TCC_RELOCATE_AUTO) < 0)
        return 1;

    /* get entry symbol */
    func = reinterpret_cast<int(__cdecl*)(int)>(tcc_get_symbol(s, "foo"));
    if (!func)
        return 1;

    /* run the code */
    func(32);

    /* delete the state */
    tcc_delete(s);

    return 0;
}
