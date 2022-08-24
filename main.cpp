#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "JSONCommands.h"


struct Commands {
	std::vector<JSONCommand*> allCommands;
	
	void printMenu() {
		std::cout << std::endl;
		for (size_t i = 0; i < allCommands.size(); i++) {
			allCommands[i]->printDescription();
			std::cout << std::endl;
		}
		std::cout << "exit\t\tExit\n";
	}

	Commands() {
		allCommands = { new Print, new Search, new Edit, new Create, new Remove, new Move, new Save };
	}

	~Commands() {
		for (size_t i = 0; i < allCommands.size(); i++) {
			delete allCommands[i];
		}
	}
	void runCommands(JSON** ptr) {
		char input[500] = "default";
		bool commandFound = false;
		while (strcmp(input,"exit") != 0) {
			if (!commandFound) {
				printMenu();
			}
			commandFound = false;
			std::cout << "\n[MAIN]> ";
			std::cin.getline(input, 500);
			
			for (size_t i = 0; i < allCommands.size(); i++) {
				if (allCommands[i]->getCaller() == input) {
					allCommands[i]->execute(ptr);
					commandFound = true;
				}
			}
			
		}
	}
};


int main()
{

	JSONParser parser;
	std::cout << "Enter the JSON file path:\n> ";
	char source[500];
	std::cin >> source;
	std::cin.ignore();
	parser.parseJSONFile(source);
	JSON* parsedObject = parser.getObject();
	if (parsedObject == nullptr) {
		return 1;
	}
	Commands c;
	c.runCommands(&parsedObject);

	if (parsedObject) {
		delete parsedObject;
	}

	return 0;

}
