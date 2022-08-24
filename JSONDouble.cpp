
#include "JSONDouble.h"

JSONDouble::JSONDouble(double val) : JSON(JSONType::JSON_DOUBLE) {
	this->value = val;
}

JSONDouble::JSONDouble() : JSON(JSONType::JSON_DOUBLE) {
	this->value = 0;
}

void JSONDouble::print() const {
	std::cout << std::to_string(value);
}

JSON* JSONDouble::clone() const {
	return new JSONDouble(this->value);
}

const int JSONDouble::findEndIndex(const std::string& rawJSON, int startIndex) const {

	int length = rawJSON.length();
	for (startIndex; startIndex < length; startIndex++) {
		if (rawJSON[startIndex] == '.') {
			startIndex++;
			break;
		}
	}
	if (rawJSON[startIndex] < '0' || rawJSON[startIndex] > '9') {
		return -1;
	}
	for (startIndex; startIndex < length; startIndex++) {
		if (rawJSON[startIndex] < '0' || rawJSON[startIndex] > '9') {
			return startIndex;
		}
	}
	return length;
}

void JSONDouble::save(std::ofstream& openedFile) {
	openedFile << this->value;
}