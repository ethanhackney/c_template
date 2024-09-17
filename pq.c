#include "NAME.h"

struct NAME *
NAME_new(void)
{
        struct NAME *pq = malloc(sizeof(*pq));

        if (!pq)
                return NULL;
        
        pq->cap = 1;
        pq->arr = malloc(sizeof(TYPE) * pq->cap);
        if (!pq->arr) {
                free(pq);
                return NULL;
        }

        pq->len = 0;
        return pq;
}

void
NAME_free(struct NAME **pqpp)
{
        free((*pqpp)->arr);
        free(*pqpp);
        *pqpp = NULL;
}

static int add_back(struct NAME *pq, TYPE v);
static size_t parent(size_t i);
static void swap(TYPE *a, TYPE *b);

int
NAME_add(struct NAME *pq, TYPE v)
{
        size_t i;

        if (add_back(pq, v))
                return -1;

        i = pq->len - 1;
        while (i > 0 && CMP_FN(pq->arr[parent(i)], pq->arr[i]) > 0) {
                swap(&pq->arr[parent(i)], &pq->arr[i]);
                i = parent(i);
        }

        return 0;
}

static int
add_back(struct NAME *pq, TYPE v)
{
        if (pq->len == pq->cap) {
                TYPE *arr = pq->arr;
                size_t cap = pq->cap * 2;
                arr = realloc(arr, sizeof(TYPE) * cap);
                if (!arr)
                        return -1;
                pq->arr = arr;
                pq->cap = cap;
        }
        pq->arr[pq->len++] = v;
        return 0;
}

static size_t
parent(size_t i)
{
        return (i - 1) / 2;
}

static void
swap(TYPE *a, TYPE *b)
{
        TYPE tmp = *a;
        *a = *b;
        *b = tmp;
}

TYPE
NAME_top(struct NAME *pq)
{
        return pq->arr[0];
}

int
NAME_empty(struct NAME *pq)
{
        return pq->len == 0;
}

static size_t left(size_t i);
static size_t right(size_t i);

int
NAME_rm(struct NAME *pq)
{
        size_t i;

        pq->arr[0] = pq->arr[--pq->len];
        i = 0;
        for (;;) {
                size_t l = left(i);
                size_t r = right(i);
                size_t min = i;

                if (l < pq->len && CMP_FN(pq->arr[l], pq->arr[i]) < 0)
                        min = l;
                if (r < pq->len && CMP_FN(pq->arr[r], pq->arr[min]) < 0)
                        min = r;
                if (min == i)
                        break;

                swap(&pq->arr[i], &pq->arr[min]);
                i = min;
        }

        return 0;
}

static size_t
left(size_t i)
{
        return (i * 2) + 1;
}

static size_t
right(size_t i)
{
        return (i * 2) + 2;
}
