/*
 * CoordinateBridgeSingleton.cpp
 *
 *  Created on: June 30, 2021
 *      Author: dkorobkov
 */
#include "Devices/Modules/CoordinateBridgeSingleton.hpp"
CoordinateBridgeSingleton *CoordinateBridgeSingleton::p_instance = 0;
CoordinateBridgeSingletonDestroyer CoordinateBridgeSingleton::destroyer;

CoordinateBridgeSingletonDestroyer::~CoordinateBridgeSingletonDestroyer() {
    delete p_instance;
}
void CoordinateBridgeSingletonDestroyer::initialize( CoordinateBridgeSingleton* p ) {
    p_instance = p;
}
CoordinateBridgeSingleton& CoordinateBridgeSingleton::getInstance() {
    if(!p_instance)     {
        p_instance = new CoordinateBridgeSingleton();
        destroyer.initialize( p_instance);
    }
    return *p_instance;
}
