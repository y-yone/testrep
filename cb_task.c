
#include <stdio.h>
#include <pthread.h>
#include <error.h>

#include "cb_locl.h"

pthread_t pt;
list_head task_head;

void cb_task_func() {
    buf_data *task;
    cb_ctx   *ctx;
    int      ret;

    for(;;) {
        if( list_is_empty( &task_head ) ) {
            sleep(1);
            continue;
        }
        lock();
        task = list_entry(&task_head);
        ctx = task->data;
        free(task);
        unlock();

        ret = ctx->cb_func(ctx);

    }
}

int register_task(cb_ctx ctx, void *data) {
	buf_data buf;

	buf = (buf_data *)malloc(sizeof(cb_ctx));
	if( buf == NULL ) {
		return -ENOMEM;
	}
	buf->data = data;
    list_init(&buf->list);

    lock();
    if( ctx->data == NULL ) {
    	ctx->data = buf;
    }
    else {
    	list_add_tail( &ctx->data->list, buf->list );
    }
    ctx->data->data = data;
    if( list_is_empty(&ctx->list) ) {
    	list_add_tail(&task_head, &ctx->list);
    }
    else {
    	list_add_tail(ctx->cb_ctx->list)
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
