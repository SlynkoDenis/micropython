#ifndef MICROPY_INCLUDED_JOS_HELPERS_H
#define MICROPY_INCLUDED_JOS_HELPERS_H

#include <assert.h>
#include <lib.h>
#include <random.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <types.h>

#define SEEK_SET	0	/* Seek from beginning of file.  */
#define SEEK_CUR	1	/* Seek from current position.  */
#define SEEK_END	2	/* Seek from end of file.  */

ssize_t fdputs(const char *s, int fd);
char *fdgets(char *s, int n, int fd);

ssize_t getrandom(void *buffer, size_t length);

long long int strtoll(const char *nptr, char **endptr, int base);

// Math lib implementations.
// double round(double);
// double modf(double x, double *iptr);
// double trunc(double x);
// double fabs(double);

#endif // MICROPY_INCLUDED_JOS_HELPERS_H
