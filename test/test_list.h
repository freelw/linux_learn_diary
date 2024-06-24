#include "list.h"
#include <stdlib.h>
#include <stdio.h>

struct student
{
    struct list_head entry;
    int id;
};

void test_list() {
    struct list_head slist;
    INIT_LIST_HEAD(&slist);
    struct student *ps0 = (struct student *)calloc(1, sizeof(struct student));
    struct student *ps1 = (struct student *)calloc(1, sizeof(struct student));
    ps0->id = 123;
    ps1->id = 125;
    list_add_tail(&ps0->entry, &slist);
    list_add_tail(&ps1->entry, &slist);

    struct student *iter = NULL;
    struct student *iter_tmp = NULL;
    struct list_head *head = &slist;
    list_for_each_entry_safe(iter, iter_tmp, &slist, entry) {
        printf("%d ", iter->id);
    }
    printf("\n");
}
