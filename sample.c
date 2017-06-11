
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "cb_locl.h"

typedef struct test_func_st {
    cb_ctx *cb_ctx;
} TEST;

int test_func(void *ctx, void *data) {
    TEST *test = (TEST*)ctx;
    
    printf("cb ctx %p, data %p.\n", ctx, data);
    free(test->cb_ctx);
    free(test);
    free(data);
    return 0;
}

int main(int argc, void** argv) {
    cb_ctx *ctx = NULL;
    TEST *test = NULL;
    void *data = NULL;
    
    ctx = (cb_ctx*)malloc(sizeof(cb_ctx));
    if(ctx == NULL) {
        goto err;
    }
    
    test = (TEST *)malloc(sizeof(TEST));
    if(test == NULL) {
        goto err;
    }
    
    data = malloc(10);
    if(data == NULL) {
        goto err;
    }
    
    list_init(&ctx->list);
    ctx->ctx = test;
    test->cb_ctx = ctx;
    
    
    printf("main ctx %p, data %p.\n", test, data);
    
    task_init();
    register_task(ctx, data, test_func, 0);
    
    sleep(10);
    return 0;
    
err:
    free(ctx);
    free(test);
    free(data);
}

