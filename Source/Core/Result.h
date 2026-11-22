#pragma once
#include <Core/Types.h>
#include <string>

class Result final {
public:
	Result(std::string_view msg = "") : _msg(msg) {}
	~Result() = default;

	std::string GetMessage() const { return _msg.data(); }

	operator b8() const { return _msg.empty(); }
private:
	std::string_view _msg;
};