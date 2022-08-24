#pragma once

#include "JSON.h"
#include <iostream>


class JSONNull : public JSON {
public:

	JSONNull();

	void print() const;

	void save(std::ofstream&);

	JSON* clone() const;

	const int findEndIndex(const std::string& rawJSON, int startIndex) const;

};