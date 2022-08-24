#include "JSON.h"
#include <iostream>

JSON::JSON(JSONType tp) {
	this->type = tp;
}

JSON** JSON::operator[](const int) {
	return nullptr;
}

JSON** JSON::operator[](const std::string& key) {
	return nullptr;
}

bool JSON::edit(int, JSON*) {
	return false;
}

bool JSON::edit(const std::string&, JSON*) {
	return false;
}

void JSON::manualAdd() {
	std::cout << "\nAddition of elements not available for this type of JSON.\n";
}

bool JSON::remove(const std::string& key) {
	std::cout << "\nRemoving elements not available for this type of JSON.\n";
	return false;
}

bool JSON::move(const std::string& key, JSON** object) {
	std::cout << "\nMoving elements not available for this type of JSON.";
	return false;
}

void JSON::search(std::vector<JSON*>& searchValues ,const std::string& key) { }

JSONType JSON::getType() const {
	return this->type;
}

JSON::~JSON() { }