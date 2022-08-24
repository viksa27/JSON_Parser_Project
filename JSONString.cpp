#include "JSONString.h"

JSONString::JSONString(std::string val) : JSON(JSONType::JSON_STRING) {
	this->value = val;
}

JSONString::JSONString() : JSON(JSONType::JSON_STRING) {
	this->value = "";
}

void JSONString::print() const {
	std::cout << "\"" << this->value << "\"";
}

JSON* JSONString::clone() const {
	return new JSONString(this->value);
}

const int JSONString::findEndIndex(const std::string& rawJSON, int startIndex) const {

	int length = rawJSON.length();
	for (startIndex; startIndex < length; startIndex++) {
		if (rawJSON[startIndex] == '"' && rawJSON[startIndex - 1] != '\\') {
			return startIndex;
		}
	}
	return -1;
}

void JSONString::save(std::ofstream& openedFile) {
	openedFile << "\"";
	openedFile << this->value;
	openedFile << "\"";
}