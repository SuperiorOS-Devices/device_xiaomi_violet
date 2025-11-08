#!/bin/bash
# Auto-apply framework patch for SurfaceFlinger (disable HW overlays by default)

apply_violet_patches() {
    echo "=== Applying SurfaceFlinger disable overlays patch ==="

    # Detect Android top automatically (works even if envsetup.sh not sourced)
    if [ -z "$ANDROID_BUILD_TOP" ]; then
        ANDROID_BUILD_TOP=$(pwd)
        while [ "$ANDROID_BUILD_TOP" != "/" ] && [ ! -d "$ANDROID_BUILD_TOP/build" ]; do
            ANDROID_BUILD_TOP=$(dirname "$ANDROID_BUILD_TOP")
        done
    fi

    PATCH_DIR="${ANDROID_BUILD_TOP}/device/xiaomi/violet/patches"
    PATCH_FILE="${PATCH_DIR}/disable_hw_overlays.patch"
    TARGET_DIR="${ANDROID_BUILD_TOP}/frameworks/native"

    echo "Using ANDROID_BUILD_TOP: $ANDROID_BUILD_TOP"
    echo "Looking for patch at: $PATCH_FILE"

    if [ ! -f "${PATCH_FILE}" ]; then
        echo "❌ Patch file not found: ${PATCH_FILE}"
        return
    fi

    cd "${TARGET_DIR}" || return

    if git apply --check "${PATCH_FILE}" >/dev/null 2>&1; then
        git apply --whitespace=fix "${PATCH_FILE}"
        echo "✅ Patch successfully applied."
    else
        echo "⚠️ Patch already applied or cannot be checked."
    fi

    cd "${ANDROID_BUILD_TOP}" || return
}

apply_violet_patches
