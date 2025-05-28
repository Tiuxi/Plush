#ifndef LISTG
#define LISTG

#include <stdio.h>
#include <stdlib.h>
#include "utils/constants.h"

typedef struct s_list {
    void* v;
    struct s_list* next;
}*List;

/**
 * Create a new `List` object with the element `elem` in the first position
 * 
 * @param elem The first element of the new list
 * @returns An new `List` object
 */
List plushList_new(void* elem);

/**
 * Free the current element of the list `lst` and return the next one
 * 
 * @param lst   The element to free
 * @returns The next element
 */
List plushList_destroyElem(List lst);

/**
 * Free the whole list `lst`.
 * Please note that it will not free the value, for this you want to use `destroyAll`
 *
 * @short Free the `lst` list
 * @param lst       The list to free
 */
void plushList_destroyList(List lst);

/**
 * Free the whole list `lst` and the value inside every cell of the list
 *
 * @short Free list cell and list values
 * @param lst       The list to free
 */
void plushList_destroyAll(List lst);

/**
 * Free the 2D list `list2D`.
 * Please note that it will not free the value of the 2D List.
 * For this you may want to use `destroy2DListAll`
 * 
 * @param list2D    The 2D list to free
 */
void plushList_destroy2DList(List list2D);

/**
 * Free the 2D list `list2D` and the values inside.
 *
 * @param list2D    The 2D list to free
 */
void plushList_destroy2DListAll(List list2D);

/**
 * Free the last element of list `lst` and free the value if `destroy` is True (or 1)
 * 
 * @param lst       The list to remove the element from
 * @param destroy   If the last element value should be free'd or not
 */
List plushList_popDestroyValue(List lst, int destroy);

#define plushList_pop(lst) plushList_popDestroyValue(lst, 0)
#define plushList_popValue(lst) plushList_popDestroyValue(lst, 1)

/**
 * Push the element `elem` at the end of the list `lst` by creating a new list cell
 *
 * @short Put the element `elem` in the list `lst`
 * @param lst       The list where the element is pushed
 * @param elem      The element to push in the list
 * @return A list with the new element in it
 */
List plushList_push(List lst, void *elem);

/**
 * Print the list of char* `lst` 
 * 
 * @param lst   The list to print
 */
void plushList_printListString(List lst);

/**
 * Get the size of a list
 * 
 * @param lst   The list to get the size of
 */
int plushList_size(List lst);

#endif