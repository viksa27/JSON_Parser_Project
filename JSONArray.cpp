
#include "JSONArray.h"
#include "JSONParser.h"

JSONArray::JSONArray() : JSON(JSONType::JSON_ARRAY) { }

JSONArray::JSONArray(const JSONArray& copy) {
	for (size_t i = 0; i < copy.values.size(); i++) {
		this->values.push_back(copy.values[i]->clone());
	}
}

JSONArray::~JSONArray() {
	for (size_t i = 0; i < this->values.size(); i++) {
		delete values[i];
	}
}

size_t JSONArray::getElementsCount() const {
	return this->values.size();
}

void JSONArray::print() const {
	std::cout << "[";
	for (size_t i = 0; i < this->values.size(); i++) {
		values[i]->print();
		if (i + 1 != values.size())
			std::cout << ", ";
	}
	std::cout << "]";
}

void JSONArray::add(JSON* value) {
	this->values.push_back(value->clone());
}

bool JSONArray::edit(const std::string& index, JSON* value) {
	try {
		int indexNum = stoi(index);
		return edit(indexNum, value);
	}
	catch (std::invalid_argument& e) {
		return false;
	}
}

bool JSONArray::edit(int index, JSON* value) {
	if (index < 0 || index >= this->values.size()) {
		return false;
	}
	delete this->values[index];
	this->values[index] = value->clone();
	return true;
}

std::vector<JSON*> JSONArray::getValues() const {
	return this->values;
}


JSON* JSONArray::clone() const {
	JSONArray* clone = new JSONArray;
	for (size_t i = 0; i < this->values.size(); i++) {
		clone->add(values[i]);
	}
	return clone;
}


void JSONArray::manualAdd() {
	std::cout << "\nNOTE: Value should be entered according to the JSON syntax"
		<< "\nEnter value for the object you want to add:\n> ";
		char value[500];
		std::cin.getline(value, 500);
		JSONParser tempParser;
		int index = 0;
		if (tempParser.parseJSONString(value)) {
			this->add(tempParser.getObject());
			std::cout << "\nCreation succesfull.\n";
			return;
		}
		std::cout << "\nInvalid value input.\nCreation failed.\n";
}

bool JSONArray::remove(const std::string& key) {
	int index;
	try {
		index = stoi(key);
	}
	catch (std::invalid_argument& e) {
		std::cerr << e.what() << std::endl;
		return false;
	}
	if (index >= this->values.size() || index < 0) {
		std::cout << "\nInvalid index input.\n";
		return false;
	}
	this->values.erase(values.begin() + index);
	return true;
}

bool JSONArray::remove(int index) {
	if (index >= this->values.size() || index < 0) {
		std::cout << "\nInvalid index input.\n";
		return false;
	}
	this->values.erase(values.begin() + index);
	return true;
}

bool JSONArray::move(const std::string& index, JSON** destination) {
	int indexInt;
	try {
		indexInt = stoi(index);
	}
	catch (std::invalid_argument& e) {
		std::cout << "\nInvalid index\n";
		return false;
	}
	return move(indexInt, destination);
}

bool JSONArray::move(int index, JSON** destination) {
	if ((*destination)->getType() != this->type) {
		std::cout << "\nSource and destination must be the same JSON type.\n";
		return false;
	}
	JSONArray* arrayCast = dynamic_cast<JSONArray*>(*destination);
	if (index < 0 || index >= this->values.size()) {
		std::cout << "\nInvalid index\n";
		return false;
	}
	JSON* value = (*(*this)[index]);
	arrayCast->add(value);
	this->remove(index);
	return true;
}

void JSONArray::search(std::vector<JSON*>& searchValues, const std::string& key) {
	for (int i = 0; i < this->values.size(); i++) {
		values[i]->search(searchValues, key);
	}
}

void JSONArray::save(std::ofstream& openedFile) {
	openedFile << "[";
	for (int i = 0; i < this->values.size(); i++) {
		values[i]->save(openedFile);
		if (i + 1 != this->values.size()) {
			openedFile << ", ";
		}
	}
	openedFile << "]";
}


const int JSONArray::findEndIndex(const std::string& rawJSON, int startIndex) const {
	int arrayOpeningsFound = 1;
	int arrayClosingsFound = 0;
	int length = rawJSON.length();
	for (startIndex; startIndex < length; startIndex++) {
		if (rawJSON[startIndex] == '[') {
			arrayOpeningsFound++;
		}
		if (rawJSON[startIndex] == ']') {
			arrayClosingsFound++;
		}
		if (arrayOpeningsFound == arrayClosingsFound) {
			return startIndex;
		}
	}
	return -1;
}

JSON** JSONArray::operator[](const int index) {
	if (index >= this->values.size() || index < 0)
		throw std::invalid_argument("\nInvalid index, not in range.");

	return &this->values[index];
}

JSON** JSONArray::operator[](const std::string& index) {
	int indexNum;
	try {
		indexNum = stoi(index);
	}
	catch (std::invalid_argument& e) {
		throw e;
	}
	return (* this)[indexNum];
}

