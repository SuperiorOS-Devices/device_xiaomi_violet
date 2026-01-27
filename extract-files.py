#!/usr/bin/env -S PYTHONPATH=../../../tools/extract-utils python3
#
# SPDX-FileCopyrightText: 2024 The LineageOS Project
# SPDX-License-Identifier: Apache-2.0
#

import extract_utils.tools

extract_utils.tools.DEFAULT_PATCHELF_VERSION = '0_9'

from extract_utils.fixups_blob import (
    blob_fixup,
    blob_fixups_user_type,
)
from extract_utils.fixups_lib import (
    lib_fixups,
    lib_fixup_remove,
    lib_fixups_user_type,
)
from extract_utils.main import (
    ExtractUtils,
    ExtractUtilsModule,
)

namespace_imports = [
    'device/xiaomi/violet',
    'hardware/qcom-caf/sm8150',
    'hardware/qcom-caf/wlan',
    'hardware/xiaomi',
    'vendor/qcom/opensource/commonsys/display',
    'vendor/qcom/opensource/commonsys-intf/display',
    'vendor/qcom/opensource/dataservices',
    'vendor/qcom/opensource/display',
]

def lib_fixup_vendor_suffix(lib: str, partition: str, *args, **kwargs):
    return f'{lib}_{partition}' if partition == 'vendor' else None

lib_fixups: lib_fixups_user_type = {
    **lib_fixups,
    (
        'com.qualcomm.qti.dpm.api@1.0',
        'libmmosal',
        'vendor.qti.hardware.fm@1.0',
        'vendor.qti.imsrtpservice@3.0',
    ): lib_fixup_vendor_suffix,
}

blob_fixups: blob_fixups_user_type = {
    'vendor/lib/libwvhidl.so':
        blob_fixup().replace_needed('libcrypto.so', 'libcrypto-v34.so'),
    'vendor/lib/mediadrm/libwvdrmengine.so':
        blob_fixup().replace_needed('libcrypto.so', 'libcrypto-v34.so'),
    'vendor/lib64/libwvhidl.so':
        blob_fixup().replace_needed('libcrypto.so', 'libcrypto-v34.so'),
    'vendor/lib64/mediadrm/libwvdrmengine.so':
        blob_fixup().replace_needed('libcrypto.so', 'libcrypto-v34.so'),
    'vendor/lib64/libvidhance.so':
        blob_fixup().add_needed('libdemangle.so')
                    .add_needed('libcomparetf2.so'),
    'vendor/lib64/camera/components/com.vidhance.node.eis.so':
        blob_fixup().add_needed('libdemangle.so')
                    .add_needed('libcomparetf2.so')
                    .replace_needed('libui.so', 'libui-v34.so'),
    'vendor/lib64/camera/components/com.vidhance.stats.aec_dmbr.so':
        blob_fixup().add_needed('libcomparetf2.so'),
    'vendor/lib64/hw/camera.qcom.so':
        blob_fixup().binary_regex_replace(b'libc\\+\\+.so', b'libc29.so'),
    'vendor/bin/mlipayd@1.1':
        blob_fixup().remove_needed('vendor.xiaomi.hardware.mtdservice@1.0.so'),
    'vendor/lib64/libmlipay.so':
        blob_fixup().remove_needed('vendor.xiaomi.hardware.mtdservice@1.0.so'),
    'vendor/lib64/libmlipay@1.1.so':
        blob_fixup().remove_needed('vendor.xiaomi.hardware.mtdservice@1.0.so'),
    'system_ext/lib64/libwfdnative.so':
        blob_fixup().remove_needed('android.hidl.base@1.0.so'),
    'system_ext/lib/libwfdnative.so':
        blob_fixup().remove_needed('android.hidl.base@1.0.so'),
    'vendor/lib64/libgoodixhwfingerprint.so':
        blob_fixup().remove_needed('android.hidl.base@1.0.so'),
    'vendor/etc/camera/camxoverridesettings.txt':
        blob_fixup().regex_replace(r'0x10080', '0')
                    .regex_replace(r'0x1F', '0x0'),
    'vendor/lib64/libvendor.goodix.hardware.interfaces.biometrics.fingerprint@2.1.so': 
        blob_fixup().remove_needed('libhidlbase.so')
                    .binary_regex_replace(b'libhidltransport.so', 'libhidlbase-v32.so\x00'),
    'vendor/lib64/mediadrm/libwvdrmengine.so':
        blob_fixup().replace_needed('libprotobuf-cpp-lite-3.9.1.so', 'libprotobuf-cpp-full-3.9.1.so'),
    'vendor/lib/mediadrm/libwvdrmengine.so':
        blob_fixup().replace_needed('libprotobuf-cpp-lite-3.9.1.so', 'libprotobuf-cpp-full-3.9.1.so'),
    'vendor/lib64/libwvhidl.so':
        blob_fixup().replace_needed('libprotobuf-cpp-lite-3.9.1.so', 'libprotobuf-cpp-full-3.9.1.so'),
}

module = ExtractUtilsModule(
    'violet',
    'xiaomi',
    blob_fixups=blob_fixups,
    lib_fixups=lib_fixups,
    namespace_imports=namespace_imports,
)

if __name__ == '__main__':
    utils = ExtractUtils.device(module)
    utils.run()
