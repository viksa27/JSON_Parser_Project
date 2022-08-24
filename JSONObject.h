#pragma once

#include "JSON.h"
#include <vector>

class JSONObject : public JSON {
private:
	std::vector<std::string> keys;
	std::vector<JSON*> values;

	void printTabulations(int repeatRate) const;
	void writeTabulations(int, std::ofstream&);


	void printObject(int numberOfTabulations = 0) const;
	void saveObject(std::ofstream&,int numberOfTabulations = 0);
public:
	bool isDuplicateKey(const std::string& key);

	JSONObject();

	JSONObject(const JSONObject& copy);

	size_t getElementsCount() const;

	void print() const;

	bool add(const std::string&, JSON*);
	bool edit(const std::string&, JSON*);

	bool remove(const std::string&);

	bool move(const std::string&, JSON**);

	void manualAdd();

	void search(std::vector<JSON*>&, const std::string&);

	std::vector<JSON*> getValues() const;

	JSON* clone() const;

	void save(std::ofstream&);

	const int findEndIndex(const std::string& rawJSON, int startIndex) const;

	JSON** operator[](const std::string& key);

	virtual ~JSONObject();
};