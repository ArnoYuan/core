#ifndef __PI_LISTS_H__
#define __PI_LISTS_H__
#include <common/pidef.h>

#ifdef __cplusplus
extern "C"{
#endif


#define PI_LIST_OBJECT_INIT(object) { &(object), &(object) }


pi_inline void pi_list_init(pi_list_t *l)
{
    l->next = l->prev = l;
}


pi_inline void pi_list_insert_after(pi_list_t *l, pi_list_t *n)
{
    l->next->prev = n;
    n->next = l->next;

    l->next = n;
    n->prev = l;
}


pi_inline void pi_list_insert_before(pi_list_t *l, pi_list_t *n)
{
    l->prev->next = n;
    n->prev = l->prev;

    l->prev = n;
    n->next = l;
}


pi_inline void pi_list_remove(pi_list_t *n)
{
    n->next->prev = n->prev;
    n->prev->next = n->next;

    n->next = n->prev = n;
}


pi_inline int pi_list_isempty(const pi_list_t *l)
{
    return l->next == l;
}


pi_inline unsigned int pi_list_len(const pi_list_t *l)
{
    unsigned int len = 0;
    const pi_list_t *p = l;
    while (p->next != l)
    {
        p = p->next;
        len ++;
    }

    return len;
}


#define pi_list_entry(node, type, member) \
    pi_container_of(node, type, member)


#define pi_list_for_each_entry(pos, head, member) \
    for (pos = pi_list_entry((head)->next, typeof(*pos), member); \
         &pos->member != (head); \
         pos = pi_list_entry(pos->member.next, typeof(*pos), member))


#define pi_list_for_each_entry_safe(pos, n, head, member) \
    for (pos = pi_list_entry((head)->next, typeof(*pos), member), \
         n = pi_list_entry(pos->member.next, typeof(*pos), member); \
         &pos->member != (head); \
         pos = n, n = pi_list_entry(n->member.next, typeof(*n), member))


#define pi_list_first_entry(ptr, type, member) \
    pi_list_entry((ptr)->next, type, member)

#define PI_SLIST_OBJECT_INIT(object) { PI_NULL }


pi_inline void pi_slist_init(pi_slist_t *l)
{
    l->next = PI_NULL;
}

pi_inline void pi_slist_append(pi_slist_t *l, pi_slist_t *n)
{
    struct pi_slist_node *node;

    node = l;
    while (node->next) node = node->next;

    /* append the node to the tail */
    node->next = n;
    n->next = PI_NULL;
}

pi_inline void pi_slist_insert(pi_slist_t *l, pi_slist_t *n)
{
    n->next = l->next;
    l->next = n;
}

pi_inline unsigned int pi_slist_len(const pi_slist_t *l)
{
    unsigned int len = 0;
    const pi_slist_t *list = l->next;
    while (list != PI_NULL)
    {
        list = list->next;
        len ++;
    }

    return len;
}

pi_inline pi_slist_t *pi_slist_remove(pi_slist_t *l, pi_slist_t *n)
{
    /* remove slist head */
    struct pi_slist_node *node = l;
    while (node->next && node->next != n) node = node->next;

    /* remove node */
    if (node->next != (pi_slist_t *)0) node->next = node->next->next;

    return l;
}

pi_inline int pi_slist_isempty(pi_slist_t *l)
{
    return l->next == PI_NULL;
}


#define pi_slist_entry(node, type, member) \
    pi_container_of(node, type, member)


#define pi_slist_fore_each_entry(pos, head, member) \
    for (pos = pi_slist_entry((head)->next, typeof(*pos), member); \
         &pos->member != (PI_NULL); \
         pos = pi_slist_entry(pos->member.next, typeof(*pos), member))


#define pi_slist_first_entry(ptr, type, member) \
    pi_slist_entry((ptr)->next, type, member)


#ifdef __cplusplus
}
#endif
#endif