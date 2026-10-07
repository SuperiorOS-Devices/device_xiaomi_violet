/*
 * Copyright (C) 2015 The CyanogenMod Project
 *               2017-2024 The LineageOS Project
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

package org.lineageos.settings.doze;

import android.os.Bundle;
import android.os.Handler;
import androidx.preference.ListPreference;
import androidx.preference.Preference;
import androidx.preference.Preference.OnPreferenceChangeListener;
import androidx.preference.PreferenceCategory;
import androidx.preference.SwitchPreferenceCompat;
import com.android.settingslib.widget.MainSwitchPreference;
import com.android.settingslib.widget.SettingsBasePreferenceFragment;
import org.lineageos.settings.R;
import org.lineageos.settings.utils.FileUtils;

public class DozeSettingsFragment
        extends SettingsBasePreferenceFragment implements OnPreferenceChangeListener {
    private SwitchPreferenceCompat mAlwaysOnDisplayPreference;
    private ListPreference mDozeBrightnessPreference;
    private SwitchPreferenceCompat mWakeOnGesturePreference;
    private SwitchPreferenceCompat mPickUpPreference;
    private SwitchPreferenceCompat mHandwavePreference;
    private SwitchPreferenceCompat mPocketPreference;

    private Handler mHandler = new Handler();

    @Override
    public void onCreatePreferences(Bundle savedInstanceState, String rootKey) {
        setPreferencesFromResource(R.xml.doze_settings, rootKey);

        boolean dozeEnabled = DozeUtils.isDozeEnabled(getActivity());

        MainSwitchPreference switchBar = findPreference(DozeUtils.DOZE_ENABLE);
        switchBar.setOnPreferenceChangeListener(this);
        switchBar.setChecked(dozeEnabled);

        mAlwaysOnDisplayPreference =
                (SwitchPreferenceCompat) findPreference(DozeUtils.ALWAYS_ON_DISPLAY);
        mAlwaysOnDisplayPreference.setEnabled(dozeEnabled);
        mAlwaysOnDisplayPreference.setChecked(DozeUtils.isAlwaysOnEnabled(getActivity()));
        mAlwaysOnDisplayPreference.setOnPreferenceChangeListener(this);

        mDozeBrightnessPreference = (ListPreference) findPreference(DozeUtils.DOZE_BRIGHTNESS_KEY);
        mDozeBrightnessPreference.setEnabled(
                dozeEnabled && DozeUtils.isAlwaysOnEnabled(getActivity()));
        mDozeBrightnessPreference.setOnPreferenceChangeListener(this);

        PreferenceCategory gesturesCategory =
                (PreferenceCategory) getPreferenceScreen().findPreference(DozeUtils.CATEG_GESTURES);
        PreferenceCategory pickupSensorCategory =
                (PreferenceCategory) getPreferenceScreen().findPreference(
                        DozeUtils.CATEG_PICKUP_SENSOR);
        PreferenceCategory proximitySensorCategory =
                (PreferenceCategory) getPreferenceScreen().findPreference(
                        DozeUtils.CATEG_PROX_SENSOR);

        mWakeOnGesturePreference =
                (SwitchPreferenceCompat) findPreference(DozeUtils.WAKE_ON_GESTURE_KEY);
        mWakeOnGesturePreference.setEnabled(dozeEnabled);
        mWakeOnGesturePreference.setOnPreferenceChangeListener(this);

        mPickUpPreference = (SwitchPreferenceCompat) findPreference(DozeUtils.GESTURE_PICK_UP_KEY);
        mPickUpPreference.setEnabled(dozeEnabled);
        mPickUpPreference.setOnPreferenceChangeListener(this);

        mHandwavePreference =
                (SwitchPreferenceCompat) findPreference(DozeUtils.GESTURE_HAND_WAVE_KEY);
        mHandwavePreference.setEnabled(dozeEnabled);
        mHandwavePreference.setOnPreferenceChangeListener(this);

        mPocketPreference = (SwitchPreferenceCompat) findPreference(DozeUtils.GESTURE_POCKET_KEY);
        mPocketPreference.setEnabled(dozeEnabled);
        mPocketPreference.setOnPreferenceChangeListener(this);

        // Hide proximity sensor related features if the device doesn't support them
        if (!DozeUtils.getProxCheckBeforePulse(getActivity())) {
            getPreferenceScreen().removePreference(proximitySensorCategory);
        }

        // Hide AOD and doze brightness if not supported and set all its dependents otherwise
        if (!DozeUtils.alwaysOnDisplayAvailable(getActivity())) {
            getPreferenceScreen().removePreference(mAlwaysOnDisplayPreference);
            getPreferenceScreen().removePreference(mDozeBrightnessPreference);
        } else {
            if (!FileUtils.isFileWritable(DozeUtils.DOZE_MODE_PATH)) {
                getPreferenceScreen().removePreference(mDozeBrightnessPreference);
            } else {
                DozeUtils.updateDozeBrightnessIcon(getContext(), mDozeBrightnessPreference);
            }
            gesturesCategory.setDependency(DozeUtils.ALWAYS_ON_DISPLAY);
            pickupSensorCategory.setDependency(DozeUtils.ALWAYS_ON_DISPLAY);
            proximitySensorCategory.setDependency(DozeUtils.ALWAYS_ON_DISPLAY);
        }
    }

    @Override
    public boolean onPreferenceChange(Preference preference, Object newValue) {
        if (DozeUtils.DOZE_ENABLE.equals(preference.getKey())) {
            boolean isChecked = (Boolean) newValue;
            DozeUtils.enableDoze(getActivity(), isChecked);

            if (!isChecked) {
                DozeUtils.enableAlwaysOn(getActivity(), false);
                mAlwaysOnDisplayPreference.setChecked(false);
                mDozeBrightnessPreference.setValue(DozeUtils.DOZE_BRIGHTNESS_LBM);
                DozeUtils.updateDozeBrightnessIcon(getContext(), mDozeBrightnessPreference);
                mPickUpPreference.setChecked(false);
                mHandwavePreference.setChecked(false);
                mPocketPreference.setChecked(false);
            }
            mAlwaysOnDisplayPreference.setEnabled(isChecked);
            mDozeBrightnessPreference.setEnabled(
                    isChecked && DozeUtils.isAlwaysOnEnabled(getActivity()));
            mWakeOnGesturePreference.setEnabled(isChecked);
            mPickUpPreference.setEnabled(isChecked);
            mHandwavePreference.setEnabled(isChecked);
            mPocketPreference.setEnabled(isChecked);
        } else if (DozeUtils.ALWAYS_ON_DISPLAY.equals(preference.getKey())) {
            DozeUtils.enableAlwaysOn(getActivity(), (Boolean) newValue);
            if (!(Boolean) newValue) {
                mDozeBrightnessPreference.setValue(DozeUtils.DOZE_BRIGHTNESS_LBM);
                DozeUtils.setDozeMode(DozeUtils.DOZE_BRIGHTNESS_LBM);
            } else {
                mPickUpPreference.setChecked(false);
                mHandwavePreference.setChecked(false);
                mPocketPreference.setChecked(false);
            }
            mDozeBrightnessPreference.setEnabled((Boolean) newValue);
        } else if (DozeUtils.DOZE_BRIGHTNESS_KEY.equals(preference.getKey())) {
            if (!DozeUtils.DOZE_BRIGHTNESS_AUTO.equals((String) newValue)) {
                DozeUtils.setDozeMode((String) newValue);
            }
        }

        mHandler.post(() -> {
            DozeUtils.checkDozeService(getActivity());
            DozeUtils.updateDozeBrightnessIcon(getContext(), mDozeBrightnessPreference);
        });

        return true;
    }

}
