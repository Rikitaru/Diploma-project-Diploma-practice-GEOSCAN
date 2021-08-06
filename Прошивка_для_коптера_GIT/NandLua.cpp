void NandLua::luaRegister(sol::state_view &aLua)
{
	aLua.create_named_table("NandLua",
		"readPositionOrigin", sol::as_function(&NandLua::readPositionOrigin, this),
        "getRotationOrigin", sol::as_function(&NandLua::getRotationOrigin, this));
}
Gnss::Position NandLua::readPositionOrigin()
{
	Gnss::Position position{0, 0, 0};
    CoordinateBridgeSingleton &coordinateBridgeSingleton = CoordinateBridgeSingleton::getInstance();
    if (coordinateBridgeSingleton.flag){
        position.altitude  = coordinateBridgeSingleton.newAcceptGnssPosition.altitude;
        position.latitude  = coordinateBridgeSingleton.newAcceptGnssPosition.latitude;
        position.longitude = coordinateBridgeSingleton.newAcceptGnssPosition.longitude;
    }
    else{
        position.latitude = readValue<float>(kShowParameters[ParamDescriptor::LatOrigin].position);
        position.longitude = readValue<float>(kShowParameters[ParamDescriptor::LonOrigin].position);
        position.altitude = readValue<float>(kShowParameters[ParamDescriptor::AltOrigin].position);
    }
	return position;
}

float NandLua::getRotationOrigin()
{
    volatile float external_rotation = 0.0;
    CoordinateBridgeSingleton &coordinateBridgeSingleton = CoordinateBridgeSingleton::getInstance();
    if (coordinateBridgeSingleton.flag){
        external_rotation  = coordinateBridgeSingleton.newAcceptGnssPosition.rotation; //возвращает угол не в радианах
    }
    return external_rotation;
}