/*
   Copyright (c) 2015, The Linux Foundation. All rights reserved.
   Copyright (C) 2016 The CyanogenMod Project.
   Copyright (C) 2019-2020 The LineageOS Project.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.
   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <fstream>
#include <unistd.h>
#include <vector>

#include <android-base/properties.h>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

#include "property_service.h"
#include "vendor_init.h"

using android::base::GetProperty;
using android::init::property_set;

std::vector<std::string> ro_props_default_source_order = {
    "",
    "odm.",
    "product.",
    "system.",
    "vendor.",
};

void property_override(char const prop[], char const value[], bool add = true)
{
    prop_info *pi;

    pi = (prop_info *)__system_property_find(prop);
    if (pi)
        __system_property_update(pi, value, strlen(value));
    else if (add)
        __system_property_add(prop, strlen(prop), value, strlen(value));
}

void vendor_load_properties()
{
    const auto set_ro_build_prop = [](const std::string &source,
                                      const std::string &prop,
                                      const std::string &value) {
        auto prop_name = "ro." + source + "build." + prop;
        property_override(prop_name.c_str(), value.c_str(), false);
    };

    const auto set_ro_product_prop = [](const std::string &source,
                                        const std::string &prop,
                                        const std::string &value) {
        auto prop_name = "ro.product." + source + prop;
        property_override(prop_name.c_str(), value.c_str(), false);
    };

    std::string region;
    region = GetProperty("ro.boot.hwc", "");

    if (region == "CN")
    {
        for (const auto &source : ro_props_default_source_order)
        {
            set_ro_build_prop(source, "fingerprint",
                              "Xiaomi/davinci/davinci:10/QKQ1.190825.002/V11.0.6.0.QFJCNXM:user/release-keys");
            set_ro_product_prop(source, "device", "davinci");
            set_ro_product_prop(source, "model", "Redmi K20");
        }
        property_override("ro.build.description", "davinci-user 10 QKQ1.190825.002 V11.0.6.0.QFJCNXM release-keys");
    }
    else if (region == "INDIA")
    {
        for (const auto &source : ro_props_default_source_order)
        {
            set_ro_build_prop(source, "fingerprint",
                              "Xiaomi/davinciin/davinciin:10/QKQ1.190825.002/V11.0.2.0.QFJINXM:user/release-keys");
            set_ro_product_prop(source, "device", "davinciin");
            set_ro_product_prop(source, "model", "Redmi K20");
        }
        property_override("ro.build.description", "davinciin-user 10 QKQ1.190825.002 V11.0.2.0.QFJINXM release-keys");
        property_override("ro.product.mod_device", "davinciin_in_global");
    }
    else if (region == "GLOBAL")
    {
        for (const auto &source : ro_props_default_source_order)
        {
            set_ro_build_prop(source, "fingerprint",
                              "Xiaomi/davinci/davinci:10/QKQ1.190825.002/V11.0.5.0.QFJMIXM:user/release-keys");
            set_ro_product_prop(source, "device", "davinci");
            set_ro_product_prop(source, "model", "Mi 9T");
        }
        property_override("ro.build.description", "davinci-user 10 QKQ1.190825.002 V11.0.5.0.QFJMIXM release-keys");
        property_override("ro.product.mod_device", "davinci_global");
    }
}
