#include "items.h"

#include "game.h"
#include "gamedata.h"

#include <iostream>


//
// BaseItem
//

BaseItem::BaseItem(const std::string& id, const std::string& title, bool consumable)
{
	this->id = id;
	this->title = title;
	this->consumable = consumable;
}



//
// TeleportItem: Teleports you to a pre-determined location
//

TeleportItem::TeleportItem(const std::string& id, const std::string& title, const std::string& targetLocationId) :
	BaseItem(id, title, true)
{
	this->targetLocationId = targetLocationId;
}


void TeleportItem::useItem()
{
	std::shared_ptr<Location> target = Game::Instance().gameData.getLocationWithId(targetLocationId);

	if (target == nullptr) {
		std::cout << "[Error] Target location not found: " << targetLocationId << "\n";
		return;
	}
	
	std::cout << "You magically disappear and re-appear in another place!\n";
	Game::Instance().player.currentLocation = target;
}



//
// NoteItem: Simply prints a note to the console when used
//

NoteItem::NoteItem(const std::string& id, const std::string& title, const std::string& useText) :
	BaseItem(id, title, false)
{
	this->useText = useText;
}


void NoteItem::useItem()
{
	std::cout << useText << "\n";
}
