#include "NAME.h"

#define NODE_OF(p) \
        ((struct NAME_node *)((char *)(p) - offsetof(struct NAME_node, n_pair)))

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

static void rb_for_each_preorder(struct NAME *rp,
                                 struct NAME_node *np,
                                 void (*fn)(struct NAME_pair *));

static void rb_for_each_inorder(struct NAME *rp,
                                struct NAME_node *np,
                                void (*fn)(struct NAME_pair *));

static void rb_for_each_postorder(struct NAME *rp,
                                  struct NAME_node *np,
                                  void (*fn)(struct NAME_pair *));

void NAME_for_each(struct NAME *rp, int type, void (*fn)(struct NAME_pair *))
{
        if (type == NAME_FOR_PREORDER)
                rb_for_each_preorder(rp, rp->r_root, fn);
        if (type == NAME_FOR_INORDER)
                rb_for_each_inorder(rp, rp->r_root, fn);
        if (type == NAME_FOR_POSTORDER)
                rb_for_each_postorder(rp, rp->r_root, fn);
}

static void
rb_for_each_preorder(struct NAME *rp,
                     struct NAME_node *np,
                     void (*fn)(struct NAME_pair *))
{
        if (np == &rp->r_nil)
                return;
        fn(&np->n_pair);
        rb_for_each_preorder(rp, np->n_left, fn);
        rb_for_each_preorder(rp, np->n_right, fn);
}

static void
rb_for_each_inorder(struct NAME *rp,
                    struct NAME_node *np,
                    void (*fn)(struct NAME_pair *))
{
        if (np == &rp->r_nil)
                return;
        rb_for_each_inorder(rp, np->n_left, fn);
        fn(&np->n_pair);
        rb_for_each_inorder(rp, np->n_right, fn);
}

static void
rb_for_each_postorder(struct NAME *rp,
                      struct NAME_node *np,
                      void (*fn)(struct NAME_pair *))
{
        if (np == &rp->r_nil)
                return;
        rb_for_each_postorder(rp, np->n_left, fn);
        rb_for_each_postorder(rp, np->n_right, fn);
        fn(&np->n_pair);
}

struct NAME_pair *
NAME_lca(struct NAME *rp, struct NAME_pair *ap, struct NAME_pair *bp)
{
        struct NAME_node *curr = rp->r_root;
        struct NAME_node *anode = NODE_OF(ap);
        struct NAME_node *bnode = NODE_OF(bp);
        KEY_T max;
        KEY_T min;
        int diff;

        diff = KEY_CMP(anode->n_pair.p_key, bnode->n_pair.p_key);
        if (diff < 0) {
                min = anode->n_pair.p_key;
                max = bnode->n_pair.p_key;
        } else {
                min = bnode->n_pair.p_key;
                max = anode->n_pair.p_key;
        }

        while (curr != &rp->r_nil) {
                if (KEY_CMP(curr->n_pair.p_key, max) > 0)
                        curr = curr->n_left;
                else if (KEY_CMP(curr->n_pair.p_key, min) < 0)
                        curr = curr->n_right;
                else
                        return &curr->n_pair;
        }

        return NULL;
}

static void rb_dump(struct NAME *rp,
                    struct NAME_node *np,
                    int space,
                    const char *fmt);

void
NAME_dump_tree(struct NAME *rp, const char *fmt)
{
        rb_dump(rp, rp->r_root, 0, fmt);
}

static void
rb_dump(struct NAME *rp, struct NAME_node *np, int space, const char *fmt)
{
        int i;

        if (np == &rp->r_nil)
                return;

        rb_dump(rp, np->n_left, space + 2, fmt);

        for (i = 0; i < space; i++)
                putchar(' ');

        printf(fmt, np->n_pair.p_key);

        rb_dump(rp, np->n_right, space + 2, fmt);
}

static struct NAME_node *rb_kth_smallest(struct NAME *rp,
                                         struct NAME_node *np,
                                         size_t *count,
                                         size_t k);

struct NAME_pair *
NAME_kth_smallest(struct NAME *rp, size_t k)
{
        struct NAME_node *kth;
        size_t count = 0;

        kth = rb_kth_smallest(rp, rp->r_root, &count, k);
        if (kth != NULL)
                return &kth->n_pair;
        else
                return NULL;
}

static struct NAME_node *
rb_kth_smallest(struct NAME *rp, struct NAME_node *np, size_t *count, size_t k)
{
        struct NAME_node *left;

        if (np == &rp->r_nil)
                return NULL;

        left = rb_kth_smallest(rp, np->n_left, count, k);

        if (left != NULL)
                return left;

        if (++(*count) == k)
                return np;

        return rb_kth_smallest(rp, np->n_right, count, k);
}

static struct NAME_node *rb_kth_largest(struct NAME *rp,
                                        struct NAME_node *np,
                                        size_t *count,
                                        size_t k);

struct NAME_pair *
NAME_kth_largest(struct NAME *rp, size_t k)
{
        struct NAME_node *p;
        size_t count = 0;

        p = rb_kth_largest(rp, rp->r_root, &count, k);
        if (p != NULL)
                return &p->n_pair;
        else
                return NULL;
}

static struct NAME_node *
rb_kth_largest(struct NAME *rp, struct NAME_node *np, size_t *count, size_t k)
{
        struct NAME_node *left;

        if (np == &rp->r_nil)
                return NULL;

        left = rb_kth_largest(rp, np->n_right, count, k);

        if (left != NULL)
                return left;

        if (++(*count) == k)
                return np;

        return rb_kth_largest(rp, np->n_left, count, k);
}

static size_t rb_nr_in_range(struct NAME *rp,
                             struct NAME_node *np,
                             KEY_T low,
                             KEY_T high);

size_t
NAME_nr_in_range(struct NAME *rp, KEY_T low, KEY_T high)
{
        return rb_nr_in_range(rp, rp->r_root, low, high);
}

static size_t rb_nr_in_range(struct NAME *rp,
                             struct NAME_node *np,
                             KEY_T low,
                             KEY_T high)
{
        size_t count = 0;

        if (np == &rp->r_nil)
                return 0;

        if (KEY_CMP(np->n_pair.p_key, low) >= 0 &&
            KEY_CMP(np->n_pair.p_key, high) <= 0)
                count++;

        count += rb_nr_in_range(rp, np->n_left, low, high);

        return count + rb_nr_in_range(rp, np->n_right, low, high);
}

struct NAME_pair *
NAME_min(struct NAME *rp)
{
        struct NAME_node *p = rp->r_root;

        if (p == &rp->r_nil)
                return NULL;

        while (p->n_left != &rp->r_nil)
                p = p->n_left;

        return &p->n_pair;
}

struct NAME_pair *
NAME_max(struct NAME *rp)
{
        struct NAME_node *p = rp->r_root;

        if (p == &rp->r_nil)
                return NULL;

        while (p->n_right != &rp->r_nil)
                p = p->n_right;

        return &p->n_pair;
}

static struct NAME_node *rb_max_at(struct NAME *rp, struct NAME_node *np);

struct NAME_pair *
NAME_inorder_pred(struct NAME *rp, struct NAME_pair *p)
{
        struct NAME_node *pre;
        struct NAME_node *cur;
        int diff;

        if (rp->r_root == &rp->r_nil)
                return NULL;

        pre = NULL;
        cur = rp->r_root;
        while (cur != &rp->r_nil) {
                diff = KEY_CMP(p->p_key, cur->n_pair.p_key);
                if (diff < 0) {
                        cur = cur->n_left;
                } else if (diff > 0) {
                        pre = cur;
                        cur = cur->n_right;
                } else {
                        if (cur->n_left != &rp->r_nil)
                                pre = rb_max_at(rp, cur->n_left);
                        break;
                }
        }

        if (pre)
                return &pre->n_pair;
        return NULL;
}

static struct NAME_node *
rb_max_at(struct NAME *rp, struct NAME_node *np)
{
        while (np->n_right != &rp->r_nil)
                np = np->n_right;
        return np;
}

struct NAME_pair *
NAME_floor(struct NAME *rp, KEY_T key)
{
        struct NAME_node *floor;
        struct NAME_node *cur;
        int diff;

        floor = NULL;
        cur = rp->r_root;
        while (cur != &rp->r_nil) {
                diff = KEY_CMP(key, cur->n_pair.p_key);
                if (diff == 0) {
                        return &cur->n_pair;
                } else if (diff < 0) {
                        cur = cur->n_left;
                } else {
                        floor = cur;
                        cur = cur->n_right;
                        
                }
        }

        if (floor)
                return &floor->n_pair;
        return NULL;
}

struct NAME_pair *
NAME_ceil(struct NAME *rp, KEY_T key)
{
        struct NAME_node *ceil;
        struct NAME_node *cur;
        int diff;

        ceil = NULL;
        cur = rp->r_root;
        while (cur != &rp->r_nil) {
                diff = KEY_CMP(key, cur->n_pair.p_key);
                if (diff == 0) {
                        return &cur->n_pair;
                } else if (diff < 0) {
                        ceil = cur;
                        cur = cur->n_left;
                } else {
                        cur = cur->n_right;
                        
                }
        }

        if (ceil)
                return &ceil->n_pair;
        return NULL;
}

static struct NAME_node *rb_min_at(struct NAME *rp, struct NAME_node *np);

struct NAME_pair *
NAME_inorder_succ(struct NAME *rp, struct NAME_pair *p)
{
        struct NAME_node *succ = NULL;
        struct NAME_node *cur = rp->r_root;
        int diff;

        while (cur != &rp->r_nil) {
                diff = KEY_CMP(p->p_key, cur->n_pair.p_key);
                if (diff == 0) {
                        if (cur->n_right != &rp->r_nil)
                                succ = rb_min_at(rp, cur->n_right);
                        break;
                } else if (diff < 0) {
                        succ = cur;
                        cur = cur->n_left;
                } else {
                        cur = cur->n_right;
                }
        }

        if (succ)
                return &succ->n_pair;
        return NULL;
}

static struct NAME_node *
rb_min_at(struct NAME *rp, struct NAME_node *np)
{
        while (np->n_left != &rp->r_nil)
                np = np->n_left;
        return np;
}
