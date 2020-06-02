
export BUILD_ROOT = $(shell pwd)
export INCLUDE_PATH = -I $(BUILD_ROOT)/lua51/include \
					  -I $(BUILD_ROOT)/misc/include \
					  -I $(BUILD_ROOT)/game/include
export LIB_NAME = -lpthread
export DEBUG = true

BUILD_DIR = $(BUILD_ROOT)/lua51/src \
			$(BUILD_ROOT)/misc/src \
			$(BUILD_ROOT)/game/src
