#pragma once

#include <memory>
#include <string>


/**
 * Base class for all items that can be found in the game
 */
class BaseItem {
public:
	BaseItem(const std::string& id, const std::string& title, bool consumable);

	virtual void useItem() = 0;

	std::string id;
	std::string title;
	bool consumable;
};


/**
 * An item that can teleport you to a pre-determined location on the map.
 */
class TeleportItem : public BaseItem {
public:
	TeleportItem(const std::string& itemId, const std::string& title, const std::string& targetLocationId);

	virtual void useItem();

protected:
	std::string targetLocationId;
};


/**
 * When you use this type of item, it simply prints some text to the console. Could be for notes or keys etc.
 */
class NoteItem : public BaseItem {
public:
	NoteItem(const std::string& id, const std::string& title, const std::string& useText);

	virtual void useItem();

protected:
	std::string useText;
};

