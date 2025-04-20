#ifndef ENTITY_H
#define ENTITY_H

#include <stdint.h>
#include <stdbool.h>
#include "vector.h"
#include "sprite.h"

typedef enum Entity_Type {
	WALL,
	DOOR,
	ZONE,
	BUTTON,
	SWITCH,
	ITEM,
	DECAL,
	NPC,
	ENEMY,
} Entity_Type;

typedef enum Entity_Flags {
	HAS_ANGLE = 1,
	HAS_TEXTURE = 1 << 1,
	HAS_ITEM = 1 << 2,
	HAS_ENTER_TRIGGER = 1 << 3,
	HAS_INTERACT_TRIGGER = 1 << 4,
	HAS_COLLECT_TRIGGER = 1 << 5,
	IS_SOLID,
	IS_VISIBLE,
} Entity_Flags;

typedef void (*Trigger)(void);

typedef struct Entity {
	Entity_Type type;
	Entity_Flags flags;
	Vector3 position;
	Vector3 size;
	Vector3 angle;
	Vector2 texture_size;
	Sprite texture;
	// Item item;
	Trigger on_enter;
	Trigger on_interact;
	Trigger on_collect;
} Entity;

#endif // ENTITY_H
