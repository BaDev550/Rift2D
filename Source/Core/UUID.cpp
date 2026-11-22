#include <Core/UUID.h>

UUID::UUID() {
	std::random_device rd;
	std::mt19937_64 gen(rd());
	std::uniform_int_distribution<u64> dis;
	_uuid = dis(gen);
}

UUID::UUID(u64 uuid) : _uuid(uuid) {}
UUID::UUID(std::string_view uuidStr) { _uuid = std::hash<std::string>{}(uuidStr.data()); }

b8 UUID::IsValid() const { return _uuid != InvalidUUID; }
std::string UUID::Str() const { return std::to_string(_uuid); }