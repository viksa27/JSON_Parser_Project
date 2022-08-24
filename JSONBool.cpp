#include "JSONBool.h"


JSONBool::JSONBool(bool val) : JSON(JSONType::JSON_BOOL) {
	this->value = val;
}
JSONBool::JSONBool() : JSON(JSONType::JSON_BOOL) {
	this->value = false;
}

bool JSONBool::getValue() const {
	return this->value;
}


void JSONBool::print() const {
	if (value) {
		std::cout << "true";
	}
	else {
		std::cout << "false";
	}
}

JSON* JSONBool::clone() const {
	JSON* clone = new JSONBool(this->value);
	return clone;
}

const int JSONBool::findEndIndex(const std::string& rawJSON, int startIndex) const {
	if (rawJSON[startIndex] == 't') {
		if (startIndex + 3 > rawJSON.length())
			return -1;
		return startIndex + 3;
	}
	else if (rawJSON[startIndex] == 'f') {
		if (startIndex + 4 > rawJSON.length())
			return -1;
		return startIndex + 4;
	}
	else
		return -1;
}

void JSONBool::save(std::ofstream& openedFile) {
	if (this->value)
		openedFile << "true";
	else
		openedFile << "false";
}