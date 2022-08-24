#pragma once

#include "JSONParser.h"

class JSONCommand {
protected:

	std::string caller;
	std::string description;

	std::vector<std::string> parsePath(const char*);
	JSON** accessObject(JSON**, std::vector<std::string>&, int i = 0);
	void createObjectPath(JSON**, std::vector<std::string>&, int i = 0);
	
public:
	
	virtual void execute(JSON**) = 0;

	JSONCommand(const std::string&, const std::string&);
	
	virtual void printMenu() const = 0;
	virtual void printHelp() const = 0;

	virtual void printDescription() const;

	std::string getCaller() const;

	virtual ~JSONCommand();
};

class Print : public JSONCommand {
private:

public:
	Print();

	void execute(JSON**);

	void printMenu() const;
	void printHelp() const;
};

class Edit : public JSONCommand {
private:

	bool editObject(JSON**, const std::string&, const std::string&) const;
	void proceedToEdit(JSON**);

public:
	Edit();

	void execute(JSON**);

	void printMenu() const;
	void printHelp() const;

};


class Create : public JSONCommand {
private:

	void proceedToCreation(JSON**);

public:
	Create();

	void execute(JSON**);

	void printMenu() const;
	void printHelp() const;

};

class Remove : public JSONCommand {
private:

	void proceedToRemoval(JSON**);

public:

	Remove();

	void execute(JSON**);

	void printMenu() const;
	void printHelp() const;
};

class Move : public JSONCommand {
private:

	void proceedToMoving(JSON**);

public:
	Move();

	void execute(JSON**);

	void printMenu() const;
	void printHelp() const;
};

class Search : public JSONCommand {
private:

	void printSearchValues(std::vector<JSON*>&, std::string&) const;
	void save(std::vector<JSON*>&);
	void saveToFile(JSONArray&);

public:
	Search();

	void execute(JSON**);

	void printMenu() const;
	void printHelp() const;

};

class Save : public JSONCommand {

public:
	Save();

	void execute(JSON**);

	void printMenu() const;
	void printHelp() const;
};