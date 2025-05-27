#include "utils/list.h"

List plushList_new(void* elem) {
    return plushList_push(NULL, elem);
}

void plushList_destroyList(List lst) {
    while (lst != NULL) {
        List tmp = lst;
        lst = lst->next;
        free(tmp);
    }
    return;
}

List plushList_destroyElem(List lst) {
    List next = lst->next;
    free(lst);
    return next;
}

void plushList_destroy2DList(List list2D) {
    while (list2D != NULL) {
        plushList_destroyList(list2D->v);
        list2D = plushList_destroyElem(list2D);
    }
}

void plushList_destroy2DListAll(List list2D) {
    while (list2D != NULL) {
        plushList_destroyAll(list2D->v);
        list2D = plushList_destroyElem(list2D);
    }
}

void plushList_destroyAll(List lst) {
    while (lst != NULL) {
        free(lst->v);
        List tmp = lst;
        lst = lst->next;
        free(tmp);
    }
    return;
}

List plushList_push(List lst, void* elem) {
    List add = (List)malloc(sizeof(struct s_list));
    add->next = NULL; add->v = elem;

    if (lst == NULL) return add;

    List tmp = lst;
    while(tmp->next != NULL) tmp = tmp->next;
    tmp->next = add;
    return lst;
}

List plushList_popDestroyValue(List lst, int destroy) {
    List tmp = lst;
    
    if (tmp->next == NULL) {
        return plushList_destroyElem(tmp);
    }

    while (tmp->next->next != NULL) tmp = tmp->next;

    if (destroy == 1) {
        free(tmp->next->v);
        tmp->next = plushList_destroyElem(tmp->next);
    }else {
        tmp->next = plushList_destroyElem(tmp->next);
    }
    
    return lst;
}

void plushList_printListString(List lst) {
    putchar('[');
    List tmp;
    for (tmp=lst; tmp->next != NULL; tmp=tmp->next) {
        printf("\"%s\", ", (char*)(tmp->v));
    }
    printf("\"%s\"]\n", (char*)(tmp->v));
}

int plushList_size(List lst) {
    if (lst == NULL) return 0;

    List tmp = lst;
    int size = 1;
    while (tmp->next != NULL) {
        tmp = tmp->next;
        size++;
    }

    return size;
}