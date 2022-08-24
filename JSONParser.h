#pragma once

#include "JSONNull.h"
#include "JSONInteger.h"
#include "JSONDouble.h"
#include "JSONString.h"
#include "JSONBool.h"
#include "JSONObject.h"
#include "JSONArray.h"
#include <fstream>


class JSONParser {
public:
	JSONParser();

	JSON* getObject() const;

	bool parseJSONFile(const char*);
	bool parseJSONString(const std::string&);

	void clear();

	~JSONParser();

private:
	JSON* parsedJSON;
	std::string rawJSON;

	void readRawJSON(std::ifstream&);

	void parseRawJSONString(int&, JSON**);

	JSON* checkFirstCharacter(int&);

	void ignoreWhiteSpaces(int&);


	void parseInteger(int&, JSON**);
	void parseDouble(int&, JSON**);
	void parseString(int&, JSON**);
	void parseNull(int&, JSON**);
	void parseObject(int&, JSON**);
	void parseTrueBool(int&, JSON**);
	void parseFalseBool(int&, JSON**);
	void parseArray(int&, JSON**);


	int readJSONInteger(int&, int&) const;
	double readJSONDouble(int&, int&) const;
	std::string readJSONString(int&, int&) const;
	bool verifyNull(int&) const;
	bool verifyTrue(int&) const;
	bool verifyFalse(int&) const;

	bool isDouble(int) const;

	void error(int);
	int findLineError(int);
	void calculateErrorPrintIndexes(int, int&, int&);
	void printError(int&, int&);
};
