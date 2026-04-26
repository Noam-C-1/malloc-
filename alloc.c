#include <stdio.h>
#include <stdlib.h> 
struct Block{ 
    int size,free; 
    struct Block* prev; 
    struct Block* next; 
    }; 
char memory[256];
int init=0; 
int sm=sizeof(memory),sb=sizeof(struct Block); 
void* myalloc(int size) 
{ 
    struct Block* block=(struct Block*)&memory;
     if (!init) 
     { 
        block->size=sm-sb; 
        block->free=1;
        block->next=NULL; 
        block->prev=NULL; 
        init=1; 
        printf("INIT\n"); 
     } 
     while(block!=NULL) 
     { 
        if (block->free==1 && block->size>=size)
         { 
            block->free=0; 
            if (block->size-size-sb>sb) 
             { 
                struct Block* split2=(struct Block*)((char*)block+size+sb); 
                split2->size=block->size-size-sb; 
                split2->free=1; block->size=size; 
                split2->next=block->next; 
                block->next=split2; 
                if (split2->next!=NULL) 
                (split2->next)->prev=split2; 
                split2->prev=block; 
              } 
            void* ptr=(void*)((char*)block+sb); return ptr;
        } 
        block=block->next; 
    } 
    return NULL; 
} 
void myfree(void* ptr) 
{ 
    struct Block* metadata=(struct Block*)((char*)ptr-sizeof(struct Block));
    metadata->free=1; 
    if (metadata->next!= NULL) 
      if(metadata->next->free==1) 
      { 
        metadata->size+=metadata->next->size+sb; 
        metadata->next=metadaata->next->next; 
        if (metadata->next!=NULL) 
        metadata->next->prev=metadata; 
    }
    if (metadata->prev!= NULL) 
     if(metadata->prev->free==1) 
     { 
        metadata->prev->size+=metadata->size+sb;
        metadata->prev->next=metadata->next;
        if (metadata->next!=NULL) 
          metadata->next->prev=metadata->prev; 
     } 
} 
int main()
{
    return 0;
}
