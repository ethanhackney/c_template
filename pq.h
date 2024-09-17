#ifndef NAME_H
#define NAME_H

#include <stdlib.h>

struct NAME {
        size_t cap;
        size_t len;
        TYPE *arr;
};

extern int CMP_FN(const TYPE, const TYPE);

struct NAME *NAME_new(void);
void NAME_free(struct NAME **pqpp);
int NAME_add(struct NAME *pq, TYPE v);
TYPE NAME_top(struct NAME *pq);
int NAME_empty(struct NAME *pq);
int NAME_rm(struct NAME *pq);

#endif
