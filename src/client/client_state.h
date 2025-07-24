#include <stdint.h>
#include "../entity/entity_array.h"

typedef struct {
	uint32_t id;
	uint32_t tick_count;
	EntityArray entities;
} ClientState;
