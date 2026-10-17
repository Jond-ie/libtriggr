include $(THEOS)/makefiles/common.mk
BUNDLE_NAME = TriggrDemoPrefs
TriggrDemoPrefs_FILES = DemoPrefs.m
TriggrDemoPrefs_INSTALL_PATH = /Library/PreferenceBundles
TriggrDemoPrefs_FRAMEWORKS = UIKit
TriggrDemoPrefs_PRIVATE_FRAMEWORKS = Preferences
TriggrDemoPrefs_CFLAGS = -fobjc-arc -I../../..
TriggrDemoPrefs_LDFLAGS = -L../../lib -ltriggr -rpath /var/jb/usr/lib
include $(THEOS_MAKE_PATH)/bundle.mk
internal-stage::
	$(ECHO_NOTHING)mkdir -p "$(THEOS_STAGING_DIR)/Library/PreferenceLoader/Preferences"$(ECHO_END)
	$(ECHO_NOTHING)cp entry.plist "$(THEOS_STAGING_DIR)/Library/PreferenceLoader/Preferences/TriggrDemo.plist"$(ECHO_END)
