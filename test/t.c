#include <stdio.h>
#include "../libcoro/coro.h"

coro_context ctx[INX], mainctx;

struct coro_stack mainstack, stack[INX];

int index[INX];

long count = 0;
void coro_body(void *arg)
{
    int i = *(int*)arg;

    while(1) {
      count++;
      if(i == (INX - 1)) {
            coro_transfer(&ctx[i], &mainctx);
            continue;
      }
      coro_transfer(&ctx[i], &ctx[i+1]);
    }
}

int main(int argc, char **argv)
{
    long counter = atol(argv[1]);
    int stacksize = atoi(argv[2]);
    printf("total:%ld, stackszie:%dKB\n", counter, stacksize);

    coro_stack_alloc(&mainstack, stacksize*1024);
    coro_create(&mainctx, NULL, NULL, mainstack.sptr, mainstack.ssze);
    int i;    
    for(i=0; i<INX; i++) {
        index[i] = i;
        coro_stack_alloc(&stack[i], stacksize*1024);
        coro_create(&ctx[i], coro_body, &index[i], stack[i].sptr, stack[i].ssze);
    }

    while(1) {
        coro_transfer(&mainctx, &ctx[0]);
        count++;
        if(count > counter) {
            break;
        } 
    }
    printf("switch count:%ld\n", count); 
    return 0;
}

// compile as this
// gcc ../libcoro/coro.c t.c -DCORO_ASM -DINX=100 / gcc ../libcoro/coro.c t.c -DCORO_UCONTEXT -DINX=100

// run as this
// time -p ./a.out 1000000000 8
