#include "utils.h"

void compress_position(Vec2 pos, Point *out) {
        out->x = (uint16_t)(pos.x * 100);
        out->y = (uint16_t)(pos.y * 100);
}

Vec2 decompress_position(Point point) {
    return (Vec2){
        .x = (float)point.x / 100.0f,
        .y = (float)point.y / 100.0f
    };
}
