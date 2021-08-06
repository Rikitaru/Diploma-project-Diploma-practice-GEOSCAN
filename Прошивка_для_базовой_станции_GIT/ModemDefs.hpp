class Modem {
public:
	enum class Command: uint8_t {
        SET_NEW_STARTING_POINT   = 0x43,
    };

	enum Ids: uint8_t {
		MASTER_ID,
		BROADCASTING_ID = 0xFF,
		UNASSIGNED_ID = BROADCASTING_ID,
	};

	enum class Result {
		SUCCESS = 0x00,
		ERROR
	};
};