#include <stdio.h>
#include <dlfcn.h>
#include "editor.h"
#include "ui.h"
#include <raylib.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>

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


hello_t hello_var = NULL;
ui_init_t ui_init = NULL;
ui_update_t ui_update = NULL;
Ui ui={0};

int frameCounter = 0;

const int checkInterval = 10; 
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
    ui_init(&ui);
    return true;

}
int main(void)
{

    int width = 600;
    int height = 800;
    const char* title = "editor";

    InitWindow(width, height, title);
    SetTargetFPS(60);
    if (!reload_libui()) {
        return 1;
    }
    printf("File name %s\n", ui.file_name);
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
                TraceLog(LOG_INFO, "File reloaded: external change detected.");
            } else {
                UnloadFileText(newText);
            }
        }
    }

    BeginDrawing();
    ui_update(&ui);
    EndDrawing();

    if (IsKeyPressed(KEY_R)) {
        if (!reload_libui()) return 1;
        lastModified = GetLastModifiedTime(ui.file_name);
    }
}

    return 0;
}
