#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

enum class JSONType {
	JSON_NULL,
	JSON_INTEGER,
	JSON_DOUBLE,
	JSON_BOOL,
	JSON_STRING,
	JSON_OBJECT,
	JSON_ARRAY
};

class JSON {
public:
	virtual JSON* clone() const = 0;
	virtual void print() const = 0;
	virtual const int findEndIndex(const std::string&, int) const = 0;

	virtual JSONType getType() const;
	


	virtual bool edit(int, JSON*);
	virtual bool edit(const std::string&, JSON*);

	virtual void manualAdd();

	virtual bool remove(const std::string&);

	virtual bool move(const std::string&, JSON**);
	virtual void search(std::vector<JSON*>&, const std::string&);

	virtual void save(std::ofstream&) = 0;

	virtual JSON** operator[](const int);
	virtual JSON** operator[](const std::string&);

	virtual ~JSON();

protected:
	JSONType type;
	JSON(JSONType tp = JSONType::JSON_NULL);

};