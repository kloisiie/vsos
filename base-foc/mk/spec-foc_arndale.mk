SPECS += foc_arm platform_arndale uboot

include $(call select_from_repositories,mk/spec-platform_arndale.mk)
include $(call select_from_repositories,mk/spec-foc_arm.mk)
