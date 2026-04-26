#include <stdio.h>
#include <stdlib.h>

typedef struct Block {
    int size;
    int free;
    struct Block* prev;
    struct Block* next;
} Block;

char memory[256];
int init = 0;

#define SB sizeof(Block)
#define SM sizeof(memory)

void init_heap() {
    struct Block* head = (Block*)memory;
    head->size = SM - SB;
    head->free = 1;
    head->next = NULL;
    head->prev = NULL;
    init = 1;
}

void* myalloc(int size)
{
    if (size <= 0) return NULL;

    if (!init) {
        init_heap();
    }

   struct Block* block = (Block*)memory;

    while (block != NULL)
    {
        if (block->free && block->size >= size)
        {
            int remaining = block->size - size;
            if (remaining >= SB + 1)
            {
                struct Block* split = (Block*)((char*)block + SB + size);

                split->size = remaining - SB;
                split->free = 1;
                split->next = block->next;
                split->prev = block;

                if (split->next)
                    split->next->prev = split;

                block->size = size;
                block->next = split;
            }

            block->free = 0;
            return (char*)block + SB;
        }

        block = block->next;
    }

    return NULL;
}

void myfree(void* ptr)
{
    if (ptr == NULL) return;

    struct Block* block = (struct Block*)((char*)ptr - SB);
    block->free = 1;

    if (block->next && block->next->free)
    {
        struct Block* next = block->next;
        block->size += SB + next->size;
        block->next = next->next;

        if (block->next)
            block->next->prev = block;
    }
    if (block->prev && block->prev->free)
    {
        struct Block* prev = block->prev;
        prev->size += SB + block->size;
        prev->next = block->next;

        if (block->next)
            block->next->prev = prev;

        block = prev;
    }
    if (block->next && block->next->free)
    {
        struct Block* next = block->next;
        block->size += SB + next->size;
        block->next = next->next;

        if (block->next)
            block->next->prev = block;
    }
}

int main()
{
int* a = myalloc(8);
int* b = myalloc(8);
int* c = myalloc(8);

myfree(a);
myfree(b);
myfree(c);

int* d = myalloc(24);

printf("%p %p\n", a, d);
return 0;
}
