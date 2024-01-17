#include "NAME.h"
#include <stdlib.h>

struct NAME *
NAME_new(void)
{
        struct NAME *lp = NULL;

        lp = malloc(sizeof(*lp));
        if (!lp)
                return NULL;

        lp->head = NULL;
        return lp;
}

void
NAME_free(struct NAME **lpp)
{
        struct NAME_link *cur = NULL;
        struct NAME_link *next = NULL;

        for (cur = (*lpp)->head; cur; cur = next) {
                next = cur->next;
                free(cur);
        }

        free(*lpp);
        *lpp = NULL;
}

int
NAME_add(struct NAME *lp, TYPE elem)
{
        struct NAME_link **p = NULL;
        struct NAME_link *newln = NULL;

        newln = malloc(sizeof(*newln));
        if (!newln)
                return -1;
        newln->elem = elem;

        for (p = &lp->head; *p; p = &(*p)->next) {
                if (CMP_FN(elem, (*p)->elem) <= 0)
                        break;
        }

        newln->next = *p;
        *p = newln;
        return 0;
}

void
NAME_for_each(struct NAME *lp, void (*fn)(TYPE))
{
        struct NAME_link *p = NULL;

        for (p = lp->head; p; p = p->next)
                fn(p->elem);
}
