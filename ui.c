#include "ui.h"
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

int col = 100;
int row = 100;

void ui_init(Ui *ui)
{
    ui->font_size = 50;
    ui->pos.x = 10;
    ui->pos.y = 10;
    ui->file_name = "hello.txt";
    ui->load_text =  LoadFileText(ui->file_name);
}
void ui_update(Ui *ui)
{
    ClearBackground(BLACK);
    if(IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)){
        if(IsKeyPressed(KEY_S)){
            if(!SaveFileText(ui->load_text,"Hello world")){
                fprintf(stderr, "Unable to save file\n");
                return;
            }
        }
    }
    col = col * (int)ui->pos.x;
    row = row * (int)ui->pos.y;
    // TODO: Add a support to modify file
    DrawText(ui->load_text, (int)ui->pos.x, (int)ui->pos.y, ui->font_size, WHITE);
}
