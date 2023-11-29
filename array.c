#include "NAME.h"

struct NAME *
NAME_new(size_t cap, size_t argc, ...)
{
        struct NAME *ap = malloc(sizeof(*ap));
        va_list va;
        size_t i;

        if (ap == NULL)
                return NULL;

        if (argc > cap)
                cap = argc;
        if (cap == 0)
                cap = 1;
        ap->a_cap = cap;

        ap->a_arr = malloc(sizeof(TYPE) * ap->a_cap);
        if (ap->a_arr == NULL) {
                free(ap);
                return NULL;
        }

        va_start(va, argc);
        for (i = 0; i < argc; i++)
                ap->a_arr[i] = va_arg(va, TYPE);
        va_end(va);
        ap->a_len = argc;

        return ap;
}

void
NAME_free(struct NAME **app)
{
        struct NAME *ap = *app;

        free(ap->a_arr);
        ap->a_arr = NULL;
        free(ap);
        *app = NULL;
}

int
NAME_add_back(struct NAME *ap, TYPE elem)
{
        if (ap->a_len == ap->a_cap) {
                TYPE *arr = ap->a_arr;
                size_t cap = ap->a_cap * 2;
                arr = realloc(arr, sizeof(TYPE) * cap);
                if (arr == NULL)
                        return -1;
                ap->a_arr = arr;
                ap->a_cap = cap;
        }
        ap->a_arr[ap->a_len++] = elem;
        return 0;
}

void
NAME_qsort(struct NAME *ap, int (*cmp)(const void *, const void *))
{
        qsort(ap->a_arr, ap->a_len, sizeof(TYPE), cmp);
}

struct NAME *
NAME_copy(struct NAME *ap)
{
        struct NAME *cpy = NAME_new(ap->a_len, 0);
        size_t i;

        if (cpy == NULL)
                return NULL;

        for (i = 0; i < ap->a_len; i++)
                cpy->a_arr[i] = ap->a_arr[i];

        cpy->a_len = ap->a_len;
        return cpy;
}
