#
# Copyright (C) 2019 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#

# Display density
TARGET_SCREEN_DENSITY := 440

# Inherit from sm6150-common
-include device/xiaomi/sm6150-common/BoardConfigCommon.mk

DEVICE_PATH := device/xiaomi/davinci

# Properties
TARGET_SYSTEM_PROP += $(DEVICE_PATH)/system.prop
TARGET_VENDOR_PROP += $(DEVICE_PATH)/vendor.prop

# Assert
TARGET_OTA_ASSERT_DEVICE := davinci,davinciin

# Init
TARGET_INIT_VENDOR_LIB := libinit_davinci
TARGET_RECOVERY_DEVICE_MODULES := libinit_davinci

# Kernel
BOARD_KERNEL_BASE := 0x00000000
BOARD_RAMDISK_OFFSET := 0x02000000
TARGET_KERNEL_CONFIG := vendor/lineage_davinci_defconfig

# Platform
TARGET_BOARD_PLATFORM_GPU := qcom-adreno618

# HIDL
DEVICE_MANIFEST_FILE += $(DEVICE_PATH)/manifest.xml

# NFC
TARGET_USES_NQ_NFC := true

# Partitions
BOARD_VENDORIMAGE_PARTITION_SIZE := 1610612736

# Inherit from the proprietary version
-include vendor/xiaomi/davinci/BoardConfigVendor.mk
