#include <assert.h>

#include <stddef.h>
#include <stdlib.h>
#include "arena.h"

#define max(a, b) (((a) >= b) ? (a) : (b))

Arena Arena_create(size_t capacity) {
	Arena arena = {
		.capacity = capacity,
		.data = calloc(capacity, 1),
	};
	// TODO: Handle error allocating data.
	assert(arena.data && "Failed to allocate buffer for arena.");
	return arena;
}

void Arena_destroy(Arena *arena) {
	free(arena->data);
	*arena = (Arena){0};
}

void *Arena_allocate(Arena *arena, size_t size) {
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

void *Arena_allocate_aligned(Arena *arena, size_t size, size_t alignment) {
	
}

void Arena_deallocate(Arena *arena, size_t amount) {
	assert(amount <= arena->size && "Can't deallocate more than is allocated.");
	arena->size -= amount;
}

#undef max
