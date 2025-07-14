#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "editor.h"
#include "ui.h"
#include "dsa.h"
#include <raylib.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>

EXPORT_DA_TYPES()


time_t GetLastModifiedTime(const char *filename) {
    struct stat attr;
    if (stat(filename, &attr) == 0) {
        return attr.st_mtime;
    }
    return 0;  
}
char *ReadEntireFile(const char *filename) {
        return LoadFileText(filename);
}

bool HasFileContentChanged(const char *oldText, const char *newText) {
        if (oldText == NULL || newText == NULL) return true;
            return strcmp(oldText, newText) != 0;
}


window_init_t window_init = NULL;
ui_init_t ui_init = NULL;
ui_update_t ui_update = NULL;
Ui ui = {0};
Input input = {0};
Window win = {0};

const char *lib_ui_name = "libui.so";
void *libui = NULL;


bool reload_libui()
{
    if (libui != NULL) dlclose(libui);
    libui = dlopen(lib_ui_name,  RTLD_NOW);
    if (libui == NULL){
        fprintf(stderr, "Could not load %s due %s\n", lib_ui_name, dlerror());
        return false;
    }
    ui_update = dlsym(libui, "ui_update");
    if (ui_update == NULL){
        fprintf(stderr, "Could not load %s due %s\n", lib_ui_name, dlerror());
        return false;
    }

    ui_init = dlsym(libui, "ui_init");
    if (ui_init == NULL){
        fprintf(stderr, "Could not load %s due %s\n", lib_ui_name, dlerror());
        return false;
    }
    window_init = dlsym(libui, "window_init");
    if (window_init == NULL){
        fprintf(stderr, "Could not load %s due %s\n", lib_ui_name, dlerror());
        return false;
    }
    ui_init(&ui);
    window_init(&win);
    return true;

}
int main(void)
{
    if (!reload_libui()) return 1;
    input.col = 0;
    input.row = 0;

    InitWindow(win.width, win.height, win.title);
    SetTargetFPS(60);
    time_t lastModified = GetLastModifiedTime(ui.file_name);
    int frameCounter = 0;
    const int checkInterval = 10;

while (!WindowShouldClose())
{
    if (frameCounter++ >= checkInterval) {
        frameCounter = 0;
        time_t currentModified = GetLastModifiedTime(ui.file_name);
        if (currentModified != lastModified) {
            char *newText = ReadEntireFile(ui.file_name);
            if (HasFileContentChanged(ui.load_text, newText)) {
                if (ui.load_text) UnloadFileText(ui.load_text);
                ui.load_text = newText;
                lastModified = currentModified;
                 } else {
                UnloadFileText(newText);
            }
        }
    }

    if (input.col > GetScreenWidth()) input.col += 2;
    if (input.row > GetScreenHeight()) input.row += 2;
 if (IsKeyPressed(KEY_ENTER))
    {
        input.letter_count = 0;
        input.text[0] = '\0';
        if (input.col > GetScreenWidth()) input.col += 2;
        if (input.row > GetScreenHeight()) input.row += 2;
        input.row += 100;
        
     }
     if(input.letter_count < MAX_INPUT)
     {
         int key = GetKeyPressed();
         if((key >= 32) && (key <= 125))
         {
             input.text[input.letter_count] = (char) key;
             input.letter_count ++;
             input.text[input.letter_count] = '\0';
             
         }
     }
    BeginDrawing();
    ClearBackground(BLACK);
    ui_update(&ui, &input);
  
    if (IsKeyPressed(KEY_R)) {
        if (!reload_libui()) return 1;
        lastModified = GetLastModifiedTime(ui.file_name);
    }
   
        EndDrawing();
}

    return 0;
}
