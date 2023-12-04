#include "NAME.h"

static size_t next_prime(size_t n);

struct NAME *
NAME_capped_new(size_t cap)
{
        struct NAME *hp = malloc(sizeof(*hp));

        if (hp == NULL)
                return NULL;

        hp->h_cap = next_prime(cap);
        hp->h_tab = calloc(hp->h_cap, sizeof(*hp->h_tab));
        if (hp->h_tab == NULL) {
                free(hp);
                return NULL;
        }

        hp->h_len = 0;
        return hp;
}

static bool is_prime(size_t n);

static size_t
next_prime(size_t n)
{
        while (!is_prime(n))
                n++;
        return n;
}

static bool
is_prime(size_t n)
{
        size_t i;

        if (n <= 1)
                return false;
        if (n <= 3)
                return true;
        if ((n % 2) == 0)
                return false;
        if ((n % 3) == 0)
                return false;

        for (i = 5; i * i <= n; i += 6) {
                if ((n % i) == 0)
                        return false;
                if ((n % (i + 2)) == 0)
                        return false;
        }

        return true;
}

struct NAME_pair *
NAME_get(struct NAME *hp, KEY_T key)
{
        size_t hash = HASH_FN(key);
        size_t idx = hash % hp->h_cap;
        struct NAME_link *p = hp->h_tab[idx];

        for (; p != NULL; p = p->ln_next) {
                if (KEY_CMP(key, p->ln_pair.p_key) == 0)
                        return &p->ln_pair;
        }

        return NULL;
}

static int hash_grow(struct NAME *hp);

struct NAME_pair *
NAME_set(struct NAME *hp, KEY_T key, VALUE_T value)
{
        size_t hash = HASH_FN(key);
        size_t idx = hash % hp->h_cap;
        struct NAME_link *p = hp->h_tab[idx];

        for (; p != NULL; p = p->ln_next) {
                if (KEY_CMP(key, p->ln_pair.p_key) == 0) {
                        p->ln_pair.p_value = value;
                        return &p->ln_pair;
                }
        }

        if (hp->h_len == hp->h_cap) {
                if (hash_grow(hp) < 0)
                        return NULL;
                idx = hash % hp->h_cap;
        }

        p = malloc(sizeof(*p));
        if (p == NULL)
                return NULL;
        p->ln_pair.p_key = key;
        p->ln_pair.p_value = value;
        p->ln_hash = hash;
        p->ln_next = hp->h_tab[idx];
        hp->h_tab[idx] = p;
        hp->h_len++;
        return &p->ln_pair;

}

static int
hash_grow(struct NAME *hp)
{
        size_t newcap = next_prime(hp->h_cap * 2);
        struct NAME_link **newtab = calloc(newcap, sizeof(*newtab));
        size_t moved = 0;
        size_t i = 0;

        if (newtab == NULL)
                return -1;

        for (; moved < hp->h_len; i++) {
                struct NAME_link *p = hp->h_tab[i];
                struct NAME_link *next;

                for (; p != NULL; p = next) {
                        size_t idx;

                        next = p->ln_next;
                        idx = p->ln_hash % newcap;
                        p->ln_next = newtab[idx];
                        newtab[idx] = p;
                        moved++;
                }
        }

        free(hp->h_tab);
        hp->h_tab = newtab;
        hp->h_cap = newcap;
        return 0;
}

extern void
NAME_free(struct NAME **hpp)
{
        struct NAME *hp = *hpp;
        size_t freed = 0;
        size_t i = 0;

        for (; freed < hp->h_len; i++) {
                struct NAME_link *p = hp->h_tab[i];
                struct NAME_link *next = NULL;

                for (; p != NULL; p = next) {
                        next = p->ln_next;
                        free(p);
                        freed++;
                }
        }

        free(hp->h_tab);
        free(hp);
        *hpp = NULL;
}

void
NAME_for_each(struct NAME *hp, void (*fn)(struct NAME_pair *))
{
        size_t visited = 0;
        size_t i = 0;

        for (; visited < hp->h_len; i++) {
                struct NAME_link *p = hp->h_tab[i];

                for (; p != NULL; p = p->ln_next) {
                        fn(&p->ln_pair);
                        visited++;
                }
        }
}
