#ifndef NAME_H
#define NAME_H

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct NAME {
        TYPE    b_size;
        TYPE    *b_set;
};

extern struct NAME *NAME_new(void);
extern void NAME_free(struct NAME **setp);
extern int NAME_set(struct NAME *set, TYPE bit);
extern int NAME_unset(struct NAME *set, TYPE bit);
extern int NAME_isset(struct NAME *set, TYPE bit);

#endif
