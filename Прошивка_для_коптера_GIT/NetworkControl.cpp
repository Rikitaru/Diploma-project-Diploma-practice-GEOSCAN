void NetworkControl::process(ModemProto::ModemLongestPacket *packet)
{
	if (packet == nullptr || !packet->header.length) {
		return;
	}
	const auto packetLen = packet->header.length;
	const auto cmd = static_cast<Modem::Command>(packet->payload[0]);
	auto *payload = packet->payload + sizeof(packet->payload[0]);
	packet->payload[0] = static_cast<uint8_t>(Modem::Result::SUCCESS);
	packet->header.length = static_cast<uint8_t>(sizeof(packet->payload[0]));
	switch(cmd) {
        case Modem::Command::SET_NEW_STARTING_POINT: {
            if (packet->header.dst != Modem::BROADCASTING_ID || Network::devId != Modem::UNASSIGNED_ID) {
                packet->header.length = 0;
                return;
            }
            /*void RadioDataHandler::handleStartingPointMessage(const void *data, size_t length)
            {
                memcpy(newStartingPointBuffer, data, length);
                memcpy(&newStartingPoint.altitude, newStartingPointBuffer, sizeof(float));
                memcpy(&newStartingPoint.latitude, newStartingPointBuffer + sizeof(float), sizeof(double));
                memcpy(&newStartingPoint.longitude, newStartingPointBuffer + sizeof(float) + sizeof(double), sizeof(double));
                memcpy(&newStartingPoint.rotation, newStartingPointBuffer + sizeof(float) + sizeof(double) + sizeof(double), sizeof(float));
                Singleton &coordinateBridgeSingleton = Singleton::getInstance();
                coordinateBridgeSingleton.test(newStartingPoint.altitude, newStartingPoint.latitude, newStartingPoint.longitude, newStartingPoint.rotation);
            }
            */
            uint8_t *ptr = (packet->payload)+1;
            float* ptr_float = new float;
            memcpy(ptr_float, ptr, sizeof(float));
            volatile float altitude = *ptr_float;
            ptr += sizeof(float);

            double* ptr_double = new double;
            memcpy(ptr_double, ptr, sizeof(double));
            volatile double latitude = *ptr_double;
            ptr += sizeof(double);

            memcpy(ptr_double, ptr, sizeof(double));
            volatile double longitude = *ptr_double;
            ptr += sizeof(double);

            memcpy(ptr_float, ptr, sizeof(float));
            volatile float rotation = *ptr_float;

            CoordinateBridgeSingleton &coordinateBridgeSingleton = CoordinateBridgeSingleton::getInstance();
            coordinateBridgeSingleton.test(altitude,latitude,longitude,rotation);

            delete ptr_double;
            delete ptr_float;
            break;
        }
		default:
			packet->header.length = 0;
			break;
	}
	packet->header.dst = packet->header.src;
	packet->header.src = Network::devId;
	return;
}