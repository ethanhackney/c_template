#ifndef NAME_H
#define NAME_H

#include <stdbool.h>
#include <stdlib.h>

extern int KEY_CMP(const KEY_T akey, const KEY_T bkey);
extern size_t HASH_FN(const KEY_T key);

struct NAME_pair {
        KEY_T   p_key;
        VALUE_T p_value;
};

struct NAME_link {
        struct NAME_link        *ln_next;
        size_t                  ln_hash;
        struct NAME_pair        ln_pair;
};

struct NAME {
        size_t                  h_cap;
        size_t                  h_len;
        struct NAME_link        **h_tab;
};

extern struct NAME *NAME_capped_new(size_t cap);

extern struct NAME_pair *NAME_get(struct NAME *hp, KEY_T key);

extern struct NAME_pair *NAME_set(struct NAME *hp, KEY_T key, VALUE_T value);

extern void NAME_free(struct NAME **hpp);

extern void NAME_for_each(struct NAME *hp, void (*fn)(struct NAME_pair *));

#endif
