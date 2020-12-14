#include "player.h"

#include "game.h"


void Player::addItem(const std::string& itemId, int amount)
{
	for (int i = 0; i < inventory.size(); ++i) {
		if (inventory[i].item->id == itemId) {
			inventory[i].amount += amount;
			return;
		}
	}

	// If we get here, the user doesn't have this item already, so add it
	InventoryItem newItem;
	newItem.item = Game::Instance().gameData.getItemWithId(itemId);
	newItem.amount = amount;

	if (newItem.item != nullptr) {
		inventory.push_back(newItem);
	}
}


void Player::removeItem(const std::string& itemId, int amount)
{
	for (int i = 0; i < inventory.size(); ++i) {
		if (inventory[i].item->id == itemId) {
			inventory[i].amount -= amount;

			if (inventory[i].amount <= 0) {
				inventory.erase(inventory.begin() + i);
			}

			return;
		}
	}
}


bool Player::hasItem(const std::string& itemId)
{
	for (int i = 0; i < inventory.size(); ++i) {
		if (inventory[i].item->id == itemId && inventory[i].amount > 0) {
			return true;
		}
	}

	return false;
}


void Player::addVisitedLocation(const std::string& id)
{
	if (hasVisitedLocation(id) == false)
	{
		visitedLocations.push_back(id);
	}
}


bool Player::hasVisitedLocation(const std::string& id)
{
	for (int i = 0; i < visitedLocations.size(); ++i) {
		if (visitedLocations[i] == id) {
			return true;
		}
	}

	return false;
}