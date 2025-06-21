SHELL := /bin/bash
CC = gcc
CXX = g++

PLUGIN_VERSION := $(shell git describe --tags --always --dirty)

# Work around hacks in the Source engine
CFLAGS = -m32 -std=gnu++23 -fpermissive -fPIC \
	-Dstrnicmp=strncasecmp -Dstricmp=strcasecmp -D_vsnprintf=vsnprintf \
	-DPOSIX -DLINUX -D_LINUX -DGNU -DGNUC -DPLUGIN_VERSION=\"$(PLUGIN_VERSION)\"

CFLAGS_PLUGIN := -Wall -Wextra -pedantic

OPTFLAGS = -O3 -s -fno-ident -fno-asynchronous-unwind-tables

RELEASE_ARCHIVE := sourced-$(PLUGIN_VERSION)-linux_amd64.tar.gz
ROOT_DIR := $(shell git rev-parse --show-toplevel)

HL2SDK = hl2sdk-$(ENGINE)
HL2SDK_DIR := $(ROOT_DIR)/vendor/$(HL2SDK)

SOURCE_DIR := $(ROOT_DIR)/src
OUTPUT_DIR := $(ROOT_DIR)/output
OBJ_DIR := $(OUTPUT_DIR)/obj
ADDONS_DIR := $(OUTPUT_DIR)/addons
RELEASE_PATH := $(ADDONS_DIR)/sourced.so

# Include Source SDK directories
INCLUDES := \
	-isystem$(HL2SDK_DIR)/public \
	-isystem$(HL2SDK_DIR)/public/tier0 \
	-isystem$(HL2SDK_DIR)/public/tier1

# Include the folder with the Source SDK libraries
LINKFLAGS := -shared -m32 -L$(HL2SDK_DIR)/lib/public/linux

all: release

.PHONY: init-submodules
init-submodules:
	git submodule status | cut -d ' ' -f 3 | xargs rm -rf
	git submodule update --init --recursive

.PHONY: clean
clean:
	rm -rf $(OUTPUT_DIR)/*

.PHONY: objects-dir-create
objects-dir-create:
	mkdir -p $(OBJ_DIR)

# $(OBJ_DIR)/globals.o: \
# 		$(SOURCE_DIR)/globals/globals.h \
# 		$(SOURCE_DIR)/globals/globals.cpp
# 	$(CXX) $(CFLAGS) $(CFLAGS_PLUGIN) $(OPTFLAGS) $(INCLUDES) \
# 		-o $(OBJ_DIR)/globals.o \
# 		-c $(SOURCE_DIR)/globals/globals.cpp

# $(OBJ_DIR)/binary_utils.o: \
# 		$(SOURCE_DIR)/utils/binary_utils.h \
# 		$(SOURCE_DIR)/utils/binary_utils.cpp
# 	$(CXX) $(CFLAGS) $(CFLAGS_PLUGIN) $(OPTFLAGS) $(INCLUDES) \
# 		-o $(OBJ_DIR)/binary_utils.o \
# 		-c $(SOURCE_DIR)/utils/binary_utils.cpp

# $(OBJ_DIR)/io_utils.o: \
# 		$(SOURCE_DIR)/utils/io_utils.h \
# 		$(SOURCE_DIR)/utils/io_utils.cpp
# 	$(CXX) $(CFLAGS) $(CFLAGS_PLUGIN) $(OPTFLAGS) $(INCLUDES) \
# 		-o $(OBJ_DIR)/io_utils.o \
# 		-c $(SOURCE_DIR)/utils/io_utils.cpp

# $(OBJ_DIR)/hooks_get_tick_interval.o: init-submodules \
# 		$(SOURCE_DIR)/hooks/get_tick_interval.h \
# 		$(SOURCE_DIR)/hooks/get_tick_interval.cpp
# 	$(CXX) $(CFLAGS) $(CFLAGS_PLUGIN) $(OPTFLAGS) $(INCLUDES) \
# 		-o $(OBJ_DIR)/hooks_get_tick_interval.o \
# 		-c $(SOURCE_DIR)/hooks/get_tick_interval.cpp

# $(OBJ_DIR)/hooks.o: init-submodules \
# 		$(SOURCE_DIR)/hooks/hooks.h \
# 		$(SOURCE_DIR)/hooks/hooks.cpp
# 	$(CXX) $(CFLAGS) $(CFLAGS_PLUGIN) $(OPTFLAGS) $(INCLUDES) \
# 		-o $(OBJ_DIR)/hooks.o \
# 		-c $(SOURCE_DIR)/hooks/hooks.cpp

# $(OBJ_DIR)/plugin.o: init-submodules \
# 		$(SOURCE_DIR)/plugin.h \
# 		$(SOURCE_DIR)/plugin.cpp
# 	$(CXX) $(CFLAGS) $(CFLAGS_PLUGIN) $(OPTFLAGS) $(INCLUDES) \
# 		-o $(OBJ_DIR)/plugin.o \
# 		-c $(SOURCE_DIR)/plugin.cpp

# $(OBJ_DIR)/plugin_exports.o: init-submodules \
# 		$(SOURCE_DIR)/plugin_exports.cpp
# 	$(CXX) $(CFLAGS) $(CFLAGS_PLUGIN) $(OPTFLAGS) $(INCLUDES) \
# 		-o $(OBJ_DIR)/plugin_exports.o \
# 		-c $(SOURCE_DIR)/plugin_exports.cpp


$(OBJ_DIR)/plugin.o: init-submodules \
		$(SOURCE_DIR)/plugin/plugin.h \
		$(SOURCE_DIR)/plugin/plugin.cpp
	$(CXX) $(CFLAGS) $(CFLAGS_PLUGIN) $(OPTFLAGS) $(INCLUDES) \
		-o $(OBJ_DIR)/plugin.o \
		-c $(SOURCE_DIR)/plugin/plugin.cpp

$(OBJ_DIR)/plugin_exports.o: init-submodules \
		$(SOURCE_DIR)/plugin/plugin_exports.cpp
	$(CXX) $(CFLAGS) $(CFLAGS_PLUGIN) $(OPTFLAGS) $(INCLUDES) \
		-o $(OBJ_DIR)/plugin_exports.o \
		-c $(SOURCE_DIR)/plugin/plugin_exports.cpp

$(OBJ_DIR): objects-dir-create \
	$(OBJ_DIR)/plugin.o \
	$(OBJ_DIR)/plugin_exports.o

# $(OBJ_DIR): objects-dir-create \
# 	$(OBJ_DIR)/globals.o \
# 	$(OBJ_DIR)/hooks_get_tick_interval.o \
# 	$(OBJ_DIR)/hooks.o \
# 	$(OBJ_DIR)/binary_utils.o \
# 	$(OBJ_DIR)/io_utils.o \
# 	$(OBJ_DIR)/plugin.o \
# 	$(OBJ_DIR)/plugin_exports.o \
# 	$(OBJ_DIR)/plugin.o

$(ADDONS_DIR):
	mkdir -p $(ADDONS_DIR)

$(RELEASE_PATH): $(OBJ_DIR) $(ADDONS_DIR) $(OBJ_DIR)
	$(CXX) $(OPTFLAGS) -o $(RELEASE_PATH) $(LINKFLAGS) \
		$(OBJ_DIR)/*.o \
		-ltier0_srv \
		-l:tier1_i486.a \
		-l:mathlib_i486.a \
		-ldl

.PHONY: release
release: $(RELEASE_PATH)

# .PHONY: release
# release: $(RELEASE_PATH)
# 	cp $(ROOT_DIR)/static/srcds_tickrate_enabler.vdf $(ADDONS_DIR)
# 	cd $(OUTPUT_DIR) && tar czf $(RELEASE_ARCHIVE) addons
