#ifndef NAME_H
#define NAME_H

#include <stdarg.h>
#include <stdlib.h>

struct NAME {
        size_t  a_cap;
        size_t  a_len;
        TYPE    *a_arr;
};

extern struct NAME *NAME_new(size_t cap, size_t argc, ...);

extern void NAME_free(struct NAME **app);

extern int NAME_add_back(struct NAME *ap, TYPE elem);

extern void NAME_qsort(struct NAME *ap, int (*cmp)(const void *, const void *));

extern struct NAME *NAME_copy(struct NAME *ap);

#endif
