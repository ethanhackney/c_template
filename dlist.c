#include "NAME.h"

struct NAME *
NAME_new(void)
{
        struct NAME *lp = malloc(sizeof(*lp));

        if (lp == NULL)
                return NULL;

        lp->l_head.n_next = &lp->l_head;
        lp->l_head.n_prev = &lp->l_head;
        return lp;
}

void
NAME_free(struct NAME **lpp)
{
        struct NAME_node *p;
        struct NAME_node *next;

        for (p = (*lpp)->l_head.n_next; p != &(*lpp)->l_head; p = next) {
                next = p->n_next;
                free(p);
        }

        free(*lpp);
        *lpp = NULL;
}

int
NAME_add_front(struct NAME *lp, TYPE elem)
{
        struct NAME_node *newln;
        struct NAME_node *head;

        newln = malloc(sizeof(*newln));
        if (newln == NULL)
                return -1;
        newln->n_elem = elem;

        head = &lp->l_head;
        newln->n_next = head->n_next;
        newln->n_prev = head;
        head->n_next->n_prev = newln;
        head->n_next = newln;
        return 0;
}

int
NAME_add_back(struct NAME *lp, TYPE elem)
{
        struct NAME_node *newln;
        struct NAME_node *head;

        newln = malloc(sizeof(*newln));
        if (newln == NULL)
                return -1;
        newln->n_elem = elem;

        head = &lp->l_head;
        newln->n_next = head;
        newln->n_prev = head->n_prev;
        head->n_prev->n_next = newln;
        head->n_prev = newln;
        return 0;
}

int
NAME_rm_front(struct NAME *lp)
{
        struct NAME_node *p = lp->l_head.n_next;

        p->n_next->n_prev = p->n_prev;
        p->n_prev->n_next = p->n_next;
        free(p);
        return 0;
}

int
NAME_rm_back(struct NAME *lp)
{
        struct NAME_node *p = lp->l_head.n_prev;

        p->n_next->n_prev = p->n_prev;
        p->n_prev->n_next = p->n_next;
        free(p);
        return 0;
}

TYPE *
NAME_front(struct NAME *lp)
{
        return &lp->l_head.n_next->n_elem;
}

TYPE *NAME_back(struct NAME *lp)
{
        return &lp->l_head.n_prev->n_elem;
}

bool
NAME_empty(struct NAME *lp)
{
        return lp->l_head.n_next == &lp->l_head;
}

void
NAME_for_each(struct NAME *lp, void (*fn)(TYPE *))
{
        struct NAME_node *p;

        for (p = lp->l_head.n_next; p != &lp->l_head; p = p->n_next)
                fn(&p->n_elem);
}
