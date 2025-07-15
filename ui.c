#include "ui.h"
#include "dsa.h"
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

EXPORT_DA_TYPES()

int col = 100;
int row = 100;
void window_init(Window *win)
{
    win->width = 600;
    win->height = 600;
    win->title = "blossom";
    
}
void ui_init(Ui *ui)
{
    ui->font_size = 20;
    ui->pos.x = 10;
    ui->pos.y = 10;
    ui->file_name = "hello.txt";
    ui->load_text =  LoadFileText(ui->file_name);
}
void ui_update(Ui *ui, Input *input)
{
    if(IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)){
        if(IsKeyPressed(KEY_S)){
            if(!SaveFileText(ui->load_text,"Hello world")){
                fprintf(stderr, "Unable to save file\n");
                return;
            }
        }
    }
    DrawText(input->text, input->col, input->row, 20, BLUE);
}
