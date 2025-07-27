#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stdbool.h>

typedef enum { 
        CMD_MOVE, 
        CMD_ATTACK, 
} CommandType;

typedef struct Point {
        int32_t x;
        int32_t y;
} Point;

typedef struct SelectionRect {
        int32_t x;
        int32_t y;
        int32_t width;
        int32_t height;
} SelectionRect;

typedef struct BoxSelectState {
        bool is_selecting;
        Point start_pos;
        Point current_pos;
} BoxSelectState;

#endif
