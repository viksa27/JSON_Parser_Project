#include "JSONInteger.h"

JSONInteger::JSONInteger(int val) : JSON(JSONType::JSON_INTEGER) {
	this->value = val;
}

JSONInteger::JSONInteger() : JSON(JSONType::JSON_INTEGER) {
	this->value = 0;
}

JSON* JSONInteger::clone() const {
	return new JSONInteger(value);
}

const int JSONInteger::findEndIndex(const std::string& rawJSON, int startIndex) const {

	int length = rawJSON.length();
	for (startIndex; startIndex < length; startIndex++) {
 		if (rawJSON[startIndex] < '0' || rawJSON[startIndex] > '9') {
			return startIndex;
		}
	}
	return length;
}


void JSONInteger::print() const {
	std::cout << std::to_string(value);
}

void JSONInteger::save(std::ofstream& openedFile) {
	openedFile << this->value;
}