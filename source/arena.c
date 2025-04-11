#include <assert.h>

#include <stdlib.h>
#include "arena.h"

#define max(a, b) (((a) >= b) ? (a) : (b))

Arena ArenaCreate(size_t capacity) {
	Arena arena = {
		.capacity = capacity,
		.data = calloc(capacity, 1),
	};
	// TODO: Handle error allocating data.
	assert(arena.data && "Failed to allocate buffer for arena.");
	return arena;
}

void ArenaDestroy(Arena *arena) {
	free(arena->data);
	*arena = (Arena){0};
}

void *ArenaAllocate(Arena *arena, size_t size) {
	if (arena->size + size > arena->capacity) {
		// Grow the data by at least a factor of 2.
		size_t newCapacity = max(arena->capacity*2, arena->capacity + size);
		void *newData = realloc(arena->data, newCapacity);
		// TODO: Handle error reallocating data.
		assert(newData && "Failed to reallocate data.");
		arena->capacity = newCapacity;
		arena->data = newData;
	}
	void *allocation = (char*)arena->data + arena->size;
	arena->size += size;
	return allocation;
}

void ArenaDeallocate(Arena *arena, size_t amount) {
	assert(amount <= arena->size && "Can't deallocate more than is allocated.");
	arena->size -= amount;
}

#undef max
