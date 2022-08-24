
#include "JSONObject.h"
#include "JSONParser.h"

void JSONObject::printTabulations(int repeatRate) const {
	for (int j = 0; j < repeatRate; j++) {
		std::cout << "    ";
	}
}

void JSONObject::writeTabulations(int repeatRate, std::ofstream& openedFile) {
	for (int j = 0; j < repeatRate; j++) {
		openedFile << "  ";
	}
}

void JSONObject::printObject(int numberOfTabulations) const {
	std::cout << "{\n";
	numberOfTabulations++;
	for (size_t i = 0; i < this->values.size(); i++) {
		printTabulations(numberOfTabulations);
		std::cout << '"' << keys[i] << "\": ";
		if (values[i]->getType() == JSONType::JSON_OBJECT) {
			JSONObject* objectCast = dynamic_cast<JSONObject*>(values[i]);
			objectCast->printObject(numberOfTabulations + 1);
		}
		else {
			values[i]->print();
		}
		if (i + 1 != values.size())
			std::cout << ",";
		std::cout << std::endl;
	}
	numberOfTabulations--;
	printTabulations(numberOfTabulations);
	std::cout << "}";
}

void JSONObject::saveObject(std::ofstream& openedFile, int numberOfTabulations) {
	openedFile << "{\n";
	numberOfTabulations++;
	for (int i = 0; i < this->keys.size(); i++) {
		writeTabulations(numberOfTabulations, openedFile);
		openedFile << "\"";
		openedFile << this->keys[i];
		openedFile << "\": ";
		if (this->values[i]->getType() == JSONType::JSON_OBJECT) {
			JSONObject* objectCast = dynamic_cast<JSONObject*>(values[i]);
			objectCast->saveObject(openedFile,numberOfTabulations + 1);
		}
		else {
			this->values[i]->save(openedFile);
		}
		if (i + 1 != this->keys.size()) {
			openedFile << ',';
		}
		openedFile << '\n';
	}
	numberOfTabulations--;
	writeTabulations(numberOfTabulations, openedFile);
	openedFile << "}";
}


bool JSONObject::isDuplicateKey(const std::string& key) {
	for (size_t i = 0; i < keys.size(); i++) {
		if (keys[i] == key)
			return true;
	}
	return false;
}

JSONObject::JSONObject() : JSON(JSONType::JSON_OBJECT) { }

JSONObject::JSONObject(const JSONObject& copy) {
	this->keys = copy.keys;
	for (size_t i = 0; i < copy.values.size(); i++) {
		this->values.push_back(copy.values[i]->clone());
	}
}

size_t JSONObject::getElementsCount() const {
	return this->values.size();
}

void JSONObject::print() const {
	printObject();
}

bool JSONObject::add(const std::string& key, JSON* value) {
	if (isDuplicateKey(key))
		return false;

	this->keys.push_back(key);
	this->values.push_back(value->clone());
	return true;
}

void JSONObject::manualAdd() {
	std::cout << "\n\nNOTE: Don't put the key in quotation marks\n"
			<< "Enter the new key for the object:\n> ";
	char key[500];
	std::cin.getline(key, 500);
	std::string keyStr = key;
	if (isDuplicateKey(keyStr)) {
		std::cout << "\nDuplicate key. Key already exists in the object.\nCreation failed.\n";
		return;
	}

	std::cout << "\nNOTE: Value should be entered according to the JSON syntax"
		<< "\nEnter value for the object you want to add:\n> ";
	char value[500];
	std::cin.getline(value, 500);
	JSONParser tempParser;
	int index = 0;
	if (tempParser.parseJSONString(value)) {
		this->add(keyStr,tempParser.getObject());
		std::cout << "\nCreation succesfull.\n";
		return;
	}
	std::cout << "\nInvalid value input.\nCreation failed.\n";
}

bool JSONObject::edit(const std::string& key, JSON* value) {
	for (size_t i = 0; i < this->keys.size(); i++) {
		if (this->keys[i] == key) {
			delete this->values[i];
			this->values[i] = value->clone();
			return true;
		}
	}
	std::cout << "\nKey not found.\n";
	return false;
}

bool JSONObject::remove(const std::string& key) {
	for (size_t i = 0; i < keys.size(); i++) {
		if (key == keys[i]) {
			keys.erase(keys.begin() + i);
			values.erase(values.begin() + i);
			return true;
		}
	}
	return false;
}

bool JSONObject::move(const std::string& key, JSON** destination) {
	if ((*destination)->getType() != this->type) {
		std::cout << "\nSource and destination must be the same JSON type.\n";
		return false;
	}
	JSONObject* objectCast = dynamic_cast<JSONObject*>(*destination);
	if (objectCast->isDuplicateKey(key)) {
		std::cout << "\nDuplicate key in destination. Move unsuccessfull.\n";
		return false;
	}
	if (!this->isDuplicateKey(key)) {
		std::cout << "\nKey doesn't exist. Move unsuccessfull.\n";
		return false;
	}
	JSON** value = (* this)[key];
	objectCast->add(key, *value);
	this->remove(key);
	return true;
}

void JSONObject::search(std::vector<JSON*>& searchValues, const std::string& key) {
	try {
		JSON** value = (* this)[key];
		searchValues.push_back((*value)->clone());
	}
	catch (std::invalid_argument& e) {

	}
	for (int i = 0; i < this->values.size(); i++) {
		values[i]->search(searchValues, key);
	}
}

void JSONObject::save(std::ofstream& openedFile) {
	saveObject(openedFile);
}



std::vector<JSON*> JSONObject::getValues() const {
	return this->values;
}


JSON* JSONObject::clone() const {
	JSONObject* clone = new JSONObject;
	for (size_t i = 0; i < this->values.size(); i++) {
		clone->add(keys[i], values[i]);
	}
	return clone;
}

const int JSONObject::findEndIndex(const std::string& rawJSON, int startIndex) const {
	int objectOpeningsFound = 1;
	int objectClosingsFound = 0;
	int length = rawJSON.length();
	for (startIndex; startIndex < length; startIndex++) {
		if (rawJSON[startIndex] == '{') {
			objectOpeningsFound++;
		}
		if (rawJSON[startIndex] == '}') {
			objectClosingsFound++;
		}
		if (objectOpeningsFound == objectClosingsFound) {
			return startIndex;
		}
	}
	return -1;
}

JSON** JSONObject::operator[](const std::string& key) {
	for (int i = 0; i < this->keys.size(); i++) {
		if (key == this->keys[i])
			return &this->values[i];
	}

	throw std::invalid_argument("\nKey doesn't exist.");
}

JSONObject::~JSONObject() {
	for (size_t i = 0; i < this->values.size(); i++) {
		delete values[i];
	}
}

