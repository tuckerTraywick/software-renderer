#ifndef ARENA_H
#define ARENA_H

#include <stddef.h>

// Represents a pool of memory you can free all at once. Frees of individual allocations can happen
// in FIFO order.
typedef struct Arena {
	size_t capacity;
	size_t size;
	void *data;
} Arena;

Arena ArenaCreate(size_t capacity);

void ArenaDestroy(Arena *arena);

void *ArenaAllocate(Arena *arena, size_t size);

void ArenaDeallocate(Arena *arena, size_t amount);

#endif // ARENA_H
