#ifndef WIFIINFOPACK_H
#define WIFIINFOPACK_H

#include<string>
class WifiInfoPack
{
public:
    WifiInfoPack();
    WifiInfoPack(std::string SSID,double frequency,int Ampitude);
    inline std::string getID(){return _ID;}
    inline double getFreq(){return _freq;}
    inline int getAmp(){return _A;}

private:
    std::string _ID;
    double _freq;
    int _A;
};

#endif // WIFIINFOPACK_H
