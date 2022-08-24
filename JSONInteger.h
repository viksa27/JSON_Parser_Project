#pragma once

#include "JSON.h"

class JSONInteger : public JSON {
public:

	JSONInteger(int val);
	JSONInteger();

	void save(std::ofstream&);

	void print() const;
	JSON* clone() const;
	const int findEndIndex(const std::string& rawJSON, int startIndex) const;

private:
	int value;
};


