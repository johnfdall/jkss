#include "control_group.c"
#include "entity_array.c"
#include "raylib.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>

typedef enum { CMD_NONE, CMD_MOVE, CMD_ATTACK, CMD_INTERACT } CommandType;

typedef struct Point {
        int32_t x;
        int32_t y;
} Point;

typedef struct Command {
        CommandType type;
        Point target;
} Command;

Rectangle BoxSelect(Vector2 drag_start) {
        Vector2 current_mouse_pos = GetMousePosition();
        float width = current_mouse_pos.x - drag_start.x;
        float height = current_mouse_pos.y - drag_start.y;

        Rectangle selectionRect = {
                .x = (width >= 0) ? drag_start.x : drag_start.x + width,
                .y = (height >= 0) ? drag_start.y : drag_start.y + height,
                .width = fabsf(width),
                .height = fabsf(height)
        };

        DrawRectangleLinesEx(selectionRect, 1.0, GREEN);

        return selectionRect;
}

int main() {
        const int screenWidth = 1920;
        const int screenHeight = 1080;
        Vector2 drag_start = {.x = 0.0, .y = 0.0};
        bool draggin = false;

        Arena arena;
        ArenaInit(&arena, 1024 * 1024 * 1024); // Should be 1 gibby

        EntityArray entityArray;
        EntityArray_INIT(&entityArray, &arena, 20);
        EntityArray_SETUP(&entityArray);

        ControlGroupArray *controlGroup = ControlGroupArray_INIT(&arena);
        ControlGroup *mainControlGroup = &controlGroup->items[MAIN_CONTROL_GROUP];

        /*SelectGroup *selectGroup = SelectGroup_INIT(&arena);*/

        InitWindow(screenWidth, screenHeight, "Raylib - Hello");
        SetTargetFPS(60);

        while (!WindowShouldClose()) {
                bool left_click_pressed = IsMouseButtonPressed(0);
                bool left_click_down = IsMouseButtonDown(0);
                HandleEntityClick(&entityArray, mainControlGroup);
                DrawEntities(&entityArray, mainControlGroup);

                if (left_click_down && left_click_pressed) {
                        drag_start = GetMousePosition();
                }

                if (left_click_down) {
                        draggin = true;
                } else {
                        draggin = false;
                }
                if (draggin) {
                        Rectangle selectRect = BoxSelect(drag_start);
                        printf("%f\n", selectRect.x);
                }

                BeginDrawing();
                ClearBackground(BLACK);
                EndDrawing();
        }

        ArenaDestroy(&arena);
        CloseWindow();

        return EXIT_SUCCESS;
}
