#include "NAME.h"

struct NAME *
NAME_capped_new(size_t cap)
{
        struct NAME *bp = malloc(sizeof(*bp));

        if (bp == NULL)
                return NULL;

        bp->b_cap = cap;
        if (bp->b_cap == 0)
                bp->b_cap = 1;

        bp->b_arr = malloc(sizeof(TYPE) * bp->b_cap);
        if (bp->b_arr == NULL) {
                free(bp);
                return NULL;
        }

        bp->b_front = 0;
        bp->b_back = 0;
        bp->b_len = 0;
        return bp;
}

void
NAME_free(struct NAME **bpp)
{
        free((*bpp)->b_arr);
        (*bpp)->b_arr = NULL;
        free(*bpp);
        *bpp = NULL;
}

static bool rbuf_full(struct NAME *bp);
static int rbuf_grow(struct NAME *bp);

int
NAME_add_front(struct NAME *bp, TYPE elem)
{
        if (rbuf_full(bp) && rbuf_grow(bp) < 0)
                return -1;

        if (bp->b_front == 0)
                bp->b_front = bp->b_cap - 1;
        else
                bp->b_front--;

        bp->b_arr[bp->b_front] = elem;
        bp->b_len++;
        return 0;
}

static bool
rbuf_full(struct NAME *bp)
{
        return bp->b_len == bp->b_cap;
}

static int
rbuf_grow(struct NAME *bp)
{
        TYPE *newarr;
        TYPE *arr = bp->b_arr;
        size_t newcap = bp->b_cap * 2;
        size_t copied;
        size_t i;
        size_t j;

        newarr = malloc(sizeof(TYPE) * newcap);
        if (newarr == NULL)
                return -1;

        copied = 0;
        i = bp->b_front;
        j = 0;
        for (copied = 0; copied < bp->b_len; copied++) {
                newarr[j++] = bp->b_arr[i];
                i = (i + 1) % bp->b_cap;
        }

        free(arr);
        bp->b_arr = newarr;
        bp->b_cap = newcap;
        bp->b_front = 0;
        bp->b_back = bp->b_len;
        return 0;
}


int
NAME_add_back(struct NAME *bp, TYPE elem)
{
        if (rbuf_full(bp) && rbuf_grow(bp) < 0)
                return -1;

        bp->b_arr[bp->b_back] = elem;
        bp->b_back = (bp->b_back + 1) % bp->b_cap;
        bp->b_len++;
        return 0;
}

int
NAME_rm_front(struct NAME *bp)
{
        /* TODO: implement shrinking */
        bp->b_front = (bp->b_front + 1) % bp->b_cap;
        bp->b_len--;
        return 0;
}

int
NAME_rm_back(struct NAME *bp)
{
        /* TODO: implement shrinking */
        if (bp->b_back == 0)
                bp->b_back = bp->b_cap - 1;
        else
                bp->b_back--;
        bp->b_len--;
        return 0;
}

TYPE *
NAME_front(struct NAME *bp)
{
        if (bp->b_front == bp->b_cap)
                return &bp->b_arr[0];
        else
                return &bp->b_arr[bp->b_front];
}

TYPE *
NAME_back(struct NAME *bp)
{
        if (bp->b_back == 0)
                return &bp->b_arr[bp->b_cap - 1];
        else
                return &bp->b_arr[bp->b_back];
}

void
NAME_for_each(struct NAME *bp, void (*fn)(TYPE *))
{
        size_t visited;
        size_t i;

        i = bp->b_front;
        for (visited = 0; visited < bp->b_len; visited++) {
                fn(&bp->b_arr[i]);
                i = (i + 1) % bp->b_cap;
        }
}

bool
NAME_empty(struct NAME *bp)
{
        return bp->b_len == 0;
}
