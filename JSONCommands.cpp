#define _CRT_SECURE_NO_WARNINGS
#include "JSONCommands.h"
#include <vector>

JSONCommand::JSONCommand(const std::string& newCaller, const std::string& newDescription) {
	this->caller = newCaller;
	this->description = newDescription;
}

void JSONCommand::printDescription() const {
	std::cout << this->description;
}
std::vector<std::string> JSONCommand::parsePath(const char* constPath) {
	const char separator[2] = ".";
	char* token;
	char* path = new char[strlen(constPath) + 1];
	strcpy(path, constPath);
	token = strtok(path, separator);
	std::vector<std::string> pathElements;
	while (token != NULL) {
		pathElements.push_back(token);

		token = strtok(NULL, separator);
	}
	delete[] path;
	return pathElements;
}

JSON** JSONCommand::accessObject(JSON** object, std::vector<std::string>& path, int i) {
	if (object == nullptr) {
		return nullptr;
	}
	if (path.empty())
		return object;
	try {
		if (i + 1 == path.size()) {
			try {
				return (*(*object))[path[i]];
			}
			catch (std::invalid_argument& e) {
				return nullptr;
			}
		}
		else {
			i++;
			try {
				return accessObject((*(*object))[path[i - 1]], path, i);
			}
			catch (std::invalid_argument& e) {
				return nullptr;
			}
		}
	}
	catch(std::invalid_argument& e) {
		return nullptr;
		//throw std::invalid_argument("\nInvalid path input.");
	}
}

void JSONCommand::createObjectPath(JSON** object, std::vector<std::string>& path, int i) {
	if (object == nullptr) {
		return;
	}
	if (path.empty())
		return;
	if (i >= path.size())
		return;
	try {
		createObjectPath((*(*object))[path[i]], path, i + 1);
	}
	catch (std::invalid_argument& e) {
		if ((*object)->getType() != JSONType::JSON_OBJECT)
			return;
		JSONObject value;
		JSONObject* objectCast = dynamic_cast<JSONObject*>(*object);
		objectCast->add(path[i], &value);
		createObjectPath((*objectCast)[path[i]], path, i + 1);
	}
}


std::string JSONCommand::getCaller() const {
	return this->caller;
}

JSONCommand::~JSONCommand() { }

Print::Print() : JSONCommand("print","print\t\tPrint object") { }

void Print::execute(JSON** object) {
	std::cout << std::endl;
	(*object)->print();
	std::cout << std::endl;
}

void Print::printMenu() const { }
void Print::printHelp() const { }


Edit::Edit() : JSONCommand("edit","edit\t\tEdit object") { }


void Edit::proceedToEdit(JSON** object) {
	std::cout << "\nPlease enter the path of the object you want to edit.\n> ";
	char path[500];
	std::cin.ignore();
	std::cin.getline(path, 500);
	std::vector<std::string> pathVector = parsePath(path);
	if (pathVector.size() == 0) {
		std::cout << "\nCan't edit root/parent object!\n";
		return;
	}

	JSON** selectedObject = accessObject(object, pathVector);
	std::string key = pathVector[pathVector.size() - 1];
	pathVector.pop_back();
	JSON** selectedParentObject = accessObject(object, pathVector);

	if (selectedObject == nullptr || selectedParentObject == nullptr) {
		std::cout << "\nInvalid object path.\n";
		return;
	}

	std::cout << "\nCurrent value of the object:\n";
	(*selectedObject)->print();
	std::cout << "\nNOTE: Value should be entered according to the JSON syntax"
		<< "\nEnter the new value for the object:\n> ";
	char value[500];
	std::cin.getline(value, 500);
	if (editObject(selectedParentObject, key, value))
		std::cout << "\nEdit successfull.\n";
	else
		std::cout << "\nInvalid key or value input.\n";
}

bool Edit::editObject(JSON** object, const std::string& key, const std::string& rawJSONString) const {
	JSONParser tempParser;
	int index = 0;
	
	if (tempParser.parseJSONString(rawJSONString)) {
		return (*object)->edit(key, tempParser.getObject());
	}

	return false;
}

void Edit::printMenu() const {
	std::cout << "\n print\t\tPrint object\n"
		<< " help\t\tGuide on how to edit an object\n"
		<< " select\t\tSelect an object to edit\n"
		<< " back\t\tReturn to main menu\n";
}

void Edit::printHelp() const {
	std::cout << " To access an element in an object enter the key\n"
		<< " To access an element in an array enter the index\n"
		<< " Root/Parent object can't be edited\n"
		//<< " To access the parent object/parent array please don't type anything as an object path.\n"
		<< " Make sure that the new value is written according to the JSON Syntax\n"
		<< " DIVIDE INDEXES AND KEYS BY A DOT SEPARATOR (.)\n"
		<< " Entry example: key1.2.key2.key3.0\n\n";
}

void Edit::execute(JSON** object) {
	printMenu();
	std::string input = "";
	while (input != "main") {
		std::cout << "\n[EDIT]> ";
		std::cin >> input;
		if (input == "print") {
			std::cout << "\n";
			(*object)->print();
			std::cout << "\n";
		}
		else if (input == "help") {
			printHelp();
		}
		else if (input == "select") {
			proceedToEdit(object);
		}
		else if (input == "back") {
			break;
		}
		else if (input != "main") {
			printMenu();
		}
	}
	std::cout << "\nReturning to main menu...\n";
	std::cin.ignore();
}



Create::Create() : JSONCommand("create","create\t\tCreate object") { }


void Create::proceedToCreation(JSON** object) {
	std::cout << "\nPlease enter the path where you want to save the new object.\n> ";
	char path[500];
	std::cin.ignore();
	std::cin.getline(path, 500);
	std::vector<std::string> pathVector = parsePath(path);
	createObjectPath(object, pathVector);
	JSON** selectedObject = accessObject(object, pathVector);
	if (selectedObject == nullptr) {
		std::cout << "\nInvalid object path.\n";
		return;
	}
	(*selectedObject)->manualAdd();
}

void Create::printMenu() const {
	std::cout << "\n print\t\tPrint object\n"
		<< " help\t\tGuide on how to create an object\n"
		<< " continue\tProceed to creation\n"
		<< " back\t\tReturn to main menu\n";
}

void Create::printHelp() const {
	std::cout << " If you want to access an element in an object enter the key.\n"
		<< " If you want to access an element in an array enter the index.\n"
		<< " If you want to access the parent object/parent array please don't type anything as a path.\n"
		<< " Make sure that the value is written according to the JSON Syntax\n"
		<< " NOTE: You can only create (add) new values to OBJECTS and ARRAYS\n"
		<< " DIVIDE INDEXES AND KEYS BY A DOT SEPARATOR (.)\n"
		<< " Entry example: key1.2.key2.key3.0\n\n";
}

void Create::execute(JSON** object) {
	printMenu();
	std::string input = "print";
	while (input != "main") {
		std::cout << "\n[CREATE]> ";
		std::cin >> input;
		if (input == "print") {
			std::cout << "\n";
			(*object)->print();
			std::cout << "\n";
		}
		else if (input == "help") {
			printHelp();
		}
		else if (input == "continue") {
			proceedToCreation(object);
		}
		else if (input == "back") {
			break;
		}
		else if (input != "main") {
			printMenu();
		}
	}
	std::cout << "\nReturning to main menu...\n";
	std::cin.ignore();
}



Remove::Remove() : JSONCommand("remove","remove\t\tRemove object") { }

void Remove::proceedToRemoval(JSON** object) {
	std::cout << "\nPlease enter the path of the object you want to remove.\n> ";
	char path[500];
	std::cin.ignore();
	std::cin.getline(path, 500);
	std::vector<std::string> pathVector = parsePath(path);
	if (pathVector.empty()) {
		std::cout << "\n\nCan't remove root/parent object.\n";
		return;
	}
	std::string input;
	JSON** selectedObject = accessObject(object, pathVector);
	std::string deleteKey = pathVector[pathVector.size() - 1];
	pathVector.pop_back();
	JSON** selectedParentObject = accessObject(object, pathVector);

	if (selectedObject == nullptr || selectedParentObject == nullptr) {
		std::cout << "\nInvalid object path.\n";
		return;
	}

	std::cout << "\nYou chose:\n";
	(*selectedObject)->print();
	std::cout << "\nAre you sure you want to delete this object?\ny/n - ";
	std::cin >> input;
	if (input == "y") {
		if ((*selectedParentObject)->remove(deleteKey)) {
			std::cout << "\nDeletion successfull.\n";
		}
	}
	else {
		std::cout << "\nDeletion canceled.\n";
	}
}

void Remove::printHelp() const {
	std::cout << " If you want to access an element in an object enter the key.\n"
		<< " If you want to access an element in an array enter the index.\n"
		<< " The object you choose will be deleted along with all the elements inside it\n"
		<< " DIVIDE INDEXES AND KEYS BY A DOT SEPARATOR (.)\n"
		<< " Entry example: key1.2.key2.key3.0\n\n";
}

void Remove::printMenu() const {
	std::cout << "\n print\t\tPrint object\n"
		<< " help\t\tGuide on how to remove an object\n"
		<< " select\t\tSelect an object to remove\n"
		<< " back\t\tReturn to main menu\n";
}

void Remove::execute(JSON** object) {
	printMenu();
	std::string input = "print";
	while (input != "main") {
		std::cout << "\n[REMOVE]> ";
		std::cin >> input;
		if (input == "print") {
			std::cout << "\n";
			(*object)->print();
			std::cout << "\n";
		}
		else if (input == "help") {
			printHelp();
		}
		else if (input == "select") {
			proceedToRemoval(object);
		}
		else if (input == "back") {
			break;
		}
		else if (input != "main") {
			printMenu();
		}
	}
	std::cout << "\nReturning to main menu...\n";
	std::cin.ignore();
}



Move::Move() : JSONCommand("move","move\t\tMove object") { }

void Move::proceedToMoving(JSON** object) {
	std::cout << "\nPlease enter the path of the object you want to move.\n> ";
	char objectPath[500];
	std::cin.ignore();
	std::cin.getline(objectPath, 500);
	std::vector<std::string> objectPathVector = parsePath(objectPath);
	if (objectPathVector.empty()) {
		std::cout << "\nCan't move root/parent object.\n";
		return;
	}
	JSON** selectedObject = accessObject(object, objectPathVector);
	std::string key = objectPathVector[objectPathVector.size() - 1];
	objectPathVector.pop_back();
	JSON** source = accessObject(object, objectPathVector);
	if (selectedObject == nullptr || source == nullptr) {
		std::cout << "\nInvalid object path.\n";
		return;
	}

	std::cout << "\nYou chose:\n";
	(*selectedObject)->print();

	std::cout << "\nPlease enter the path where you want to save the chosen object.\n> ";
	char destinationPath[500];
	std::cin.getline(destinationPath, 500);
	std::vector<std::string> destinationPathVector = parsePath(destinationPath);
	JSON** destination = accessObject(object, destinationPathVector);

	if (destination == nullptr) {
		std::cout << "\nInvalid object path.\n";
		return;
	}

	std::cout << "\nYour object will be moved to:\n";
	(*destination)->print();
	std::cout << "\nAre you sure you want to proceed?\ny/n - ";
	std::string input;
	std::cin >> input;
	if (input == "y") {
		if ((*source)->move(key, destination)) {
			std::cout << "\nMove successfull.\n";
		}
	}
	else {
		std::cout << "Cancelling...\n\n";
	}
}

void Move::printMenu() const {
	std::cout << "\n print\t\tPrint object\n"
		<< " help\t\tGuide on how to move an object\n"
		<< " select\t\tSelect an object to move\n"
		<< " back\t\tReturn to main menu\n";
}

void Move::printHelp() const {
	std::cout << " If you want to access an element in an object enter the key.\n"
		<< " If you want to access an element in an array enter the index.\n"
		<< " Make sure the object's current parent and the new parent are of the same type.\n"
		<< " Array elements can be moved to a new array, object's elements can be moved to a new object.\n"
		<< " DIVIDE INDEXES AND KEYS BY A DOT SEPARATOR (.)\n"
		<< " Entry example: key1.2.key2.key3.0\n";
}

void Move::execute(JSON** object) {
	printMenu();
	std::string input = "";
	while (input != "main") {
		std::cout << "\n[MOVE]> ";
		std::cin >> input;

		if (input == "print") {
			std::cout << "\n";
			(*object)->print();
			std::cout << "\n";
		}
		else if (input == "select") {
			proceedToMoving(object);
		}
		else if (input == "help") {
			printHelp();
		}
		else if (input == "back") {
			break;
		}
		else if (input != "main") {
			printMenu();
		}
	}
	std::cout << "\nReturning to main menu...\n";
	std::cin.ignore();
}


Search::Search() : JSONCommand("search","search\t\tSearch through key") { }

void Search::printSearchValues(std::vector<JSON*>& values, std::string& key) const {
	std::cout << "\nValues found with key \"" << key << "\":\n";
	for (size_t i = 0; i < values.size(); i++) {
		std::cout << i + 1 << ". ";
		values[i]->print();
		std::cout << "\n";
	}
}

void Search::execute(JSON** object) {
	std::vector<JSON*> validValues;
	char keyInput[500];
	std::cout << "\nEnter the search key:\n> ";
	std::cin.getline(keyInput, 500);
	std::string key = keyInput;
	(*object)->search(validValues, key);
	if (validValues.size() == 0) {
		std::cout << "\nNo values found with key \"" << key << "\"\n";
		return;
	}
	else {
		std::cout << "\n" << validValues.size() << " values found with key \"" << key << "\"\n";
	}
	printMenu();

	std::string input = "print";
	while (input != "main") {
		std::cout << "\n[SEARCH]> ";
		std::cin >> input;
		if (input == "print") {
			printSearchValues(validValues, key);
		}
		else if (input == "help") {
			printHelp();
		}
		else if (input == "save") {
			save(validValues);
		}
		else if (input == "back") {
			break;
		}
		else if (input != "main") {
			printMenu();
		}
	}

	for (size_t i = 0; i < validValues.size(); i++) {
		if (validValues[i]) {
			delete validValues[i];
		}
	}
	std::cout << "\nReturning to main menu...\n";
	std::cin.ignore();
}

void Search::printMenu() const {
	std::cout << "\n print\t\tPrint found objects\n"
		<< " help\t\tGuide on how to save objects to file\n"
		<< " save\t\tSave objects to file\n"
		<< " back\t\tReturn to main menu\n";
}

void Search::printHelp() const {
	std::cout << " If you want to save all the elements, dont type anything as an input.\n"
		<< " If you want to save only some of the elements, type the indexes of the elements you want to save.\n"
		<< " DIVIDE INDEXES BY A DOT SEPARATOR (.)\n"
		<< " Entry example: 1.2.6.5.3.4\n"
		<< " The elements will be saved in the order you write their indexes.\n";
}

void Search::save(std::vector<JSON*>& validValues) {
	std::cout << "Please write the indexes of the elements you want to save\n"
		<< "NOTE: If you want to save everything, leave input blank.\n> ";
	char path[500];
	std::cin.ignore();
	std::cin.getline(path, 500);
	std::vector<std::string> pathVector = parsePath(path);
	if (pathVector.empty()) {
		JSONArray arraySave;
		for (size_t i = 0; i < validValues.size(); i++) {
			arraySave.add(validValues[i]);
		}
		saveToFile(arraySave);
	}
	else {
		JSONArray arraySave;
		for (size_t i = 0; i < pathVector.size(); i++) {
			int arrayIndex;
			try {
				arrayIndex = stoi(pathVector[i]);
			}
			catch (std::invalid_argument& e) {
				std::cout << "\nInvalid index pattern.\n";
				return;
			}
			arrayIndex--;
			if (arrayIndex < 0 || arrayIndex >= validValues.size()) {
				std::cout << "\nInvalid index pattern.\n";
				return;
			}
			else {
				arraySave.add(validValues[arrayIndex]);
			}
		}
		saveToFile(arraySave);
	}
}

void Search::saveToFile(JSONArray& validValues) {
	
	std::cout << "\nWARNING: If the file already exists, its data will be erased and replaced with the new data."
		<< "\nPlease enter the directory of the save file :\n> ";
	char source[500];
	std::cin.getline(source, 500);
	std::cout << "\nAre you sure you want to save\n";
	validValues.print();
	std::cout << "\nto a new file - \"" << source << "\"?\ny/n - ";
	char input[500];
	std::cin.getline(input,500);
	std::cout << std::endl;
	if (input[0] == 'y') {
		std::ofstream file;
		file.open(source, std::ios::trunc | std::ios::out);
		file.close();
		file.open(source, std::ios::app);
		if(!file.is_open()) {
			std::cout << "\nError. File didn't open.\nReturning...\n";
			return;
		}
		validValues.save(file);
		std::cout << "\nSave successfull\n";
 	}
	else {
		std::cout << "Cancelling...\n";
		return;
	}
}


Save::Save() : JSONCommand("save", "save\t\tSave JSON") { }

void Save::printMenu() const { }

void Save::printHelp() const { }

void Save::execute(JSON** object) {
	std::cout << "\nPlease enter the path of the object you want to save.\n> ";
	char path[500];
	std::cin.getline(path, 500);
	std::vector<std::string> pathVector = parsePath(path);
	JSON** objectForSaving = accessObject(object, pathVector);
	if (objectForSaving == nullptr) {
		std::cout << "\nInvalid object path.\n";
		return;
	}
	std::cout << "\nYou are about to save:\n";
	(*objectForSaving)->print();
	std::cout << "\n\nAre you sure you want to save the object printed above? y/n - ";
	std::string input;
	std::cin >> input;
	if (input != "y") {
		std::cout << "\nCancelling...\n";
		return;
	}
	std::cout << "\nEnter the name of the new JSON file:\n> ";
	char fileName[500];
	std::cin.ignore();
	std::cin.getline(fileName, 500);
	std::ifstream testFile;
	testFile.open(fileName);
	if (testFile.is_open()) {
		std::cout << "\nFile already exists. Do you want to overwrite?\ny/n - ";
		std::cin >> input;
		std::cin.ignore();
		if (input != "y") {
			std::cout << "\nCancelling...\n";
			return;
		}
	}
	std::ofstream file;
	file.open(fileName);
	if (!file.is_open()) {
		std::cout << "Can't create/open file.\n";
		return;
	}
	(*objectForSaving)->save(file);
	std::cout << "\nFile saved.";
	std::cout << std::endl;
}