#pragma once

#include "gamedata.h"
#include "items.h"

#include <memory>
#include <string>


/**
 * Represents a single "slot" in the user's inventory which could contain 1 or more copies of a single item
 */
struct InventoryItem {
	std::shared_ptr<BaseItem> item;
	int amount = 0;
};


/**
 * Stores all state for the current player (serialize this to save the game)
 */
class Player {
public:

	std::string name;
	std::shared_ptr<Location> currentLocation;

	void addItem(const std::string& itemId, int amount);
	void removeItem(const std::string& itemId, int amount);
	bool hasItem(const std::string& itemId);

	void addVisitedLocation(const std::string& id);
	bool hasVisitedLocation(const std::string& id);

	std::vector<InventoryItem> inventory;
	std::vector<std::string> visitedLocations;
};
