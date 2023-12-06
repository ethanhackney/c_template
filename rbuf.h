#ifndef NAME_H
#define NAME_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

struct NAME {
        size_t  b_cap;
        size_t  b_len;
        size_t  b_front;
        size_t  b_back;
        TYPE    *b_arr;
};

extern struct NAME *NAME_capped_new(size_t cap);

extern void NAME_free(struct NAME **bpp);

extern int NAME_add_front(struct NAME *bp, TYPE elem);

extern int NAME_add_back(struct NAME *bp, TYPE elem);

extern int NAME_rm_front(struct NAME *bp);

extern int NAME_rm_back(struct NAME *bp);

extern TYPE *NAME_front(struct NAME *bp);

extern TYPE *NAME_back(struct NAME *bp);

extern void NAME_for_each(struct NAME *bp, void (*fn)(TYPE *));

extern bool NAME_empty(struct NAME *bp);

#endif
