#pragma once

#include "JSON.h"
#include <vector>

class JSONArray : public JSON {
public:

	JSONArray();
	JSONArray(const JSONArray& copy);
	virtual ~JSONArray();

	size_t getElementsCount() const;

	void print() const;

	void add(JSON*);

	bool edit(int, JSON*);
	bool edit(const std::string&, JSON*);

	bool remove(int);
	bool remove(const std::string&);

	bool move(const std::string&, JSON**);
	bool move(int, JSON**);

	void save(std::ofstream&);

	void manualAdd();

	void search(std::vector<JSON*>& searchValues, const std::string& key);

	std::vector<JSON*> getValues() const;

	JSON* clone() const;

	const int findEndIndex(const std::string& rawJSON, int startIndex) const; 

	JSON** operator[](const int);
	JSON** operator[](const std::string&);

	

private:
	std::vector<JSON*> values;

};