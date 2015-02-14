#include "stdafx.h"
#include "TCharacterInventory.h"

TCharacterInventory Inventory;
//------------------------------------------------------------

TCharacterInventory::TCharacterInventory(void)
{
}

//------------------------------------------------------------

TCharacterInventory::~TCharacterInventory(void)
{
}

//------------------------------------------------------------

void TCharacterInventory::ParseInventory( TSocket *Client, int characterslot, int inventorytype, std::string inventoryarray )
{
	std::vector<std::string> Items = General.split(inventoryarray, ' ');
	std::vector<std::string> Index;
	int itemssize = Items.size();

	//Inventar Items auf 0 setzen
	if(Client->CharacterList[characterslot].Inventory.Initialized != true)
	{
		//Size Allozieren
		Client->CharacterList[characterslot].Inventory.Index.resize(INVENTORY_SIZE);
		Client->CharacterList[characterslot].Inventory.Index[EQUIP_INVENTORY].resize(MAX_SLOTS);
		Client->CharacterList[characterslot].Inventory.Index[MAIN_INVENTORY].resize(MAX_SLOTS);
		Client->CharacterList[characterslot].Inventory.Index[OTHER_INVENTORY].resize(MAX_SLOTS);

		for(auto i = 0; i < INVENTORY_SIZE; i++)
		{
			for(auto y = 0; y < MAX_SLOTS; y++)
			{
				Client->CharacterList[characterslot].Inventory.Index[i][y].inUse = false;
				Client->CharacterList[characterslot].Inventory.Index[i][y].inUse = false;
				Client->CharacterList[characterslot].Inventory.Index[i][y].count = -1;
				Client->CharacterList[characterslot].Inventory.Index[i][y].id = -1;
				Client->CharacterList[characterslot].Inventory.Index[i][y].rare = -1;
				Client->CharacterList[characterslot].Inventory.Index[i][y].upgrade = -1;
			}
		}

		Client->CharacterList[characterslot].Inventory.Initialized = true;
	}

	//Alle Items durchlaufen & die Informationen splitten (Slot etc)
	for(auto i = 0; i < itemssize; i++)
	{
		Index = General.split(Items[i], '.');

		//Items im Inventar hinzufügen
		switch (inventorytype)
		{
		case EQUIP_INVENTORY:
			this->SetItemFromDatabase(Client, characterslot, inventorytype, General.s_atoi(Index[0]), General.s_atoi(Index[1]), 1, General.s_atoi(Index[2]), General.s_atoi(Index[3]));
			break;
		case MAIN_INVENTORY:
			this->SetItemFromDatabase(Client, characterslot, inventorytype, General.s_atoi(Index[0]), General.s_atoi(Index[1]), General.s_atoi(Index[2]), NULL, NULL);
			break;
		case OTHER_INVENTORY:
			this->SetItemFromDatabase(Client, characterslot, inventorytype, General.s_atoi(Index[0]), General.s_atoi(Index[1]), General.s_atoi(Index[2]), NULL, NULL);
			break;
		};
	}
}

//------------------------------------------------------------

void TCharacterInventory::AddItem( TSocket *Client, int inventorytype, int slot, int id, int count, int rare, int upgrade )
{
	std::stringstream ss;

	if(count <= 0)
	{
		return;
	}
	
	inventorytype = this->DetectInventory(Client, inventorytype, id);

	if(inventorytype == UNKNOWN_INVENTORY)
	{
		return;
	}

	if(inventorytype == GOLD_INVENTORY)
	{
		Client->Character->Inventory.gold_count += count;
		ss << "gold " << Client->Character->Inventory.gold_count;
	}else
	{
		slot = this->DetectSlot(Client, inventorytype, slot);

		if(slot == SLOTS_FULL)
		{
			return;
		}

		Client->Character->Inventory.Index[inventorytype][slot].inUse = true;
		Client->Character->Inventory.Index[inventorytype][slot].count = count;
		Client->Character->Inventory.Index[inventorytype][slot].id = id;
		Client->Character->Inventory.Index[inventorytype][slot].rare = rare;
		Client->Character->Inventory.Index[inventorytype][slot].upgrade = upgrade;

		switch (inventorytype)
		{
		case EQUIP_INVENTORY:	
			ss << "ivn " << inventorytype << " " << slot << "." << id << "." << rare << "." << upgrade;
			break;
		case MAIN_INVENTORY:
			ss << "ivn " << inventorytype << " " << slot << "." << id<< "." << count;
			break;
		case OTHER_INVENTORY:
			ss << "ivn " << inventorytype << " " << slot << "." << id << "." << count;
			break;
		};
	}

	//Packet senden zum Item hinzufügen
	Client->Send(ss.str());
}

//------------------------------------------------------------

void TCharacterInventory::SetItemFromDatabase( TSocket *Client, int characterslot, int inventorytype, int slot, int id, int count, int rare, int upgrade )
{
	//Inventar Item übernehmen
	Client->CharacterList[characterslot].Inventory.Index[inventorytype][slot].inUse = true;
	Client->CharacterList[characterslot].Inventory.Index[inventorytype][slot].count = count;
	Client->CharacterList[characterslot].Inventory.Index[inventorytype][slot].id = id;
	Client->CharacterList[characterslot].Inventory.Index[inventorytype][slot].rare = rare;
	Client->CharacterList[characterslot].Inventory.Index[inventorytype][slot].upgrade = upgrade;
}

//------------------------------------------------------------

void TCharacterInventory::RemoveItem( TSocket *Client, int inventorytype, int slot, int amount)
{
	std::stringstream ss;
	int allowed_slot;

	if(Client->Character->Inventory.BackpackActive)
	{
		allowed_slot = 45;
	} else
	{
		allowed_slot = 32;
	}

	//Prüfen ob Slots & Inventartype im richtigen Bereich
	if(slot < 0 || slot > allowed_slot || inventorytype < 0 || inventorytype > 2 || Client->Character->Inventory.Index[inventorytype][slot].inUse == false)
	{
		return;
	}

	bool removed = true;
	//Item in der Inventar Struktur vom Clienten entfernen
	if(amount == -1) //Default - Item komplett löschen
	{
		Client->Character->Inventory.Index[inventorytype][slot].inUse = false;
		Client->Character->Inventory.Index[inventorytype][slot].count = -1;
		Client->Character->Inventory.Index[inventorytype][slot].id = -1;
		Client->Character->Inventory.Index[inventorytype][slot].rare = -1;
		Client->Character->Inventory.Index[inventorytype][slot].upgrade = -1;
	}else
	{
		if((Client->Character->Inventory.Index[inventorytype][slot].count - amount) > 0) //Anzahl reduzieren
		{
			Client->Character->Inventory.Index[inventorytype][slot].count -= amount;
			removed = false;
		}else //Anzahl = 0 -> komplett löschen
		{
			Client->Character->Inventory.Index[inventorytype][slot].inUse = false;
			Client->Character->Inventory.Index[inventorytype][slot].count = -1;
			Client->Character->Inventory.Index[inventorytype][slot].id = -1;
			Client->Character->Inventory.Index[inventorytype][slot].rare = -1;
			Client->Character->Inventory.Index[inventorytype][slot].upgrade = -1;
		}
	}

	//Item entfernen Packet senden
	if(removed)
	{
		ss << "ivn " << inventorytype << " " << slot << ".-1.0";
		Client->Send(ss.str());
	}else //Nur geändert
	{
		switch (inventorytype)
		{
		case MAIN_INVENTORY:
			ss << "ivn " << inventorytype << " " << slot << "." << Client->Character->Inventory.Index[inventorytype][slot].id<< "." << Client->Character->Inventory.Index[inventorytype][slot].count;
			break;
		case OTHER_INVENTORY:
			ss << "ivn " << inventorytype << " " << slot << "." << Client->Character->Inventory.Index[inventorytype][slot].id << "." << Client->Character->Inventory.Index[inventorytype][slot].count;
			break;
		};

		Client->Send(ss.str());
	}
}

//------------------------------------------------------------

void TCharacterInventory::RemoveItemRequest( TSocket *Client, int inventorytype, int slot )
{
	std::stringstream accept;
	accept << "#b_i^" << inventorytype << "^" << slot << "^1";
	Message.Dialog(Client, accept.str(), "#b_i^Cancel", "Do you really want to delete this Item?");
}

//------------------------------------------------------------

void TCharacterInventory::MoveItem( TSocket *Client, int inventorytype, int slot, int count, int newslot )
{
	int allowed_slot;
	int difference;
	structInventory TempItem = {Client->Character->Inventory.Index[inventorytype][slot].inUse, Client->Character->Inventory.Index[inventorytype][slot].id, Client->Character->Inventory.Index[inventorytype][slot].count, Client->Character->Inventory.Index[inventorytype][slot].rare, Client->Character->Inventory.Index[inventorytype][slot].upgrade};
	structInventory TempSecondItem = {Client->Character->Inventory.Index[inventorytype][newslot].inUse, Client->Character->Inventory.Index[inventorytype][newslot].id, Client->Character->Inventory.Index[inventorytype][newslot].count, Client->Character->Inventory.Index[inventorytype][newslot].rare, Client->Character->Inventory.Index[inventorytype][newslot].upgrade};

	//Überprüfen welche Slots erlaubt sind
	if(Client->Character->Inventory.BackpackActive)
	{
		allowed_slot = 45;
	} else
	{
		allowed_slot = 32;
	}

	//Prüfen ob Gültiger Slot, Inventartype etc.
	if(inventorytype < 0 || inventorytype > 2 || slot < 0 || slot > allowed_slot || 
		count > Client->Character->Inventory.Index[inventorytype][slot].count || count < 1 || 
		count > 99 || newslot < 0 || newslot > allowed_slot || 
		Client->Character->Inventory.Index[inventorytype][slot].inUse == false)
	{
		return;
	}

	//Item Moven auf leeren Slot
	if(TempSecondItem.inUse == false)
	{
		//Wenn weniger Items gemoved werden, als vorhanden, nur bestimmte Anzahl moven
		if(TempItem.count > count)
		{
			Client->Character->Inventory.Index[inventorytype][slot].count -= count;
			this->AddItem(Client, inventorytype, slot, TempItem.id, Client->Character->Inventory.Index[inventorytype][slot].count, TempItem.rare, TempItem.upgrade);
			this->AddItem(Client, inventorytype, newslot, TempItem.id, count, TempItem.rare, TempItem.upgrade);
		} else
		{
			this->RemoveItem(Client, inventorytype, slot);
			this->AddItem(Client, inventorytype, newslot, TempItem.id, TempItem.count, TempItem.rare, TempItem.upgrade);
		}
	} else
	{
		//Wenn  auf dem anderen Slot, das gleiche Item, Item addieren
		if(TempItem.id == TempSecondItem.id)
		{
			difference = (TempItem.count + TempSecondItem.count);

			//Wenn die Differenz größer, Items abziehen und Moven, ansonsten direkt addieren
			if(difference > 99)
			{
				difference = TempSecondItem.count - 99;
				Client->Character->Inventory.Index[inventorytype][newslot].count += difference;
				Client->Character->Inventory.Index[inventorytype][slot].count -= difference;
				this->AddItem(Client, inventorytype, slot, TempItem.id, Client->Character->Inventory.Index[inventorytype][slot].count, TempItem.rare, TempItem.upgrade);
				this->AddItem(Client, inventorytype, newslot, TempSecondItem.id, Client->Character->Inventory.Index[inventorytype][newslot].count, TempSecondItem.rare, TempSecondItem.upgrade);
			} else
			{
				Client->Character->Inventory.Index[inventorytype][newslot].count += count;
				Client->Character->Inventory.Index[inventorytype][slot].count -= count;

				//Wenn beim Moven keine Items verfügbar entfernen, ansonsten rest dalassen
				if(Client->Character->Inventory.Index[inventorytype][slot].count <= 0)
				{
					RemoveItem(Client, inventorytype, slot);
				} else
				{
					this->AddItem(Client, inventorytype, slot, TempItem.id, Client->Character->Inventory.Index[inventorytype][slot].count, TempItem.rare, TempItem.upgrade);
				}

				this->AddItem(Client, inventorytype, newslot, TempSecondItem.id, Client->Character->Inventory.Index[inventorytype][newslot].count, TempSecondItem.rare, TempSecondItem.upgrade);
			}

		} else //Ansonsten Item einfach tauschen
		{
			this->RemoveItem(Client, inventorytype, slot);
			this->RemoveItem(Client, inventorytype, newslot);
			this->AddItem(Client, inventorytype, newslot, TempItem.id, TempItem.count, TempItem.rare, TempItem.upgrade);
			this->AddItem(Client, inventorytype, slot, TempSecondItem.id, TempSecondItem.count, TempSecondItem.rare, TempSecondItem.upgrade);
		}
	}
}

//------------------------------------------------------------

int TCharacterInventory::DetectSlot( TSocket *Client, int inventorytype, int slot )
{
	//Wenn nicht auf AUTODETECT_SLOT gesetzt, dann manuell slot besetzen
	if(slot != AUTODETECT_SLOT)
	{
		return slot;
	} else
	{
		//Freien Slot suchen und returnen
		for(auto i = 0; i < MAX_SLOTS; i++)
		{
			if(Client->Character->Inventory.Index[inventorytype][i].inUse == false)
			{
				return i;
			}
		}

		return SLOTS_FULL;
	}
}

//------------------------------------------------------------

int TCharacterInventory::DetectInventory( TSocket *Client, int inventorytype, int itemid )
{
	if(inventorytype != AUTODETECT_INVENTORY)
	{
		return inventorytype;
	} else
	{
		int itemssize = Items.vItems.size();

		//Alle Items ablaufen
		for(auto i = 0; i < itemssize; i++)
		{
			//Wenn Item gefunden, Inventartype returnen
			if(Items.vItems[i].vnum == itemid)
			{
				return Items.vItems[i].inventorytype;
			}
		}

		return UNKNOWN_INVENTORY;
	}
}

//------------------------------------------------------------

void TCharacterInventory::UseItem( TSocket *Client, int inventorytype, int slot )
{
	int used;

	//Prüfen ob Slot überhaupt belegt
	if(Client->Character->Inventory.Index[inventorytype][slot].inUse == true)
	{
		//Item nutzen
		used = Items.MakeItem(Client, Client->Character->Inventory.Index[inventorytype][slot].id);

		//Wenn das Item in der Liste nicht gefunden, returnen
		if(used == ITEM_NOT_FOUND)
		{
			return;
		}

		//Wenn Item gelöscht wird, nachdem benutzen
		if(used)
		{
			//Anzahl um 1 reduzieren
			Client->Character->Inventory.Index[inventorytype][slot].count--;

			//Wenn keine Items mehr vorhanden (Count <= 0) Item löschen, ansonsten reduzierung anzeigen
			if(Client->Character->Inventory.Index[inventorytype][slot].count <= 0)
			{
				this->RemoveItem(Client, inventorytype, slot);
			} else
			{
				this->AddItem(Client, inventorytype, slot, Client->Character->Inventory.Index[inventorytype][slot].id, Client->Character->Inventory.Index[inventorytype][slot].count, Client->Character->Inventory.Index[inventorytype][slot].rare, Client->Character->Inventory.Index[inventorytype][slot].upgrade);
			}
		}  
	}
}

//------------------------------------------------------------

void TCharacterInventory::DropItem(TSocket *Client, int inventory, int slot, int amount)
{
	if(Client->Character->Inventory.Index[inventory][slot].inUse)
	{
		if(Client->Character->Inventory.Index[inventory][slot].count >= amount)
		{
			//drop 2024 441127 8 7 10 0 0
			//drop <modalid> <objectid> <x> <y> <count> <quest> <bound>
			int vnum = Client->Character->Inventory.Index[inventory][slot].id;
			int x = General.CreateRandomNumber(Client->Character->Map.x - 1, Client->Character->Map.x + 1);
			int y = General.CreateRandomNumber(Client->Character->Map.y - 1, Client->Character->Map.y + 1);
			int count = amount;
			int quest = 0;
			int bound = 0;
			int itemid = General.CreateRandomNumber(10000, 90000);
			int rare = Client->Character->Inventory.Index[inventory][slot].rare;
			int upgrade = Client->Character->Inventory.Index[inventory][slot].upgrade;

			int MapIndex = Map.GetMapIndex(Client->Character->Map.id);

			structMapItem MapItem = {itemid, vnum, count, x, y, quest, bound, rare, upgrade};
			Items.lockthread.lock();
			Map.MapData[MapIndex].Item.push_back(MapItem);
			Items.lockthread.unlock();

			//Item entfernen
			Inventory.RemoveItem(Client, inventory, slot, amount);

			std::stringstream message;

			message << "drop " << vnum << " " << itemid << " " << x << " " << y << " " << count << " " << quest << " " << bound;
			
			Map.SendMap(MapIndex, NULL, true, message.str());
		}
	}
}

//------------------------------------------------------------

void TCharacterInventory::GetItem(TSocket *Client, int action, int charid, int itemid)
{
	if(action == 1) // Andere Actions?
	{
		if(Client->Character->CharacterInfo.ID == charid)
		{
			std::stringstream message;
			Items.lockthread.lock();
			int MapIndex = Map.GetMapIndex(Client->Character->Map.id);
			int MapItemIndex = Map.GetMapItemIndex(Client->Character->Map.id, MapIndex, itemid);
			
			if(MapItemIndex != ITEM_NOT_FOUND)
			{
				//If(Item gehört nicht noch einem anderen Char (Drop)) {}
				int distance = Math.Distance(Client->Character->Map.x, Map.MapData[MapIndex].Item[MapItemIndex].x, Client->Character->Map.y, Map.MapData[MapIndex].Item[MapItemIndex].y);
				if(distance <= 3)
				{
					if(!Map.MapData[MapIndex].Item[MapItemIndex].dropbound.drop_bound_is || (Map.MapData[MapIndex].Item[MapItemIndex].dropbound.drop_bound_is && Map.MapData[MapIndex].Item[MapItemIndex].dropbound.drop_bound_charid == Client->Character->CharacterInfo.ID))
					{
						Inventory.AddItem(Client, -1, -1, Map.MapData[MapIndex].Item[MapItemIndex].vnum, Map.MapData[MapIndex].Item[MapItemIndex].amount, Map.MapData[MapIndex].Item[MapItemIndex].rare, Map.MapData[MapIndex].Item[MapItemIndex].upgrade);

						/*
						say 1 825479 12 Du hast ein Item erhalten:Kekse x 10
						icon 1 825479 1 2024
						get 1 825479 441259 0				
						*/
						int itemindex = Items.GetItemIndex(Map.MapData[MapIndex].Item[MapItemIndex].vnum);

						message << "say 1 " << Client->Character->CharacterInfo.ID << " " << ITEM_CHAT << " You got an item: " << Items.vItems[itemindex].name << " x " << Map.MapData[MapIndex].Item[MapItemIndex].amount;
						Client->Send(message.str());
						message.str("");

						message << "icon 1 " << Client->Character->CharacterInfo.ID << " 1 " << Map.MapData[MapIndex].Item[MapItemIndex].vnum;
						Map.SendMap(MapIndex, NULL, true, message.str());
						message.str("");

						message << "get 1 " << Client->Character->CharacterInfo.ID << " " << itemid << " 0";

						Map.SendMap(MapIndex, NULL, true, message.str());
						Map.MapData[MapIndex].Item.erase(Map.MapData[MapIndex].Item.begin()+MapItemIndex);
					}else
					{
						std::stringstream message;
						message << "say 1 " << Client->Character->CharacterInfo.ID << " " << SYSTEM_CHAT << " Item is bound to another player.";
						Client->Send(message.str());
					}
				}
			}
			Items.lockthread.unlock();

		}
	}
}

//------------------------------------------------------------