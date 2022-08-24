#include "JSONNull.h"

JSONNull::JSONNull() : JSON(JSONType::JSON_NULL) { }

void JSONNull::print() const {
	std::cout << "null";
}

JSON* JSONNull::clone() const {
	return new JSONNull();
}
const int JSONNull::findEndIndex(const std::string& rawJSON, int startIndex) const {
	if (startIndex + 3 > rawJSON.length())
		return -1;
	return startIndex + 3;
}

void JSONNull::save(std::ofstream& openedFile) {
	openedFile << "null";
}