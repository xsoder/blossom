#ifndef UI_H
#define UI_H

#include "raylib.h"
typedef struct {
    int width;
    int height;
    char *title;
} Window;
typedef struct{
    Vector2 pos;
    char *load_text;
    char *file_name;
    float font_size;
    float line_space;
} Ui;
#define MAX_INPUT 100

typedef struct {
    char text[MAX_INPUT+1];
    int letter_count;
    int col;
    int row;
} Input;

typedef void (*window_init_t)(Window *win);
typedef void (*ui_init_t)(Ui *ui);
typedef void (*ui_update_t)(Ui *ui, Input *input);

#endif //UI_H
