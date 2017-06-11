
#include <stdio.h>

typedef int (*TASK_FUNC)(void* ctx, void* data);

typedef struct cb_ctx_st cb_ctx;

typedef struct list_head_st {
    list_head *next, *prev;
} list_head;

typedef list_head list_t;

typedef struct task_data_st {
    void      *data;
    TASK_FUNC task_func;
    list_t list;
} task_data;

typedef struct cb_ctx_st {
    void *ctx;
    task_data *task;
    list_t list;
} cb_ctx;

inline int list_is_empty(list_t *list) {
    return (list->next == list->prev);
}
inline void list_init(list_t *list) {
    list->prev = list;
    list->next = list;
}

inline void list_add_tail(list_t *head, list_t *list) {
    list->prev = head->prev;
    head->prev->next = list;
    head->prev = list;
    list->next = head;
}

inline void list_add_head(list_t *head, list_t *list) {
    list->next = head->next;
    head->next->prev = list;
    head->next = list;
    list->prev = head;
}

inline list_t* list_entry( list_t *head ) {
    list_t list;

    list = head->next;
    list_del_init(list);
    return list;
}

inline void list_del_init(list_t list) {
    list->prev->next = list->next;
    list->next->prev = list->prev;
    list->next = list;
    list->prev = list;
}

int register_task(cb_ctx *ctx, void *data, TASK_FUNC task_func, int add_head);
int task_init(void);

