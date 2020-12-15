#include "gamedata.h"

#include "utils.h"

#include <iostream>
#include <fstream>
#include <string>


//
// LocationChoice
//

LocationChoice::LocationChoice(const std::string& choiceLocationId, const std::string& choiceText) :
	nextLocationId(choiceLocationId), text(choiceText) 
{ }



//
// Location
//

Location::Location(const std::string& locId, const std::string& locText) :
	id(locId), text(locText)
{ }



//
// GameData
//

GameData::GameData()
{
	initializeLocations();
	initializeItems();
}


std::shared_ptr<Location> GameData::getLocationWithId(const std::string& id)
{
	for (size_t i = 0; i < locations.size(); ++i) {
		if (locations[i]->id == id) {
			return locations[i];
		}
	}

	return nullptr;
}


std::shared_ptr<BaseItem> GameData::getItemWithId(const std::string& id)
{
	for (size_t i = 0; i < items.size(); ++i) {
		if (items[i]->id == id) {
			return items[i];
		}
	}

	return nullptr;
}


const std::vector<std::shared_ptr<Location>>& GameData::getLocations() const
{
	return locations;
}


const std::vector<std::shared_ptr<BaseItem>>& GameData::getItems() const
{
	return items;
}


void GameData::initializeLocations()
{
	int count = 0;
	count += loadLocations("content/locations/area01.txt");
	
	if (count == 0) {
		std::cout << "Failed to load locations. If you are on macOS, did you set the working directory? \n";
	}
}


void GameData::initializeItems()
{
	int count = 0;
	count += loadItems("content/items.txt");
	
	if (count == 0) {
		std::cout << "Failed to load items. If you are on macOS, did you set the working directory? \n";
	}
}


int GameData::loadLocations(const std::string& path)
{
	std::ifstream file(path);

	if (!file.is_open()) {
		std::cout << "Failed to load locations from file: " << path;
		return 0;
	}

	int locationsLoaded = 0;
	std::string line;
	std::shared_ptr<Location> tempLocation = std::make_shared<Location>("", "");

	while (std::getline(file, line)) {
		if (line.length() > 0)
		{
			if (line[0] == '#') {
				// #id
				tempLocation->id = line.substr(1, std::string::npos);
			}
			else if (line[0] == '&') {
				// &nextRoomId:choiceText
				line = line.substr(1, std::string::npos);
				size_t colon = line.find(':');

				if (colon != std::string::npos) {
					LocationChoice choice("", "");
					choice.nextLocationId = line.substr(0, colon);
					choice.text = line.substr(colon + 1, std::string::npos);
					tempLocation->choices.push_back(choice);
				}
			}
			else if (line[0] == '^') {
				// ^itemId
				line = line.substr(1, std::string::npos);
				tempLocation->items.push_back(line);
			}
			else if (line[0] == '#') {
				// Comment line - simply ignore!
			}
			else if (line[0] == '=') {
				// "Write/save the temp location to the database" 
				locations.push_back(tempLocation);
				
				// Reset the 'tempLocation' to a completely new location
				tempLocation = std::make_shared<Location>("", "");
				locationsLoaded++;
			}
			else {
				// With no markers, just add this text to the body of the location.
				tempLocation->text += line + "\n";
			}
		}
	}

	std::cout << "Loaded " << locationsLoaded << " from " << path << "\n";
	return locationsLoaded;
}


int GameData::loadItems(const std::string& path)
{
	std::ifstream file(path);
	
	if (file.is_open() == false) {
		std::cout << "Failed to open items file at path: " << path << "\n";
		return 0;
	}
	
	int count = 0;
	std::string line;
	
	while(std::getline(file, line)) {
		// Split each line on 'tab'
		std::vector<std::string> tokens = split(line, '\t');
		
		// Should always have at least 3 tokens on a line - the type, ID and text. Extra options might exist for each type
		if (tokens.size() < 3) {
			continue;
		}
		
		if (tokens[0] == "note" || tokens[0] == "Note")
		{
			// Note requires 4 tokens (tab separated) "note id text useText"
			if (tokens.size() < 4) {
				continue;
			}
			
			std::shared_ptr<NoteItem> item = std::make_shared<NoteItem>(tokens[1], tokens[2], tokens[3]);
			items.push_back(item);
			count++;
		}
		else if (tokens[0] == "teleport" || tokens[0] == "Teleport")
		{
			// Teleport requires 4 tokens (tab separated) "teleport id text locationId
			if (tokens.size() < 4) {
				continue;
			}
			
			std::shared_ptr<TeleportItem> item = std::make_shared<TeleportItem>(tokens[1], tokens[2], tokens[3]);
			items.push_back(item);
			count++;
		}
	}
	
	return count;
}
