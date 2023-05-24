/*
 * Copyright (c) 2017-2018, The Linux Foundation. All rights reserved.
 * Not a Contribution
 */
/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* ​​​​​Changes from Qualcomm Innovation Center are provided under the following license:
 * Copyright (c) 2023 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#define LOG_TAG "LocSvc_GnssMeasurementInterface"

#include <log_util.h>
#include <GnssMeasurement.h>

namespace android {
namespace hardware {
namespace gnss {
namespace V1_0 {
namespace implementation {

void GnssMeasurement::GnssMeasurementDeathRecipient::serviceDied(
        uint64_t cookie, const wp<IBase>& who) {
    LOC_LOGE("%s] service died. cookie: %llu, who: %p",
            __FUNCTION__, static_cast<unsigned long long>(cookie), &who);
    if (mGnssMeasurement != nullptr) {
        mGnssMeasurement->close();
    }
}

GnssMeasurement::GnssMeasurement() {
    mGnssMeasurementDeathRecipient = new GnssMeasurementDeathRecipient(this);
    mApi = new MeasurementAPIClient();
}

GnssMeasurement::~GnssMeasurement() {
    if (mApi) {
        mApi->destroy();
        mApi = nullptr;
    }
}

// Methods from ::android::hardware::gnss::V1_0::IGnssMeasurement follow.
Return<GnssMeasurement::GnssMeasurementStatus> GnssMeasurement::setCallback(
        const sp<V1_0::IGnssMeasurementCallback>& callback)  {
    return setCallback(callback, mGnssMeasurementCbIface, GNSS_POWER_MODE_INVALID);
}

template <typename T>
Return<IGnssMeasurement::GnssMeasurementStatus> GnssMeasurement::setCallback(
        const sp<T>& callback, sp<T>& myCallback, GnssPowerMode powerMode) {
    Return<GnssMeasurement::GnssMeasurementStatus> ret =
            IGnssMeasurement::GnssMeasurementStatus::ERROR_GENERIC;
    if (mApi == nullptr) {
        LOC_LOGE("%s]: mApi is nullptr", __FUNCTION__);
        return ret;
    }
    if (myCallback != callback) {
        if (nullptr == callback) {
            LOC_LOGE("%s]: callback is nullptr", __FUNCTION__);
            mApi->measurementSetCallback(callback);
            close();
        } else {
            if (nullptr != myCallback) {
                myCallback->unlinkToDeath(mGnssMeasurementDeathRecipient);
            }
            myCallback = callback;
            myCallback->linkToDeath(mGnssMeasurementDeathRecipient, 0);
            ret = mApi->measurementSetCallback(callback, powerMode);
        }
    }

    return ret;
}

Return<void> GnssMeasurement::close()  {
    if (mApi != nullptr) {
        mApi->measurementSetCallback<V1_0::IGnssMeasurementCallback>(nullptr);
    }

    if (mGnssMeasurementCbIface != nullptr) {
        mGnssMeasurementCbIface->unlinkToDeath(mGnssMeasurementDeathRecipient);
        mGnssMeasurementCbIface = nullptr;
    }

    return Void();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace gnss
}  // namespace hardware
}  // namespace android
