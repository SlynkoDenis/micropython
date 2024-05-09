#include "helpers.h"

ssize_t fdputs(const char *s, int fd) {
    size_t len = strlen(s);
    if (len == 0) {
        return 0;
    }
    return write(fd, s, len);
}

char *fdgets(char *s, int n, int fd) {
    ssize_t num_read = readn(fd, s, n);
    if (num_read < 0) {
        return NULL;
    }
    return s + num_read;
}

ssize_t getrandom(void *buffer, size_t length) {
    int *int_buffer = (int *)buffer;

    size_t reminder = length % sizeof(int);
    size_t ints_count = length / sizeof(int);
    size_t i = 0;
    for (; i < ints_count; ++i) {
        int_buffer[i] = rand();
    }

    char *char_buffer = (char *)(int_buffer + i);
    union {
        int r;
        char ch[sizeof(int)];
    } r;
    r.r = rand();
    for (i = 0; i < reminder; ++i) {
        char_buffer[i] = r.ch[i];
    }

    return length;
}

/* fill in functions for libc and libm */
void __assert_fail(const char *__assertion, const char *__file, unsigned int __line, const char *__function) {
    cprintf("Assert at %s:%d:%s() \"%s\" failed\n", __file, __line, __function, __assertion);
    exit(1);
}

void *__memcpy_chk(void *dest, const void *src, size_t len, size_t slen) {
    if (len > slen) {
        return NULL;
    }
    return memcpy(dest, src, len);
}

void __stack_chk_fail(void) {
    static bool failed_once;

    if (failed_once) {
        return;
    }
    failed_once = true;
    cprintf("Stack corruption detected !\n");
    assert(0);
}

// TODO: uncomment if floating-point registers are supported in QEMU.
// double fabs(double x) {
//     union {double f; uint64_t i;} u = {x};
//     u.i &= -1ULL/2;
//     return u.f;
// }

// #define NAN (__builtin_nanf (""))

// double nan(const char *s) {
//     return NAN;
// }


// assumes endptr != NULL
// doesn't check for sign
// doesn't check for base-prefix
long long int strtoll(const char *nptr, char **endptr, int base) {
    long long val = 0;

    for (; *nptr; nptr++) {
        int v = *nptr;
        if ('0' <= v && v <= '9') {
            v -= '0';
        } else if ('A' <= v && v <= 'Z') {
            v -= 'A' - 10;
        } else if ('a' <= v && v <= 'z') {
            v -= 'a' - 10;
        } else {
            break;
        }
        if (v >= base) {
            break;
        }
        val = val * base + v;
    }

    *endptr = (char *)nptr;

    return val;
}
