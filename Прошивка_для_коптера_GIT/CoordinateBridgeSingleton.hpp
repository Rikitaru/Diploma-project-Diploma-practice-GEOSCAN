#ifndef COORDINATEBRIDGESINGLETON_H
#define COORDINATEBRIDGESINGLETON_H
/*
 * CoordinateBridgeSingleton.hpp
 *
 *  Created on: June 30, 2021
 *      Author: dkorobkov
 */
#include "Types.hpp"
#include <cmath>
class CoordinateBridgeSingleton;

class CoordinateBridgeSingletonDestroyer
{
  private:
    CoordinateBridgeSingleton* p_instance;
  public:
    ~CoordinateBridgeSingletonDestroyer();
    void initialize( CoordinateBridgeSingleton* p );
};

class CoordinateBridgeSingleton
{
  private:
    static CoordinateBridgeSingleton* p_instance;
    static CoordinateBridgeSingletonDestroyer destroyer;
  protected:
    CoordinateBridgeSingleton() { }
    CoordinateBridgeSingleton( const CoordinateBridgeSingleton& );
    CoordinateBridgeSingleton& operator=( CoordinateBridgeSingleton& );
   ~CoordinateBridgeSingleton() { }
    friend class CoordinateBridgeSingletonDestroyer;
  public:
    bool flag = false;
    NewStartPoint newAcceptGnssPosition;
    void test(float alt, double lat, double longitude, float rot){
        newAcceptGnssPosition.altitude = alt;
        newAcceptGnssPosition.rotation = rot;
        newAcceptGnssPosition.latitude = lat;
        newAcceptGnssPosition.longitude = longitude;
        flag=true;
        return;
    }
    static CoordinateBridgeSingleton& getInstance();
};

#endif // COORDINATEBRIDGESINGLETON_H
