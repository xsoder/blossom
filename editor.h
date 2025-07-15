#ifndef EDITOR_H
#define EDITOR_H

#include <time.h>
#include <string.h>
#include <stdbool.h>


time_t GetLastModifiedTime(const char *filename);
char *ReadEntireFile(const char *filename);
bool HasFileContentChanged(const char *oldText, const char *newText);
bool reload_libui();

#endif //EDITOR_H
