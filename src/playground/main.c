#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
	float x;
	float y;
} vec2_t;

typedef struct {
	uint16_t x;
	uint16_t y;
} vec2_network_t;

void compress_position(vec2_t pos, vec2_network_t *out) 
{
        out->x = (uint16_t)(pos.x * 100);
        out->y = (uint16_t)(pos.y * 100);
}

vec2_t decompress_position(vec2_network_t point) 
{
    return (vec2_t){
        .x = (float)point.x / 100.0f,
        .y = (float)point.y / 100.0f
    };
}

int main() 
{
	vec2_t testing = {
		.x = 137.3291,
		.y = 99.3218
	};
	printf("Before compression: {x: %f, y: %f}\n", testing.x, testing.y);
	vec2_network_t out = {0};
	compress_position(testing, &out);
	printf("After compression: {x: %d, y: %d}\n", out.x, out.y);
	vec2_t after = decompress_position(out);
	printf("After decompression: {x: %f, y: %f}\n", after.x, after.y);

	return EXIT_SUCCESS;
}
