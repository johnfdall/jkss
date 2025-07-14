#include <stdio.h>
#include <stdlib.h>
#pragma once

typedef struct {
        void *buffer;
        size_t capacity;
        size_t offset;
} Arena;

void ArenaInit(Arena *arena, size_t capacity) {
        arena->buffer = malloc(capacity);
        arena->capacity = capacity;
        arena->offset = 0;

        if (!arena->buffer) {
                fprintf(stderr, "Arena allocation failed\n");
                exit(1);
        }
}

void *ArenaAllocate(Arena *arena, size_t size) {
        if (arena->offset + size > arena->capacity) {
                fprintf(stderr, "Arena allocation failed: out of memory\n");
                exit(1);
        }

        void *ptr = (char *)arena->buffer + arena->offset;
        arena->offset += size;
        return ptr;
}

void *ArenaAllocateAlign(Arena *arena, size_t size, size_t alignment) {
        size_t currentPosition = (size_t)arena->buffer + arena->offset;
        size_t alignedPosition =
                (currentPosition + (alignment - 1)) & ~(alignment - 1);

        if (alignedPosition + size > arena->capacity) {
                fprintf(stderr, "Arena out of memory\n");
                exit(1);
        }

        arena->offset = alignedPosition + size;
        return (void *)alignedPosition;
}

void ArenaReset(Arena *arena) { arena->offset = 0; }

void ArenaDestroy(Arena *arena) {
        free(arena->buffer);
        arena->buffer = NULL;
        arena->capacity = 0;
        arena->offset = 0;
}
