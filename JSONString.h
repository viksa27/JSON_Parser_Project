#pragma once

#include "JSON.h"

class JSONString : public JSON {
private:
	std::string value;
public:
	JSONString(std::string val);
	JSONString();

	void print() const;

	void save(std::ofstream&);

	JSON* clone() const;

	const int findEndIndex(const std::string& rawJSON, int startIndex) const;
};