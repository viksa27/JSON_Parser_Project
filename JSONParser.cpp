#include "JSONParser.h"


JSONParser::JSONParser() : parsedJSON(nullptr) { }

JSON* JSONParser::getObject() const {
	if (this->parsedJSON) {
		return this->parsedJSON->clone();
	}
	else return nullptr;
}

bool JSONParser::parseJSONString(const std::string& json) {
	this->clear();
	this->rawJSON = json;
	int index = 0;
	try {
		parseRawJSONString(index, &this->parsedJSON);
	}
	catch (std::invalid_argument& e) {
		std::cerr << e.what() << std::endl;
		if (this->parsedJSON) {
			delete this->parsedJSON;
			this->parsedJSON = nullptr;
		}
		return false;
	}
	if (index < rawJSON.length()) {
		error(index);
		std::cout << "\nError - expected end of file.";
		delete this->parsedJSON;
		this->parsedJSON = nullptr;
		return false;
	}
	return true;
}

bool JSONParser::parseJSONFile(const char* source) {
	this->clear();
	std::ifstream file;
	file.open(source);
	if (!file.is_open()) {
		std::cout << "File not found!\n";
		return false;
	}
	readRawJSON(file);
	file.close();
	int index = 0;
	try {
		parseRawJSONString(index, &this->parsedJSON);
	}
	catch (std::invalid_argument& e) {
		std::cerr << e.what() << std::endl;
		if (this->parsedJSON) {
			delete this->parsedJSON;
			this->parsedJSON = nullptr;
		}
		return false;
	}
	if (index < rawJSON.length()) {
		error(index);
		std::cout << "\nError - expected end of file.";
		delete this->parsedJSON;
		this->parsedJSON = nullptr;
		return false;
	}
	return true;
}

JSONParser::~JSONParser() {
	if (parsedJSON)
		delete parsedJSON;
}

void JSONParser::clear() {
	if (this->parsedJSON) {
		delete parsedJSON;
	}
	parsedJSON = nullptr;
}


bool JSONParser::isDouble(int startIndex) const {
	int length = rawJSON.length();
	for (startIndex; startIndex < length; startIndex++) {
		if (rawJSON[startIndex] == '.')
			return true;
		if (rawJSON[startIndex] < '0' || rawJSON[startIndex] > '9') {
			return false;
		}
	}
	return false;
}

int JSONParser::findLineError(int endIndex) {
	if (endIndex < 0) {
		throw std::invalid_argument("\nError - start index can't be less than zero!");
	}
	int line = 1;
	for (int i = endIndex; i >= 0; i--) {
		if (rawJSON[i] == '\n') {
			line++;
		}
	}
	return line;
}

void JSONParser::error(int index) {
	int startErrorIndex, endErrorIndex;
	int lineError = findLineError(index);
	calculateErrorPrintIndexes(index, startErrorIndex, endErrorIndex);
	std::cout << "Parse error at index: " << index << ", line " << lineError << ".\n";
	printError(startErrorIndex, endErrorIndex);
}

void JSONParser::calculateErrorPrintIndexes(int index, int& startErrorIndex, int& endErrorIndex) {
	startErrorIndex = index - 10;
	endErrorIndex = index + 10;
	if (startErrorIndex < 0) {
		startErrorIndex = 0;
	}
	if (endErrorIndex > rawJSON.length()) {
		endErrorIndex = rawJSON.length() - 1;
	}
}

void JSONParser::printError(int& startErrorIndex, int& endErrorIndex) {
	if (startErrorIndex >= 0 && endErrorIndex < rawJSON.length()) {
		bool moveArrows = false;
		if (startErrorIndex > 0) {
			std::cout << ".... ";
			moveArrows = true;
		}
		for (int i = startErrorIndex; i <= endErrorIndex; i++) {
			if (rawJSON[i] == '\n' && endErrorIndex < rawJSON.length()) {
				endErrorIndex++;
				continue;
			}
			else if (rawJSON[i] == '\n') continue;
			std::cout << rawJSON[i];
		}
		if (endErrorIndex < rawJSON.length() - 1) {
			std::cout << " ....";
		}
		std::cout << std::endl;
		if (moveArrows) {
			std::cout << "     ";
		}
		int arrowCounter = startErrorIndex;
		for (int i = startErrorIndex; i <= endErrorIndex; i++) {
			if (rawJSON[i] == '\t') {
				arrowCounter -= 4;
			}
		}
		for (int i = arrowCounter; i <= endErrorIndex; i++) {
			std::cout << "^";
		}
	}
}


int JSONParser::readJSONInteger(int& startIndex, int& endIndex) const {
	std::string val;
	for (int i = startIndex; i < endIndex; i++) {
		val += rawJSON[i];
	}
	startIndex += val.length();
	return atoi(val.c_str());
}

double JSONParser::readJSONDouble(int& startIndex, int& endIndex) const {
	std::string val;
	for (int i = startIndex; i < endIndex; i++) {
		val += rawJSON[i];
	}
	startIndex += val.length();
	return std::stod(val);
}

std::string JSONParser::readJSONString(int& startIndex, int& endIndex) const {
	std::string val;
	for (int i = startIndex; i < endIndex; i++) {
		val += rawJSON[i];
	}
	startIndex += val.length() + 1;
	return val;
}


bool JSONParser::verifyTrue(int& startIndex) const {
	if (startIndex + 4 > rawJSON.length()) {
		return false;
	}
	if (rawJSON[startIndex + 1] == 'r')
		if (rawJSON[startIndex + 2] == 'u')
			if (rawJSON[startIndex + 3] == 'e')
				return true;
	return false;
}

bool JSONParser::verifyFalse(int& startIndex) const {
	if (startIndex + 5 > rawJSON.length()) {
		return false;
	}
	if (rawJSON[startIndex + 1] == 'a')
		if (rawJSON[startIndex + 2] == 'l')
			if (rawJSON[startIndex + 3] == 's')
				if (rawJSON[startIndex + 4] == 'e')
					return true;
	return false;
}

bool JSONParser::verifyNull(int& startIndex) const {
	if (startIndex + 4 > rawJSON.length()) {
		return false;
	}
	if (rawJSON[startIndex + 1] == 'u')
		if (rawJSON[startIndex + 2] == 'l')
			if (rawJSON[startIndex + 3] == 'l')
				return true;
	return false;
}

void JSONParser::parseInteger(int& startIndex, JSON** obj) {
	int endIndex = (*obj)->findEndIndex(this->rawJSON, startIndex);
	delete (*obj);
	if (endIndex == -1) {
		(*obj) = nullptr;
		error(startIndex);
		throw std::invalid_argument("\nError - expected a number.");
	}
	*obj = new JSONInteger(readJSONInteger(startIndex, endIndex));
}

void JSONParser::parseDouble(int& startIndex, JSON** obj) {
	int endIndex = (*obj)->findEndIndex(this->rawJSON, startIndex);
	delete (*obj);
	if (endIndex == -1) {
		(*obj) = nullptr;
		error(startIndex);
		throw std::invalid_argument("\nError - expected a number.");
	}
	*obj = new JSONDouble(readJSONDouble(startIndex, endIndex));
}

void JSONParser::parseString(int& startIndex, JSON** obj) {
	int endIndex = (*obj)->findEndIndex(this->rawJSON, startIndex);
	delete (*obj);
	if (endIndex == -1) {
		(*obj) = nullptr;
		error(startIndex);
		throw std::invalid_argument("\nError - expected \". End of the string not found");
	}
	*obj = new JSONString(readJSONString(startIndex, endIndex));
}

void JSONParser::parseNull(int& startIndex, JSON** obj) {
	if (verifyNull(startIndex)) {
		startIndex += 4;
	}
	else {
		delete (*obj);
		(*obj) = nullptr;
		error(startIndex);
		throw std::invalid_argument("\nError - invalid input. Did you mean \"null\"?");;
	}
}

void JSONParser::parseTrueBool(int& startIndex, JSON** obj) {
	if (verifyTrue(startIndex)) {
		startIndex += 4;
	}
	else {
		delete (*obj);
		(*obj) = nullptr;
		error(startIndex);
		throw std::invalid_argument("\nError - invalid input. Did you mean \"true\"?");
	}
}

void JSONParser::parseFalseBool(int& startIndex, JSON** obj) {
	if (verifyFalse(startIndex)) {
		startIndex += 5;
	}
	else {
		delete (*obj);
		(*obj) = nullptr;
		error(startIndex);
		throw std::invalid_argument("\nError - invalid input. Did you mean \"false\"?");
	}
}

void JSONParser::parseObject(int& startIndex, JSON** obj) {
	bool emptyObject = true;
	int endIndex = (*obj)->findEndIndex(this->rawJSON, startIndex);
	if (endIndex == -1) {
		delete (*obj);
		(*obj) = nullptr;
		error(startIndex);
		throw std::invalid_argument("\nError - expected }. End of the object not found");
	}
	while (startIndex < endIndex) {
		emptyObject = false;
		JSON* objectTypeGetter = checkFirstCharacter(startIndex);
		JSONType type = objectTypeGetter->getType();
		if (objectTypeGetter) {
			delete objectTypeGetter;
		}
		else {
			delete (*obj);
			(*obj) = nullptr;
			error(startIndex);
			throw std::invalid_argument("\nError - expected a key (string).");
		}

		if (type != JSONType::JSON_STRING) {
			delete (*obj);
			(*obj) = nullptr;
			error(startIndex);
			throw std::invalid_argument("\nError - expected a key (string).");
		}
		else {
			std::string newKey;
			JSONString stringIndexFinder;
			int stringEndIndex = stringIndexFinder.findEndIndex(rawJSON, startIndex);
			if (stringEndIndex == -1) {
				delete (*obj);
				(*obj) = nullptr;
				error(startIndex);
				throw std::invalid_argument("\nError - expected \". End of the object key (string) not found");
			}
			newKey = readJSONString(startIndex, stringEndIndex);
			ignoreWhiteSpaces(startIndex);
			if (rawJSON[startIndex] != ':') {
				delete (*obj);
				(*obj) = nullptr;
				error(startIndex);
				throw std::invalid_argument("\nError - expected colon(:) after the object key.");
			}
			else {
				startIndex++;
			}
			JSONObject* objectCast = dynamic_cast<JSONObject*>(*obj);
			JSON* newObject = nullptr;
			try {
				parseRawJSONString(startIndex, &newObject);
			}
			catch (std::invalid_argument& e) {
				delete (*obj);
				(*obj) = nullptr;
				throw e;
			}
			if (!(objectCast->add(newKey, newObject))) {
				if (newObject) {
					delete newObject;
				}
				delete (*obj);
				(*obj) = nullptr;
				startIndex -= newKey.length() + 5;
				error(startIndex);
				throw std::invalid_argument("\nError - duplicate key in object.");
			}
			ignoreWhiteSpaces(startIndex);
			if (startIndex < endIndex) {
				if (rawJSON[startIndex] != ',') {
					delete (*obj);
					(*obj) = nullptr;
					error(startIndex);
					throw std::invalid_argument("\nError - expected (,) to divide object's elements.");
				}
			}
			startIndex++;
		}
	}
	if (emptyObject) {
		startIndex++;
	}
}

void JSONParser::parseArray(int& startIndex, JSON** obj) {
	bool emptyArray = true;
	int endIndex = (*obj)->findEndIndex(this->rawJSON, startIndex);
	if (endIndex == -1) {
		delete (*obj);
		(*obj) = nullptr;
		error(startIndex);
		throw std::invalid_argument("\nError - expected ]. End of the array not found");
	}
	while (startIndex < endIndex) {
		emptyArray = false;
		JSON* newValue = nullptr;
		try {
			parseRawJSONString(startIndex, &newValue);
			JSONArray* arrayCast = dynamic_cast<JSONArray*>(*obj);
			arrayCast->add(newValue);
		}
		catch (std::invalid_argument& ex)
		{
			delete (*obj);
			(*obj) = nullptr;
			throw ex;
		}
		if (newValue)
			delete newValue;
		ignoreWhiteSpaces(startIndex);
		if (startIndex < endIndex) {
			if (rawJSON[startIndex] != ',') {
				error(startIndex);
				throw std::invalid_argument("\nError - expected (,) to divide array's elements.");
			}
		}
		startIndex++;
	}
	if (emptyArray) {
		startIndex++;
	}
}


void JSONParser::ignoreWhiteSpaces(int& startIndex) {
	while (rawJSON[startIndex] == '\t' || rawJSON[startIndex] == '\n' || rawJSON[startIndex] == ' ' || rawJSON[startIndex] == '\0') {
		if (startIndex < rawJSON.length()) {
			startIndex++;
		}
		if (startIndex == rawJSON.length())
			break;
	}
}


JSON* JSONParser::checkFirstCharacter(int& startIndex) {
	ignoreWhiteSpaces(startIndex);
	char read;
	read = rawJSON[startIndex];
	switch (read) {
	case 'n': return new JSONNull;
	case 't': return new JSONBool(true);
	case 'f': return new JSONBool(false);
	case '"': {
		startIndex++;
		return new JSONString;
	}
	case '{': {
		startIndex++;
		return new JSONObject;
	}
	case '[': {
		startIndex++;
		return new JSONArray;
	}
	}
	int intValue = read - '0';
	if (intValue >= 0 && intValue <= 9) {
		if (isDouble(startIndex))
			return new JSONDouble;
		else 
			return new JSONInteger;
		/*JSONInteger integerTest;
		JSONDouble doubleTest;
		if (integerTest.findEndIndex(rawJSON, startIndex) < doubleTest.findEndIndex(rawJSON, startIndex))
			return new JSONDouble;
		else
			return new JSONInteger;*/
	}
	return nullptr;
}

void JSONParser::parseRawJSONString(int& startIndex, JSON** obj) {
	*obj = checkFirstCharacter(startIndex);
	if (!*obj) {
		error(startIndex);
		throw std::invalid_argument("\nError - expected string, number, bool, null, object or array.");
	}
	switch ((*obj)->getType()) {
	case JSONType::JSON_INTEGER: {
		parseInteger(startIndex, obj);
		break;
	}
	case JSONType::JSON_DOUBLE: {
		parseDouble(startIndex, obj);
		break;
	}
	case JSONType::JSON_STRING: {
		parseString(startIndex, obj);
		break;
	}
	case JSONType::JSON_NULL: {
		parseNull(startIndex, obj);
		break;
	}
	case JSONType::JSON_BOOL: {
		JSONBool* boolCast = dynamic_cast<JSONBool*>(*obj);
		if (boolCast->getValue())
			parseTrueBool(startIndex, obj);
		else
			parseFalseBool(startIndex, obj);
		break;
	}
	case JSONType::JSON_OBJECT: {
		parseObject(startIndex, obj);
		break;
	}
	case JSONType::JSON_ARRAY: {
		parseArray(startIndex, obj);
		break;
	}
	}
	ignoreWhiteSpaces(startIndex);
}



void JSONParser::readRawJSON(std::ifstream& file) {
	if (!file.is_open())
		return;
	char read;
	file.get(read);
	while (!file.eof()) {
		rawJSON += read;
		file.get(read);
	}
}

