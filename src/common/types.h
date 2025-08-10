#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stdbool.h>

typedef enum { 
        CMD_MOVE, 
        CMD_ATTACK, 
} CommandType;

//TODO(John Fredrik): This should be moved and turned
//into a struct which represents network Vec2
typedef struct {
        uint16_t x;
        uint16_t y;
} Point;

typedef struct {
        float x;
        float y;
} Vec2;

typedef struct {
        int32_t x;
        int32_t y;
        int32_t width;
        int32_t height;
} SelectionRect;

typedef struct {
        bool is_selecting;
        Vec2 start_pos;
        Vec2 current_pos;
} BoxSelectState;

#endif
