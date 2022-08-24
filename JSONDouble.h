#pragma once

#include "JSON.h"

class JSONDouble : public JSON {
public:

	JSONDouble(double val);
	JSONDouble();

	void print() const;

	void save(std::ofstream&);

	JSON* clone() const;

	const int findEndIndex(const std::string& rawJSON, int startIndex) const;

private:
	double value;
};