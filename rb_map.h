#ifndef NAME_H
#define NAME_H

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

enum {
        NAME_BLACK,
        NAME_RED,
};

enum {
        NAME_FOR_PREORDER,
        NAME_FOR_INORDER,
        NAME_FOR_POSTORDER,
};

extern int KEY_CMP(const KEY_T, const KEY_T);

struct NAME_pair {
        KEY_T   p_key;
        VALUE_T p_value;
};

struct NAME_node {
        struct NAME_node        *n_left;
        struct NAME_node        *n_right;
        struct NAME_node        *n_parent;
        int                     n_color;
        struct NAME_pair        n_pair;
};

struct NAME {
        struct NAME_node        r_nil;
        struct NAME_node        *r_root;
};

extern struct NAME *NAME_new(void);

extern void NAME_free(struct NAME **rpp);

extern struct NAME_pair *NAME_get(struct NAME *rp, KEY_T key);

extern struct NAME_pair *NAME_set(struct NAME *rp, KEY_T key, VALUE_T value);

extern void NAME_for_each(struct NAME *rp,
                          int type,
                          void (*fn)(struct NAME_pair *));

extern struct NAME_pair *NAME_lca(struct NAME *rp,
                                  struct NAME_pair *ap,
                                  struct NAME_pair *bp);

extern void NAME_dump_tree(struct NAME *rp, const char *fmt);

extern struct NAME_pair *NAME_kth_smallest(struct NAME *rp, size_t k);

extern struct NAME_pair *NAME_kth_largest(struct NAME *rp, size_t k);

extern size_t NAME_nr_in_range(struct NAME *rp, KEY_T low, KEY_T high);

extern struct NAME_pair *NAME_min(struct NAME *rp);

extern struct NAME_pair *NAME_max(struct NAME *rp);

extern struct NAME_pair *NAME_inorder_pred(struct NAME *rp, struct NAME_pair *p);

extern struct NAME_pair *NAME_floor(struct NAME *rp, KEY_T key);

extern struct NAME_pair *NAME_ceil(struct NAME *rp, KEY_T key);

#endif
