#include "wifiinfopack.h"

WifiInfoPack::WifiInfoPack(){};
WifiInfoPack::WifiInfoPack(std::string SSID,double frequency,int Ampitude)
{
    _ID=SSID;
    _freq=frequency;
    _A=Ampitude;
}

