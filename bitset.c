#include "NAME.h"

struct NAME *
NAME_new(void)
{
        struct NAME *set;

        set = malloc(sizeof(*set));
        if (!set)
                return NULL;

        set->b_size = 1;
        set->b_set = calloc(set->b_size, sizeof(TYPE));
        if (!set->b_set) {
                free(set);
                return NULL;
        }

        return set;
}

void
NAME_free(struct NAME **setp)
{
        free((*setp)->b_set);
        free(*setp);
        *setp = NULL;
}

int
NAME_set(struct NAME *set, TYPE bit)
{
        TYPE idx = bit / (TYPE)BITS;
        TYPE idxbit;

        if (idx >= set->b_size) {
                TYPE newsize = idx + 1;
                TYPE *bset = set->b_set;

                bset = realloc(bset, sizeof(*bset) * newsize);
                if (!bset)
                        return -1;

                memset(bset + set->b_size, 0, (newsize - set->b_size));
                set->b_set = bset;
                set->b_size = newsize;
        }

        idxbit = bit % (TYPE)BITS;
        set->b_set[idx] |= ((TYPE)1 << idxbit);
        return 0;
}

int
NAME_unset(struct NAME *set, TYPE bit)
{
        TYPE idx;
        TYPE idxbit;

        if (NAME_set(set, bit) < 0)
                return -1;

        idx = bit / (TYPE)BITS;
        idxbit = bit % (TYPE)BITS;
        set->b_set[idx] &= ~((TYPE)1 << idxbit);
        return 0;
}

int
NAME_isset(struct NAME *set, TYPE bit)
{
        TYPE idx = bit / (TYPE)BITS;
        TYPE idxbit = bit % (TYPE)BITS;

        return set->b_set[idx] & ((TYPE)1 << idxbit);
}
