#include "utils.h"

void compress_position(Vec2 pos, Point *out) {
        out->x = (uint16_t)(pos.x);
        out->y = (uint16_t)(pos.y);
}

Vec2 decompress_position(Point point) {
    return (Vec2){
        .x = (float)point.x,
        .y = (float)point.y
    };
}
