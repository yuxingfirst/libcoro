#include <stdio.h>
#include "coro.h"

coro_context ctx, mainctx;
struct coro_stack mainstack, stack;
long count = 0;
void coro_body(void *arg)
{
    while(1) {
      count++;
      coro_transfer(&ctx, &mainctx);
      //printf("coro:%d\n", count);
    }
}

int main(int argc, char **argv)
{
    long counter = atol(argv[1]);
    int stacksize = atoi(argv[2]);
    printf("total:%ld, stackszie:%dKB\n", counter, stacksize);
    coro_stack_alloc(&mainstack, stacksize*1024);
    coro_create(&mainctx, NULL, NULL, mainstack.sptr, mainstack.ssze);
    coro_stack_alloc(&stack, stacksize*1024);
    coro_create(&ctx, coro_body, NULL, stack.sptr, stack.ssze);
    while(1) {
        coro_transfer(&mainctx, &ctx);
        count++;
        if(count > counter) {
            break;
        } 
        //printf("maincoro:%d\n", count);
    }
    printf("switch count:%d\n", count); 
    return 0;
}

// gcc coro.c t.c -DCORO_ASM
// time -p ./a.out 1000000000 8
