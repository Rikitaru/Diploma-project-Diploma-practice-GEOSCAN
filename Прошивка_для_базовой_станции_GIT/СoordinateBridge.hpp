/*
 * CoordinateBridge.hpp
 *
 *  Created on: June 30, 2021
 *      Author: dkorobkov
 */
#ifndef COORDINATEBRIDGE_H
#define COORDINATEBRIDGE_H_H
#include "Devices/BridgeDevice.hpp"
#include "Types.hpp"
#include "Drivers/Network/NetworkInitializer.hpp"
#include <Os/SoftwareTimer.hpp>
#include "WorkQueue.hpp"
typedef float Altitude; //!< Metres
typedef double Latitude; //!< Metres
typedef double Longitude; //!< Metres
typedef float Rotation; //!< Metres
struct State{
    bool flag_altitude;
    bool flag_latitude;
    bool flag_longitude;
    bool flag_rotation;
    State()  {
      flag_altitude = 0;
      flag_latitude = 0;
      flag_longitude = 0;
      flag_rotation = 0;
    }
    bool check_box()  {
      return ((flag_altitude == 1) && (flag_latitude == 1) && (flag_longitude == 1) && (flag_rotation == 1));
    }
    void reset (){
        flag_altitude = 0;
        flag_latitude = 0;
        flag_longitude = 0;
        flag_rotation = 0;
    }
};
class CoordinateBridge: public BridgeDevice,  public Os::SoftwareTimerObserver {
public:
        Os::SoftwareTimer timer;
        RadioHandler *ptr_RadioHandler = NULL;
        NewStartPoint newStartPoint;
        State state;
        int count = 10;
        int time = 1000;
        bool flag = false;
        virtual void onSoftwareTimerOverflow() override{
            WorkQueue::LowPriority::push([this] { generation(); });
            if (count>0){
                timer.stop();
                timer.start(time);
            }
            else{
                flagEntry = false;
            }
            countEntry= count;
        }

        CoordinateBridge(const char *aDeviceName = "CoordinateBridge"):
                  BridgeDevice{AircraftDevice::DeviceType::UNSPECIFIED, aDeviceName},
                  timer{},
                  altitudeEntry("altitude",*this),
                  latitudeEntry("latitude",*this),
                  longitudeEntry("longitude",*this),
                  rotationEntry("rotation",*this),
                  flagEntry("flag",*this),
                  timeEntry("time", *this),
                  countEntry("count", *this)
                {
                    timer.subscribe(this),
                    appendEntry(&flagEntry);
                    appendEntry(&timeEntry);
                    appendEntry(&countEntry);
                    appendEntry(&altitudeEntry);
                    appendEntry(&latitudeEntry);
                    appendEntry(&longitudeEntry);
                    appendEntry(&rotationEntry);
                }
        void generation(){
            --count;
            void *ptr_data  = &newStartPoint;
            ptr_RadioHandler->sendStartingPoint(ptr_data , sizeof(NewStartPoint));
        }
protected:
        class DevEntryFlag : public Device::VolatileField<bool> {
        private:
            CoordinateBridge &bridge;
        public:
            DevEntryFlag(const char *aName, CoordinateBridge &aBridge) :
                Device::VolatileField<bool>{aName, false},
                bridge{aBridge}
            {
            }
            Device::Result write (const void *aInput) override
            {
                Device::VolatileField<bool>::write(aInput);
                bridge.flag = *static_cast<const bool *>(aInput);
                if ((bridge.state.check_box()) && (bridge.flag == true)) {
                    --(bridge.count);
                    bridge.timer.stop();
                    bridge.timer.start(bridge.time);
                    //bridge.state.reset();
                }
                return Device::Result::SUCCESS;
            }
            Device::VolatileField<bool> &operator=(const bool &val)
            {
                Device::VolatileField<bool>::operator=(static_cast<bool>(val));
                return *this;
            }
        };
        class DevEntryTime : public Device::VolatileField<int> {
        private:
            CoordinateBridge &bridge;
        public:
            DevEntryTime(const char *aName, CoordinateBridge &aBridge) :
                Device::VolatileField<int>{aName, static_cast<int>(1000)},
                bridge{aBridge}
            {
            }

            Device::Result write (const void *aInput) override
            {
                Device::VolatileField<int>::write(aInput);
                bridge.time = *static_cast<const int *>(aInput);
                return Device::Result::SUCCESS;
            }
        };
        class DevEntryCount : public Device::VolatileField<int> {
        private:
            CoordinateBridge &bridge;
        public:
            DevEntryCount(const char *aName, CoordinateBridge &aBridge) :
                Device::VolatileField<int>{aName, static_cast<int>(10)},
                bridge{aBridge}
            {
            }
            Device::Result write (const void *aInput) override
            {
                Device::VolatileField<int>::write(aInput);
                bridge.count = *static_cast<const int *>(aInput);
                return Device::Result::SUCCESS;
            }
            Device::VolatileField<int> &operator=(const int &val)
            {
                Device::VolatileField<int>::operator=(static_cast<int>(val));
                return *this;
            }
        };
        template<typename T>
        class DevEntry : public Device::VolatileField<T> {
        private:
            CoordinateBridge &bridge;
        public:
            DevEntry(const char *aName, CoordinateBridge &aBridge) :
                Device::VolatileField<T>{aName, static_cast<T>(-1)},
                bridge{aBridge}
            {
            }
            Device::Result write (const void *aInput) override
            {
                Device::VolatileField<T>::write(aInput);
                if(strcmp(this->nameString,"altitude")==0){
                    bridge.newStartPoint.altitude = *static_cast<const Altitude *>(aInput);
                    bridge.state.flag_altitude=1;
                }
                else if (strcmp(this->nameString,"latitude")==0){
                    bridge.newStartPoint.latitude = *static_cast<const Latitude *>(aInput);
                    bridge.state.flag_latitude=1;
                }
                else if (strcmp(this->nameString,"longitude")==0){
                    bridge.newStartPoint.longitude = *static_cast<const Longitude *>(aInput);
                    bridge.state.flag_longitude=1;
                }
                else if (strcmp(this->nameString,"rotation")==0){
                    bridge.newStartPoint.rotation = *static_cast<const Rotation *>(aInput);
                    bridge.state.flag_rotation=1;
                }
                return Device::Result::SUCCESS;
            }
            Device::VolatileField<T> &operator=(const T &val)
            {
                Device::VolatileField<T>::operator=(static_cast<T>(val));
                return *this;
            }
        };
        DevEntry<Altitude> altitudeEntry;
        DevEntry<Latitude> latitudeEntry;
        DevEntry<Longitude> longitudeEntry;
        DevEntry<Rotation> rotationEntry;
        DevEntryFlag flagEntry;
        DevEntryTime timeEntry;
        DevEntryCount countEntry;
};
#endif // COORDINATEBRIDGE_H
