
#include <stdio.h>
#include <pthread.h>
#include <error.h>

#include "cb_locl.h"

pthread_t pt;
list_head task_head;

void cb_task_func() {
	cb_ctx    *cb;
    task_data *task;
    task_data *next_task;
    cb_ctx    *ctx;
    int       ret;
    int       (*task_func)(void*, void*);
    void      *data;

    for(;;) {

        lock();
        if( list_is_empty( &task_head ) ) {
        	unlock();
            sleep(1);
            continue;
        }

        cb = list_entry(&task_head);
        ctx = cb->ctx;
        task = cb->task;

        data = task->data;
        task_func = task->cb_func;

        if( !list_is_empty(task->list) ) {
            next_task = task->list->next;
            cb->task = next_task;
            list_add_tail(&task_head, &cb->list);
        }
        unlock();

        free(task);
        ret = task_func(ctx, data);

    }
}

int register_task(cb_ctx ctx, void *data, int (*task_func)(void*, void*), int head) {
	task_data task;

	task = (task_data *)malloc(sizeof(task_data));
	if( task == NULL ) {
		return -ENOMEM;
	}
	task->data = data;
	task->cb_func = task_func;
    list_init(&task->list);

    lock();
    if( ctx->task == NULL ) {
    	ctx->task = task;
    }
    else {
    	list_add_tail( &ctx->task->list, task->list );
    }

    ctx->task->data = data;
    if( list_is_empty(&ctx->list) ) {
    	list_add_tail(&task_head, &ctx->list);
    }
    else if(head){
    	list_add_head(ctx->task->list)
    }
    else {
    	list_add_tail(ctx->task->list)
    }
    unlock();

    return 0;
}

int cb_task_init(void) {
    int ret;
    list_init(&task_head);
    ret = pthread_create(&pt, NULL, cb_task_func, NULL);

    return ret;
}
