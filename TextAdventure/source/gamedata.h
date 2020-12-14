#pragma once

#include "items.h"

#include <memory>
#include <string>
#include <vector>


/**
 * Represents a choice that the user can make when they are at a given location
 */
struct LocationChoice {
	LocationChoice(const std::string& choiceLocationId, const std::string& choiceText);

	std::string nextLocationId;
	std::string text;
};


/**
 * Represents a single location in the game world (room or conversation choice).
 */
struct Location {
	Location(const std::string& locId, const std::string& locText);

	std::string id;
	std::string text;

	std::vector<LocationChoice> choices;
	std::vector<std::string> items;
};


/**
 * Essentially an "in-memory database" of all of the game data in the game (locations and items).
 */
class GameData {
public:
	GameData();

	std::shared_ptr<Location> getLocationWithId(const std::string& id);
	std::shared_ptr<BaseItem> getItemWithId(const std::string& id);

	std::vector<std::shared_ptr<Location>> locations;
	std::vector<std::shared_ptr<BaseItem>> items;
	
private:
	void initializeLocations();
	void initializeItems();

	int loadLocations(const std::string& path);
	int loadItems(const std::string& path);
};
