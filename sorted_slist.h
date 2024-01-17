#ifndef NAME_H
#define NAME_H

extern int CMP_FN(const TYPE, const TYPE);

struct NAME_link {
        struct NAME_link        *next;
        TYPE                    elem;
};

struct NAME {
        struct NAME_link        *head;
};

extern struct NAME *NAME_new(void);

extern void NAME_free(struct NAME **lpp);

extern int NAME_add(struct NAME *lp, TYPE elem);

extern void NAME_for_each(struct NAME *lp, void (*fn)(TYPE));

#endif
