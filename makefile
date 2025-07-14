CC = gcc
CFLAGS = -Wall -Werror

SRC = editor.c
BUILD = editor
BUILD_DIR = build

PKG_CONFIG ?= pkg-config
RAYLIB_CFLAGS = $(shell $(PKG_CONFIG) --cflags raylib)
RAYLIB_LDFLAGS = $(shell $(PKG_CONFIG) --libs raylib)
default:
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(RAYLIB_CFLAGS) -fPIC -shared -o $(BUILD_DIR)/libui.so ui.c $(RAYLIB_LDFLAGS)
	$(CC) $(CFLAGS) $(RAYLIB_CFLAGS) -L$(BUILD_DIR) -Wl,-rpath,$(BUILD_DIR) -o $(BUILD_DIR)/$(BUILD) $(SRC) $(RAYLIB_LDFLAGS) -lui
