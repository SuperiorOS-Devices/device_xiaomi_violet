/* Copyright (c) 2020, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of The Linux Foundation, nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef DATAITEM_CONCRETETYPES_H
#define DATAITEM_CONCRETETYPES_H

#include <string>
#include <cstring>
#include <sstream>
#include <DataItemId.h>
#include <IDataItemCore.h>
#include <gps_extended_c.h>
#include <inttypes.h>

#define MAC_ADDRESS_LENGTH    6
// MAC address length in bytes
// QMI_LOC_SRN_MAC_ADDR_LENGTH_V02
#define SRN_MAC_ADDRESS_LENGTH    6
#define WIFI_SUPPLICANT_DEFAULT_STATE    0

#define TIME_DEFAULT_CURRTIME 0
#define TIMEZONE_DEFAULT_RAWOFFSET 0
#define TIMEZONE_DEFAULT_DSTOFFSET 0

#define NETWORKINFO_DEFAULT_TYPE 300
#define SERVICESTATUS_DEFAULT_STATE 3 /// OOO

#define BATTERY_PCT_DEFAULT 50

#define STRINGIFY_ERROR_CHECK_AND_DOWN_CAST(T, ID) \
if (getId() != ID) { result = 1; break; } \
T * d = static_cast<T *>(this);

// macro for copier
#define COPIER_ERROR_CHECK_AND_DOWN_CAST(T, ID) \
   if (src == NULL) { result = 1; break; } \
   if (getId() != src->getId()) { result = 2; break; } \
   if (getId() != ID) { result = 3; break; } \
   T * s = static_cast<T *>(this); \
   T * d = static_cast<T *>(src);


static constexpr char sDelimit = ':';

namespace loc_core {
using namespace std;

enum NetworkType {
    TYPE_MOBILE = 0,
    TYPE_WIFI,
    TYPE_ETHERNET,
    TYPE_BLUETOOTH,
    TYPE_MMS,
    TYPE_SUPL,
    TYPE_DUN,
    TYPE_HIPRI,
    TYPE_WIMAX,
    TYPE_PROXY,
    TYPE_UNKNOWN,
};

typedef struct NetworkInfoType {
    // Unique network handle ID
    uint64_t networkHandle;
    // Type of network for corresponding network handle
    NetworkType networkType;
    NetworkInfoType() : networkHandle(NETWORK_HANDLE_UNKNOWN), networkType(TYPE_UNKNOWN) {}
    NetworkInfoType(string strObj) {
        size_t posDelimit = strObj.find(sDelimit);

        if ( posDelimit != string::npos) {
            int32_t type = TYPE_UNKNOWN;
            string handleStr = strObj.substr(0, posDelimit);
            string typeStr = strObj.substr(posDelimit + 1, strObj.length() - posDelimit - 1);
            stringstream(handleStr) >> networkHandle;
            stringstream(typeStr) >> type;
            networkType = (NetworkType) type;
        } else {
            networkHandle = NETWORK_HANDLE_UNKNOWN;
            networkType = TYPE_UNKNOWN;
        }
    }
    bool operator== (const NetworkInfoType& other) {
        return ((networkHandle == other.networkHandle) && (networkType == other.networkType));
    }
    string toString() {
        string valueStr;
        valueStr.clear ();
        char nethandle [32];
        memset (nethandle, 0, 32);
        snprintf(nethandle, sizeof(nethandle), "%" PRIu64, networkHandle);
        valueStr += string(nethandle);
        valueStr += sDelimit;
        char type [12];
        memset (type, 0, 12);
        snprintf (type, 12, "%u", networkType);
        valueStr += string (type);
        return valueStr;
    }
} NetworkInfoType;


class AirplaneModeDataItem: public IDataItemCore  {
public:
    AirplaneModeDataItem(IDataItemCore* di):
            AirplaneModeDataItem(((AirplaneModeDataItem*)di)->mMode) {}
    AirplaneModeDataItem(bool mode = false):
        mMode(mode) {mId = AIRPLANEMODE_DATA_ITEM_ID;}
    virtual ~AirplaneModeDataItem() {}
    virtual void stringify(string& /*valueStr*/) override;
    virtual int32_t copyFrom(IDataItemCore* /*src*/) override;
// Data members
    bool mMode;

};

class ENHDataItem: public IDataItemCore {
public:
    ENHDataItem(bool enabled = false) :
        mEnabled(enabled) {mId = ENH_DATA_ITEM_ID;}
    virtual ~ENHDataItem() {}
    virtual void stringify(string& /*valueStr*/) override;
    virtual int32_t copyFrom(IDataItemCore* /*src*/) override;
// Data members
    bool mEnabled;
};

class GPSStateDataItem: public IDataItemCore {
public:
    GPSStateDataItem(bool enabled = false) :
        mEnabled(enabled) {mId = GPSSTATE_DATA_ITEM_ID;}
    virtual ~GPSStateDataItem() {}
    virtual void stringify(string& /*valueStr*/) override;
    virtual int32_t copyFrom(IDataItemCore* /*src*/) override;
// Data members
    bool mEnabled;
};

class NLPStatusDataItem: public IDataItemCore {
public:
    NLPStatusDataItem(bool enabled = false) :
        mEnabled(enabled) {mId = NLPSTATUS_DATA_ITEM_ID;}
    virtual ~NLPStatusDataItem() {}
    virtual void stringify(string& /*valueStr*/) override;
    virtual int32_t copyFrom(IDataItemCore* /*src*/) override;
// Data members
    bool mEnabled;
};

class WifiHardwareStateDataItem: public IDataItemCore {
public:
    WifiHardwareStateDataItem(bool enabled = false) :
        mEnabled(enabled) {mId = WIFIHARDWARESTATE_DATA_ITEM_ID;}
    virtual ~WifiHardwareStateDataItem() {}
    virtual void stringify(string& /*valueStr*/) override;
    virtual int32_t copyFrom(IDataItemCore* /*src*/) override;
// Data members
    bool mEnabled;
};

class ScreenStateDataItem: public IDataItemCore {
public:
    ScreenStateDataItem(bool state = false) :
        mState(state) {mId = SCREEN_STATE_DATA_ITEM_ID;}
    virtual ~ScreenStateDataItem() {}
    virtual void stringify(string& /*valueStr*/) override;
    virtual int32_t copyFrom(IDataItemCore* /*src*/) override;
// Data members
    bool mState;
};

class PowerConnectStateDataItem: public IDataItemCore {
public:
    PowerConnectStateDataItem(bool state = false) :
        mState(state) {mId = POWER_CONNECTED_STATE_DATA_ITEM_ID;}
    virtual ~PowerConnectStateDataItem() {}
    virtual void stringify(string& /*valueStr*/) override;
    virtual int32_t copyFrom(IDataItemCore* /*src*/) override;
// Data members
    bool mState;
};

class TimeZoneChangeDataItem: public IDataItemCore {
public:
    TimeZoneChangeDataItem(int64_t currTimeMillis = TIME_DEFAULT_CURRTIME,
            int32_t rawOffset = TIMEZONE_DEFAULT_RAWOFFSET,
            int32_t dstOffset = TIMEZONE_DEFAULT_DSTOFFSET) :
        mCurrTimeMillis (currTimeMillis),
        mRawOffsetTZ (rawOffset),
        mDstOffsetTZ (dstOffset) {mId = TIMEZONE_CHANGE_DATA_ITEM_ID;}
    virtual ~TimeZoneChangeDataItem() {}
    virtual void stringify(string& /*valueStr*/) override;
    virtual int32_t copyFrom(IDataItemCore* /*src*/) override;
// Data members
    int64_t mCurrTimeMillis;
    int32_t mRawOffsetTZ;
    int32_t mDstOffsetTZ;
};

class TimeChangeDataItem: public IDataItemCore {
public:
    TimeChangeDataItem(int64_t currTimeMillis = TIME_DEFAULT_CURRTIME,
            int32_t rawOffset = TIMEZONE_DEFAULT_RAWOFFSET,
            int32_t dstOffset = TIMEZONE_DEFAULT_DSTOFFSET) :
        mCurrTimeMillis (currTimeMillis),
        mRawOffsetTZ (rawOffset),
        mDstOffsetTZ (dstOffset) {mId = TIME_CHANGE_DATA_ITEM_ID;}
    virtual ~TimeChangeDataItem() {}
    virtual void stringify(string& /*valueStr*/) override;
    virtual int32_t copyFrom(IDataItemCore* /*src*/) override;
// Data members
    int64_t mCurrTimeMillis;
    int32_t mRawOffsetTZ;
    int32_t mDstOffsetTZ;
};

class ShutdownStateDataItem: public IDataItemCore {
public:
    ShutdownStateDataItem(bool state = false) :
        mState (state) {mId = SHUTDOWN_STATE_DATA_ITEM_ID;}
    virtual ~ShutdownStateDataItem() {}
    virtual void stringify(string& /*valueStr*/) override;
    virtual int32_t copyFrom(IDataItemCore* /*src*/) override;
// Data members
    bool mState;
};

class AssistedGpsDataItem: public IDataItemCore {
public:
    AssistedGpsDataItem(bool enabled = false) :
        mEnabled(enabled) {mId = ASSISTED_GPS_DATA_ITEM_ID;}
    virtual ~AssistedGpsDataItem() {}
    virtual void stringify(string& /*valueStr*/) override;
    virtual int32_t copyFrom(IDataItemCore* /*src*/) override;
// Data members
    bool mEnabled;
};

class NetworkInfoDataItem: public IDataItemCore {
public:
    NetworkInfoDataItem(
            int32_t type = NETWORKINFO_DEFAULT_TYPE,
            std::string typeName = "",
            std::string subTypeName = "",
            bool available = false,
            bool connected = false,
            bool roaming = false,
            uint64_t networkHandle = NETWORK_HANDLE_UNKNOWN, string apn = ""):
        NetworkInfoDataItem(getNormalizedType(type), type, typeName, subTypeName, available,
        connected, roaming, networkHandle, "") {}
    NetworkInfoDataItem(NetworkType initialType, int32_t type, string typeName,
            string subTypeName, bool available, bool connected, bool roaming,
            uint64_t networkHandle, string apn):
            mAllTypes(typeToAllTypes(initialType)),
            mType(type),
            mTypeName(typeName),
            mSubTypeName(subTypeName),
            mAvailable(available),
            mConnected(connected),
            mRoaming(roaming),
            mNetworkHandle(networkHandle), mApn(apn) {
                mId = NETWORKINFO_DATA_ITEM_ID;
                mAllNetworkHandles[0].networkHandle = networkHandle;
                mAllNetworkHandles[0].networkType = initialType;
            }
    virtual ~NetworkInfoDataItem() {}
    virtual void stringify(string& /*valueStr*/) override;
    virtual int32_t copyFrom(IDataItemCore* /*src*/) override;
    inline uint64_t getAllTypes() { return mAllTypes; }
    inline NetworkInfoType* getNetworkHandle() {
        return &mAllNetworkHandles[0];
    }
    inline virtual NetworkType getType(void) const {
        return getNormalizedType(mType);
    }
    inline static NetworkType getNormalizedType(int32_t type) {
        NetworkType typeout = TYPE_UNKNOWN;
        switch (type) {
            case 100:
                typeout = TYPE_WIFI;
                break;
            case 101:
                typeout = TYPE_ETHERNET;
                break;
            case 102:
                typeout = TYPE_BLUETOOTH;
                break;
            case 201:
                typeout = TYPE_MOBILE;
                break;
            case 202:
                typeout = TYPE_DUN;
                break;
            case 203:
                typeout = TYPE_HIPRI;
                break;
            case 204:
                typeout = TYPE_MMS;
                break;
            case 205:
                typeout = TYPE_SUPL;
                break;
            case 220:
                typeout = TYPE_WIMAX;
                break;
            case 300:
            default:
                typeout = TYPE_UNKNOWN;
                break;
        }
        return typeout;
   }
    // Data members
    uint64_t mAllTypes;
    int32_t mType;
    string mTypeName;
    string mSubTypeName;
    bool mAvailable;
    bool mConnected;
    bool mRoaming;
    NetworkInfoType mAllNetworkHandles[MAX_NETWORK_HANDLES];
    uint64_t mNetworkHandle;
    string mApn;
protected:
    inline uint64_t typeToAllTypes(NetworkType type) {
        return (type >= TYPE_UNKNOWN || type < TYPE_MOBILE) ?  0 : (1<<type);
    }
private:
    inline void setType(NetworkType type) {
        switch (type) {
            case TYPE_WIFI:
                mType = 100;
                break;
            case TYPE_ETHERNET:
                mType = 101;
                break;
            case TYPE_BLUETOOTH:
                mType = 102;
                break;
            case TYPE_MOBILE:
                mType = 201;
                break;
            case TYPE_DUN:
                mType = 202;
                break;
            case TYPE_HIPRI:
                mType = 203;
                break;
            case TYPE_MMS:
                mType = 204;
                break;
            case TYPE_SUPL:
                mType = 205;
                break;
            case TYPE_WIMAX:
                mType = 220;
                break;
            default:
                mType = 300;
                break;
        }
    }
};

class ServiceStatusDataItem: public IDataItemCore {
public:
    ServiceStatusDataItem(int32_t serviceState = SERVICESTATUS_DEFAULT_STATE) :
        mServiceState (serviceState) {mId = SERVICESTATUS_DATA_ITEM_ID;}
    virtual ~ServiceStatusDataItem() {}
    virtual void stringify(string& /*valueStr*/) override;
    virtual int32_t copyFrom(IDataItemCore* /*src*/) override;
// Data members
    int32_t mServiceState;
};

class ModelDataItem: public IDataItemCore {
public:
    ModelDataItem(const string & name = "") :
        mModel (name) {mId = MODEL_DATA_ITEM_ID;}
    virtual ~ModelDataItem() {}
    virtual void stringify(string& /*valueStr*/) override;
    virtual int32_t copyFrom(IDataItemCore* /*src*/) override;
// Data members
    string mModel;
};

class ManufacturerDataItem: public IDataItemCore {
public:
    ManufacturerDataItem(const string & name = "") :
        mManufacturer (name) {mId = MANUFACTURER_DATA_ITEM_ID;}
    virtual ~ManufacturerDataItem() {}
    virtual void stringify(string& /*valueStr*/) override;
    virtual int32_t copyFrom(IDataItemCore* /*src*/) override;
// Data members
    string mManufacturer;
};

class RilServiceInfoDataItem : public IDataItemCore {
public:
    inline RilServiceInfoDataItem() :
            mData(nullptr) {mId = RILSERVICEINFO_DATA_ITEM_ID;}
    inline virtual ~RilServiceInfoDataItem() { if (nullptr != mData) free(mData); }
    virtual void stringify(string& /*valueStr*/) {}
    virtual int32_t copyFrom(IDataItemCore* src) {
        memcpy(mData,  ((RilServiceInfoDataItem*)src)->mData, mLength);
        return 0;
    }
    inline RilServiceInfoDataItem(const RilServiceInfoDataItem& peer) :
            RilServiceInfoDataItem() {
        mLength = peer.mLength;
        mData = malloc(mLength);
        memcpy(mData,  peer.mData, mLength);
        peer.setPeerData(*this);
    }
    inline virtual bool operator==(const RilServiceInfoDataItem& other) const {
        return other.mData == mData;
    }
    inline virtual void setPeerData(RilServiceInfoDataItem& /*peer*/) const {}
    void* mData;
    int mLength;
};

class RilCellInfoDataItem : public IDataItemCore {
public:
    inline RilCellInfoDataItem() :
            mData(nullptr) {mId = RILCELLINFO_DATA_ITEM_ID;}
    inline virtual ~RilCellInfoDataItem() { if (nullptr != mData) free(mData); }
    virtual void stringify(string& /*valueStr*/) {}
    virtual int32_t copyFrom(IDataItemCore* src) {
        memcpy(mData,  ((RilCellInfoDataItem*)src)->mData, mLength);
        return 0;
    }
    inline RilCellInfoDataItem(const RilCellInfoDataItem& peer) :
            RilCellInfoDataItem() {
        mLength = peer.mLength;
        mData = malloc(mLength);
        memcpy(mData,  peer.mData, mLength);
        peer.setPeerData(*this);
    }
    inline virtual bool operator==(const RilCellInfoDataItem& other) const {
        return other.mData == mData;
    }
    inline virtual void setPeerData(RilCellInfoDataItem& /*peer*/) const {}
    void* mData;
    int mLength;
};

class WifiSupplicantStatusDataItem: public IDataItemCore {
public:
    WifiSupplicantStatusDataItem() :
        mState((WifiSupplicantState)WIFI_SUPPLICANT_DEFAULT_STATE),
        mApMacAddressValid(false),
        mWifiApSsidValid(false) {
            mId = WIFI_SUPPLICANT_STATUS_DATA_ITEM_ID;
            memset (mApMacAddress, 0, sizeof (mApMacAddress));
            mWifiApSsid.clear();
        }
    virtual ~WifiSupplicantStatusDataItem() {}
    virtual void stringify(string& /*valueStr*/) override;
    virtual int32_t copyFrom(IDataItemCore* /*src*/) override;
    // Data members
    typedef enum WifiSupplicantState {
        DISCONNECTED,
        INTERFACE_DISABLED,
        INACTIVE,
        SCANNING,
        AUTHENTICATING,
        ASSOCIATING,
        ASSOCIATED,
        FOUR_WAY_HANDSHAKE,
        GROUP_HANDSHAKE,
        COMPLETED,
        DORMANT,
        UNINITIALIZED,
        INVALID
    } WifiSupplicantState;
    /* Represents whether access point attach state*/
    WifiSupplicantState mState;
    /* Represents info on whether ap mac address is valid */
    bool mApMacAddressValid;
    /* Represents mac address of the wifi access point*/
    uint8_t mApMacAddress[MAC_ADDRESS_LENGTH];
    /* Represents info on whether ap SSID is valid */
    bool mWifiApSsidValid;
    /* Represents Wifi SSID string*/
    string mWifiApSsid;
};

class TacDataItem: public IDataItemCore {
public:
    TacDataItem(const string & name = "") :
        mValue (name) {mId = TAC_DATA_ITEM_ID;}
    virtual ~TacDataItem() {}
    virtual void stringify(string& /*valueStr*/) override;
    virtual int32_t copyFrom(IDataItemCore* /*src*/) override;
// Data members
    string mValue;
};

class MccmncDataItem: public IDataItemCore {
public:
    MccmncDataItem(const string & name = "") :
        mValue(name) {mId = MCCMNC_DATA_ITEM_ID;}
    virtual ~MccmncDataItem() {}
    virtual void stringify(string& /*valueStr*/) override;
    virtual int32_t copyFrom(IDataItemCore* /*src*/) override;
// Data members
    string mValue;
};

class SrnDeviceScanDetailsDataItem: public IDataItemCore {
public:
    SrnDeviceScanDetailsDataItem(DataItemId Id) :
        mValidSrnData(false),
        mApSrnRssi(-1),
        mApSrnTimestamp(0),
        mRequestTimestamp(0),
        mReceiveTimestamp(0),
       mErrorCause(-1) {mId = Id;}
    virtual ~SrnDeviceScanDetailsDataItem() {}
    // Data members common to all SRN tech types
    /* Represents info on whether SRN data is valid (no error)*/
    bool mValidSrnData;
    /* SRN device RSSI reported */
    int32_t mApSrnRssi;
    /* MAC adress of SRN device */
    uint8_t mApSrnMacAddress[SRN_MAC_ADDRESS_LENGTH];
    /* UTC timestamp at which the scan was requested.for this SRN device*/
    int64_t mApSrnTimestamp;
    /* UTC timestamp at which the scan was started. */
    int64_t mRequestTimestamp;
    /* UTC timestamp at which the scan was received.*/
    int64_t mReceiveTimestamp;
    /* Reason for the error/failure if SRN details are not valid */
    int32_t mErrorCause;
};

class BtDeviceScanDetailsDataItem: public SrnDeviceScanDetailsDataItem {
public:
    BtDeviceScanDetailsDataItem() :
        SrnDeviceScanDetailsDataItem(BT_SCAN_DATA_ITEM_ID) {}
    virtual ~BtDeviceScanDetailsDataItem() {}
    virtual void stringify(string& /*valueStr*/) override;
    virtual int32_t copyFrom(IDataItemCore* /*src*/) override;
};

class BtLeDeviceScanDetailsDataItem: public SrnDeviceScanDetailsDataItem {
public:
    BtLeDeviceScanDetailsDataItem() :
        SrnDeviceScanDetailsDataItem(BTLE_SCAN_DATA_ITEM_ID) {}
    virtual ~BtLeDeviceScanDetailsDataItem() {}
    virtual void stringify(string& /*valueStr*/) override;
    virtual int32_t copyFrom(IDataItemCore* /*src*/) override;
};

class BatteryLevelDataItem: public IDataItemCore {
public:
    inline BatteryLevelDataItem(uint8_t batteryPct = BATTERY_PCT_DEFAULT) :
            mBatteryPct(batteryPct) {mId = BATTERY_LEVEL_DATA_ITEM_ID;}
    inline ~BatteryLevelDataItem() {}
    virtual void stringify(string& /*valueStr*/) override;
    virtual int32_t copyFrom(IDataItemCore* /*src*/) override;
// Data members
    uint8_t mBatteryPct;
};

} // namespace loc_core

#endif //DATAITEM_CONCRETETYPES_H
