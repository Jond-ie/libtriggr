TARGET := iphone:clang:16.5:15.0
ARCHS = arm64 arm64e
INSTALL_TARGET_PROCESSES = SpringBoard
include $(THEOS)/makefiles/common.mk
TWEAK_NAME = TriggrDemo
TriggrDemo_FILES = Tweak.x
TriggrDemo_CFLAGS = -fobjc-arc -I../..
# Linked like any third-party tweak would: against libtriggr.dylib
TriggrDemo_LDFLAGS = -L../lib -ltriggr -rpath /var/jb/usr/lib
include $(THEOS_MAKE_PATH)/tweak.mk
SUBPROJECTS += Prefs
include $(THEOS_MAKE_PATH)/aggregate.mk
