#include "NAME.h"

struct NAME *
NAME_new(void)
{
        struct NAME *rp = malloc(sizeof(*rp));
        struct NAME_node *nil = NULL;

        if (rp == NULL)
                return NULL;

        nil = &rp->r_nil;
        nil->n_left = nil->n_right = nil->n_parent = nil;
        nil->n_color = NAME_BLACK;

        rp->r_root = nil;
        return rp;
}

static void rb_free(struct NAME *rp, struct NAME_node **npp);

void
NAME_free(struct NAME **rpp)
{
        rb_free(*rpp, &(*rpp)->r_root);
        free(*rpp);
        *rpp = NULL;
}

static void
rb_free(struct NAME *rp, struct NAME_node **npp)
{
        if (*npp == &rp->r_nil)
                return;
        rb_free(rp, &(*npp)->n_left);
        rb_free(rp, &(*npp)->n_right);
        free(*npp);
        *npp = NULL;
}

struct NAME_pair *
NAME_get(struct NAME *rp, KEY_T key)
{
        struct NAME_node *p = rp->r_root;

        while (p != &rp->r_nil) {
                int diff = KEY_CMP(key, p->n_pair.p_key);
                if (diff == 0)
                        return &p->n_pair;
                else if (diff < 0)
                        p = p->n_left;
                else
                        p = p->n_right;
        }

        return NULL;
}

static void rb_fixup(struct NAME *rp, struct NAME_node *np);

struct NAME_pair *
NAME_set(struct NAME *rp, KEY_T key, VALUE_T value)
{
        struct NAME_node **p = &rp->r_root;
        struct NAME_node *parent = &rp->r_nil;
        struct NAME_node *newnode = NULL;

        while (*p != &rp->r_nil) {
                int diff = KEY_CMP(key, (*p)->n_pair.p_key);
                parent = *p;
                if (diff == 0) {
                        (*p)->n_pair.p_value = value;
                        return &(*p)->n_pair;
                } else if (diff < 0)
                        p = &(*p)->n_left;
                else
                        p = &(*p)->n_right;
        }

        newnode = malloc(sizeof(*newnode));
        if (newnode == NULL)
                return NULL;
        newnode->n_pair.p_key = key;
        newnode->n_pair.p_value = value;
        newnode->n_left = &rp->r_nil;
        newnode->n_right = &rp->r_nil;
        newnode->n_parent = parent;
        newnode->n_color = NAME_RED;
        *p = newnode;
        rb_fixup(rp, *p);
        return &newnode->n_pair;
}

static void rb_left_rotate(struct NAME *rp, struct NAME_node *np);
static void rb_right_rotate(struct NAME *rp, struct NAME_node *np);

static void
rb_fixup(struct NAME *rp, struct NAME_node *np)
{
        while (np->n_parent->n_color == NAME_RED) {
                struct NAME_node *dad = np->n_parent;
                struct NAME_node *grandpa = dad->n_parent;

                if (dad == grandpa->n_left) {
                        struct NAME_node *uncle = grandpa->n_right;

                        if (uncle->n_color == NAME_RED) {
                                dad->n_color = NAME_BLACK;
                                uncle->n_color = NAME_BLACK;
                                grandpa->n_color = NAME_RED;
                                np = grandpa;
                        } else {
                                if (np == dad->n_right) {
                                        np = dad;
                                        rb_left_rotate(rp, np);
                                        dad = np->n_parent;
                                        grandpa = dad->n_parent;
                                }
                                dad->n_color = NAME_BLACK;
                                grandpa->n_color = NAME_RED;
                                rb_right_rotate(rp, grandpa);
                        }
                } else {
                        struct NAME_node *uncle = grandpa->n_left;

                        if (uncle->n_color == NAME_RED) {
                                dad->n_color = NAME_BLACK;
                                uncle->n_color = NAME_BLACK;
                                grandpa->n_color = NAME_RED;
                                np = grandpa;
                        } else {
                                if (np == dad->n_left) {
                                        np = dad;
                                        rb_right_rotate(rp, np);
                                        dad = np->n_parent;
                                        grandpa = dad->n_parent;
                                }
                                dad->n_color = NAME_BLACK;
                                grandpa->n_color = NAME_RED;
                                rb_left_rotate(rp, grandpa);
                        }
                }
        }

        rp->r_root->n_color = NAME_BLACK;
}

static void
rb_left_rotate(struct NAME *rp, struct NAME_node *np)
{
        struct NAME_node *right = np->n_right;

        np->n_right = right->n_left;
        if (right->n_left != &rp->r_nil)
                right->n_left->n_parent = np;

        right->n_parent = np->n_parent;
        if (np->n_parent == &rp->r_nil)
                rp->r_root = right;
        else if (np == np->n_parent->n_left)
                np->n_parent->n_left = right;
        else
                np->n_parent->n_right = right;

        right->n_left = np;
        np->n_parent = right;
}

static void
rb_right_rotate(struct NAME *rp, struct NAME_node *np)
{
        struct NAME_node *left = np->n_left;

        np->n_left = left->n_right;
        if (left->n_right != &rp->r_nil)
                left->n_right->n_parent = np;

        left->n_parent = np->n_parent;
        if (np->n_parent == &rp->r_nil)
                rp->r_root = left;
        else if (np == np->n_parent->n_left)
                np->n_parent->n_left = left;
        else
                np->n_parent->n_right = left;

        left->n_right = np;
        np->n_parent = left;
}

static void rb_for_each(struct NAME *rp,
                        struct NAME_node *np,
                        void (*fn)(struct NAME_pair *));

void NAME_for_each(struct NAME *rp, void (*fn)(struct NAME_pair *))
{
        rb_for_each(rp, rp->r_root, fn);
}

static void
rb_for_each(struct NAME *rp, struct NAME_node *np, void (*fn)(struct NAME_pair *))
{
        if (np == &rp->r_nil)
                return;
        rb_for_each(rp, np->n_left, fn);
        fn(&np->n_pair);
        rb_for_each(rp, np->n_right, fn);
}
