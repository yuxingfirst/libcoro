#include <stdio.h>

#include "coro.h"

coro_context ctx, mainctx;
struct coro_stack stack;

int count = 0;

void coro_body(void *arg)
{
    while(1) {
      count++;
      coro_transfer(&ctx, &mainctx);
    }
}

int main(int argc, char **argv)
{
    coro_create(&mainctx, NULL, NULL, NULL, 0);
    coro_stack_alloc(&stack, 0);
    coro_create(&ctx, coro_body, NULL, stack.sptr, stack.ssze);
    printf("Created a coro\n");
    while(1) {
      coro_transfer(&mainctx, &ctx);
      count++;
    }
    
    printf("switch count:%d\n", count);
    
    return 0;
}
