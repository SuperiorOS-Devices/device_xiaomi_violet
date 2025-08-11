#
# Copyright (C) 2018-2020 The LineageOS Project
# Copyright (C) 2020 The PixelExperience Project
#
# SPDX-License-Identifier: Apache-2.0
#

# Inherit from violet device
$(call inherit-product, device/xiaomi/violet/device.mk)

# Inherit some common LineageOS stuff.
$(call inherit-product, vendor/lineage/config/common_full_phone.mk)

# Inherit MiuiCamera
$(call inherit-product-if-exists, vendor/MiuiCamera/config.mk)

# AxionAOSP
AXION_CAMERA_REAR_INFO := 48,5
AXION_CAMERA_FRONT_INFO := 13
AXION_MAINTAINER := Joker-V2
AXION_PROCESSOR := Qualcomm_Snapdragon_675
PERF_GOV_SUPPORTED := true
PERF_DEFAULT_GOV := schedutil
TARGET_ENABLE_BLUR := true

# Device identifier. This must come after all inclusions.
PRODUCT_NAME := lineage_violet
PRODUCT_DEVICE := violet
PRODUCT_BRAND := Xiaomi
PRODUCT_MODEL := Redmi Note 7 Pro
PRODUCT_MANUFACTURER := Xiaomi

PRODUCT_GMS_CLIENTID_BASE := android-xiaomi

BUILD_FINGERPRINT := xiaomi/violet/violet:10/QKQ1.190915.002/V12.5.1.0.QFHINXM:user/release-keys
PRODUCT_BUILD_PROP_OVERRIDES += \
    BuildDesc=$(call normalize-path-list, "violet-user 10 QKQ1.190915.002 V12.5.1.0 release-keys")

PRODUCT_PRODUCT_PROPERTIES += \
   ro.build.fingerprint=$(BUILD_FINGERPRINT)
