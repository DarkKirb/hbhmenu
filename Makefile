# TARGET #

TARGET := 3DS
LIBRARY := 0

ifeq ($(TARGET),$(filter $(TARGET),3DS WIIU))
    ifeq ($(strip $(DEVKITPRO)),)
        $(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>devkitPro")
    endif
endif

# COMMON CONFIGURATION #

NAME := TestMenu

BUILD_DIR := build
OUTPUT_DIR := output
INCLUDE_DIRS := include
SOURCE_DIRS := source

EXTRA_OUTPUT_FILES :=

LIBRARY_DIRS := $(DEVKITPRO)/portlibs/armv6k
LIBRARIES := z

BUILD_FLAGS :=
RUN_FLAGS :=

# 3DS/Wii U CONFIGURATION #

ifeq ($(TARGET),$(filter $(TARGET),3DS WIIU))
    TITLE := TestMenu
    DESCRIPTION := TestMenu
    AUTHOR := darklink
endif

# 3DS CONFIGURATION #

ifeq ($(TARGET),3DS)
    LIBRARY_DIRS += $(DEVKITPRO)/libctru
    LIBRARIES += ctru

    PRODUCT_CODE := NTR-P-CTAP
    UNIQUE_ID := 0x1

    CATEGORY := Application
    USE_ON_SD := true

    MEMORY_TYPE := Application
    SYSTEM_MODE := 64MB
    SYSTEM_MODE_EXT := Legacy
    CPU_SPEED := 804MHz
    ENABLE_L2_CACHE := true

    ICON_FLAGS :=

    ROMFS_DIR := romfs/
    BANNER_AUDIO := meta/audio_3ds.wav
    BANNER_IMAGE := meta/banner_3ds.png
    ICON := meta/icon_3ds.png
endif

# Wii U CONFIGURATION #

ifeq ($(TARGET),WIIU)
    LIBRARY_DIRS +=
    LIBRARIES +=

    LONG_DESCRIPTION := Build template.
    VERSION := Dev
    ICON := meta/icon_wiiu.png
endif

# INTERNAL #

include buildtools/make_base
