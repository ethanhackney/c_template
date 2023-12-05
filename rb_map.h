#ifndef NAME_H
#define NAME_H

#include <stdlib.h>

enum {
        NAME_BLACK,
        NAME_RED,
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

extern void NAME_for_each(struct NAME *rp, void (*fn)(struct NAME_pair *));

#endif
