#ifndef UI_H
#define UI_H

#include "raylib.h"

typedef struct{
    Vector2 pos;
    char *load_text;
    char *file_name;
    float font_size;
    float line_space;
} Ui;

typedef void (*hello_t)(void);
typedef void (*ui_init_t)(Ui *ui);
typedef void (*ui_update_t)(Ui *ui);
#endif //UI_H
