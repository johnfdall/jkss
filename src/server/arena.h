#ifndef ARENA_H
#define ARENA_H 
#include <stdlib.h>

typedef struct {
        void *buffer;
        size_t capacity;
        size_t offset;
} Arena;

void ArenaInit(Arena *arena, size_t capacity);
void *ArenaAllocate(Arena *arena, size_t size);
void *ArenaAllocateAlign(Arena *arena, size_t size, size_t alignment);
void ArenaReset(Arena *arena);
void ArenaDestroy(Arena *arena);

#endif
