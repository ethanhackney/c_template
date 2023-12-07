#ifndef NAME_H
#define NAME_H

#include <stdlib.h>
#include <stdbool.h>

struct NAME_node {
        struct NAME_node        *n_next;
        struct NAME_node        *n_prev;
        TYPE                    n_elem;
};

struct NAME {
        struct NAME_node        l_head;
};

extern struct NAME *NAME_new(void);

extern void NAME_free(struct NAME **lpp);

extern int NAME_add_front(struct NAME *lp, TYPE elem);

extern int NAME_add_back(struct NAME *lp, TYPE elem);

extern int NAME_rm_front(struct NAME *lp);

extern int NAME_rm_back(struct NAME *lp);

extern TYPE *NAME_front(struct NAME *lp);

extern TYPE *NAME_back(struct NAME *lp);

extern bool NAME_empty(struct NAME *lp);

extern void NAME_for_each(struct NAME *lp, void (*fn)(TYPE *));

#endif
