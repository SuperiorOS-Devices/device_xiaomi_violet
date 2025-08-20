/*
 * Copyright (C) 2019-2020 The LineageOS Project
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

#define LOG_TAG "vendor.lineage.livedisplay-service.xiaomi_sm6150"

#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>
#include <binder/ProcessState.h>
#include <livedisplay/sdm/PictureAdjustment.h>

#include "SunlightEnhancement.h"

using ::aidl::vendor::lineage::livedisplay::SunlightEnhancement;
using ::aidl::vendor::lineage::livedisplay::sdm::PictureAdjustment;
using ::aidl::vendor::lineage::livedisplay::sdm::SDMController;

int main() {
    android::ProcessState::self()->setThreadPoolMaxThreadCount(1);
    android::ProcessState::self()->startThreadPool();

    std::shared_ptr<SDMController> controller = std::make_shared<SDMController>();

    std::shared_ptr<PictureAdjustment> pictureAdjustment =
            ndk::SharedRefBase::make<PictureAdjustment>(controller);
    std::string instance = std::string() + PictureAdjustment::descriptor + "/default";
    if (AServiceManager_addService(pictureAdjustment->asBinder().get(), instance.c_str()) !=
        STATUS_OK) {
        LOG(ERROR) << "Cannot register picture adjustment HAL service.";
        return 1;
    }

    std::shared_ptr<SunlightEnhancement> sunlightEnhancement =
            ndk::SharedRefBase::make<SunlightEnhancement>();
    instance = std::string() + SunlightEnhancement::descriptor + "/default";
    if (AServiceManager_addService(sunlightEnhancement->asBinder().get(), instance.c_str()) !=
        STATUS_OK) {
        LOG(ERROR) << "Cannot register sunlight enhancement HAL service.";
        return 1;
    }
    LOG(INFO) << "LiveDisplay HAL service is ready.";

    ABinderProcess_joinThreadPool();

    LOG(ERROR) << "LiveDisplay HAL service failed to join thread pool.";
    return 1;
}
