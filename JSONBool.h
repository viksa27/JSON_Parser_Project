#pragma once

#include "JSON.h"

class JSONBool : public JSON {
public:

	JSONBool(bool val);
	JSONBool();

	bool getValue() const;

	void print() const;

	JSON* clone() const;

	void save(std::ofstream&);

	const int findEndIndex(const std::string& rawJSON, int startIndex) const;

private:
	bool value;
};