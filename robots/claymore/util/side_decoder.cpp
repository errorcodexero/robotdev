#include "side_decoder.h"


sideDecoder::sideDecoder(const std::string& sides) {
	sideString = sides;
}

sideDecoder::side sideDecoder::nearSwitch() {
	if (sideString[0] == 'L') {
		return side::LEFT;
	}
	else {
		return side::RIGHT;
	}
}
sideDecoder::side sideDecoder::scale() {
	if (sideString[1] == 'L') {
		return side::LEFT;
	}
	else {
		return side::RIGHT;
	}
}
sideDecoder::side sideDecoder::farSwitch() {
	if (sideString[2] == 'L') {
		return side::LEFT;
	}
	else {
		return side::RIGHT;
	}
}