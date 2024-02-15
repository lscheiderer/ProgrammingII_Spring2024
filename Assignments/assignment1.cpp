
#include<iostream>
#include<vector>
#include <algorithm>
#include <random>

struct Room
{
	enum class Name { cell, gate, armory, jailers, exit };
	enum class Direction { N, S, E, W, none };
	std::string message; // the message displayed when in the room.
	std::vector<Direction> doors;// the directions in which there are doors.
	std::vector<Name> connectedRoom;// the name of the room in which the corresponding door leads.
	//NOTE:: these two vectors will eventually become a std::map after the STL level.
	bool hasKey{ false }; //whether or not the player has picked up the key.
};
//struct keeps track of the room the player is in, their health, and rather or not they have picked up the key.
struct Player
{
	Room::Name currentRoom{ Room::Name::cell };
	int health{ 10 };
	bool hasKey{ false };
};
/* GUI Functions */
void clearConsole();
void pauseConsole();
void splashScreen();
void displayGameState(const Player& player, const std::vector<Room>& map);
void displayGameDone(const Player& player, const std::vector<Room>& map);
void displayIllegalMove(const Player& player, const Room::Direction& direction);
Room::Direction getAction();

/* Engine Functions*/
std::vector<Room> buildMap();
void randomizeKey(std::vector<Room>& map);
bool changeGameState(Room::Direction& direction, Player& player, std::vector<Room>& map);
bool gameIsNotDone(const Player& player);

int main() {

	/*Splash Screen*/
	clearConsole();
	splashScreen();

	/*set up game*/
	std::vector<Room> map{ buildMap() };

	//used to randomize key placement
	char randkey{};
	std::cout << "Would you like to randomize the key? (y or n)" << std::endl;
	std::cin >> randkey;
	if (randkey == 'y')
	{
		randomizeKey(map);
	}
	else
	{
		map.at(3).hasKey = true;
	}

	Player player;

	/*Until Game Termination Condition*/
	while (gameIsNotDone(player))
	{

		/*Display Game State*/
		clearConsole();
		displayGameState(player, map);

		/*Collect Player Action*/
		Room::Direction action{ getAction() };

		/*Update Game State*/
		if (!changeGameState(action, player, map))
		{
			displayIllegalMove(player, action);
		}
	}

	/*Display Termination Game State*/
	displayGameDone(player, map);

	return 0;

}


std::vector<Room> buildMap()
{
	std::vector<Room> map;
	map.push_back(
		{
			"A small, dark prison cell with doors South and East.",
			{Room::Direction::S, Room::Direction::E},
			{Room::Name::armory, Room::Name::gate},
			false
		});
	map.push_back(
		{
			"A large, torchlit room with doors West, South, and East. \n There is daylight entering under the door to the East.",
			{Room::Direction::W, Room::Direction::S, Room::Direction::E},
			{Room::Name::cell, Room::Name::jailers, Room::Name::exit},
			false
		});
	map.push_back(
		{
			"A store room with doors North and East.",
			{Room::Direction::N, Room::Direction::E},
			{Room::Name::cell, Room::Name::jailers},
			false
		});
	map.push_back(
		{
			"A jailer's barracks with doors West and North.",
			{Room::Direction::W, Room::Direction::N},
			{Room::Name::armory, Room::Name::gate},
			false
		});
	map.push_back(
		{
			"YOU FOUND THE KEY AND ESCAPED!",
			{},
			{},
			false
		});
	return map;
}

void clearConsole() {
	system("cls");
}

void pauseConsole() {
	system("PAUSE");
}

void splashScreen() {
	std::cout << "DUNGEON ADVENTURE" << std::endl;
	std::cout << std::endl;
	std::cout << "Your name here (2020)" << std::endl;
	std::cout << "CPSC 2376, Programming II, Level 1 Quest" << std::endl;
	std::cout << "UA Little Rock, Computer Science Dept." << std::endl;
	std::cout << std::endl;
	std::cout << "INSTRUCTIONS:" << std::endl;
	std::cout << std::endl;
	std::cout << "Find the key and get out of the dungeon!" << std::endl;
	std::cout << std::endl;
	std::cout << "        (North)          " << std::endl;
	std::cout << "           w             " << std::endl;
	std::cout << "           |             " << std::endl;
	std::cout << "(West) a --+-- d (East)  " << std::endl;
	std::cout << "           |             " << std::endl;
	std::cout << "           s             " << std::endl;
	std::cout << "        (South)          " << std::endl;
	std::cout << std::endl;
	pauseConsole();
}

//randomly place the key in either the cell, the armory, the jailer's barrack, or the gate room.
void randomizeKey(std::vector<Room>& map) //you will need to decide on the return type and parameters!
{
	std::random_device seed;
	std::default_random_engine engine(seed());
	std::uniform_int_distribution<int> keyplaces(1, 4);
	int kplacement = keyplaces(engine) - 1;
	if (kplacement == 0)
	{
		map.at(0).hasKey = true;
	}
	else
	{
		map.at(0).hasKey = false;
	}

	if (kplacement == 1)
	{
		map.at(1).hasKey = true;
	}
	else
	{
		map.at(1).hasKey = false;
	}

	if (kplacement == 2)
	{
		map.at(2).hasKey = true;
	}
	else
	{
		map.at(2).hasKey = false;
	}

	if (kplacement == 3)
	{
		map.at(3).hasKey = true;
	}
	else
	{
		map.at(3).hasKey = false;
	}

}

//output information as demonstrated in the instructions.
//You will need to deal with display changes that depend on the key.
//NOTE:: NO game variables such as the player or rooms should be modified in this function!!!
void displayGameState(const Player& player, const std::vector<Room>& map)
{
	Room currentroom = map.at((int)player.currentRoom);
	std::cout << "View:" << currentroom.message << std::endl;
	std::cout << "Health: " << player.health << std::endl;
	if (player.hasKey == true)
	{
		std::cout << "Equip: Key" << std::endl;
	}
	else
	{
		std::cout << "Equip: " << std::endl;
	}
}
//output messages depending on if the player has one or lost.
void displayGameDone(const Player& player, const std::vector<Room>& map)
{
	if (player.currentRoom == Room::Name::exit && player.hasKey == true)
	{
		std::cout << map.at(4).message << std::endl;
	}
	if (player.health == 0)
	{
		std::cout << "There is a trail of blood behind you, you fall to your knees, the world fades... \n"
			"YOU HAVE DIED" << std::endl;
	}

}

//output illegal move bmessages depending if player tries to exit without the key
//or the player tries to exit the wrong way out of a room.
void displayIllegalMove(const Player& player, const Room::Direction& direction)
{

	if (player.currentRoom == Room::Name::gate && player.hasKey == false && direction == Room::Direction::E)
	{
		std::cout << "You pat your pockets for a key, there is not one, its probably around here somewhere..." << std::endl;
		pauseConsole();
	}
	else
	{
		std::cout << "There is a wall, thus no door, thus cannot go that way." << std::endl;
		pauseConsole();
	}
}

//ask for user input, convert char to Room::Direction
Room::Direction getAction()
{
	char userinpt{};
	std::cout << "Where would you like to go?" << std::endl;
	std::cin >> userinpt;
	if (userinpt == 'w')
	{
		return (Room::Direction::N);
	}
	if (userinpt == 'a')
	{
		return (Room::Direction::W);
	}
	if (userinpt == 'd')
	{
		return (Room::Direction::E);
	}
	if (userinpt == 's')
	{
		return (Room::Direction::S);
	}
	else
	{
		return (Room::Direction::none);
	}
}

//This function is the only one that should make modifications to the player/rooms including picking up the key.
// This function should call other functions in order to display illegal moves and/or move the player to a new room.
// If the player moves to a new room, health needs to be decremented.
// Look at int main to decide on return type.

bool changeGameState(Room::Direction& direction, Player& player, std::vector<Room>& map)
{
	player.health = player.health - 1;
	if (map.at((int)player.currentRoom).hasKey == true)
	{
		player.hasKey = true;
	}
	Room currentroom = map.at((int)player.currentRoom);

	int index{};
	for (auto doors : currentroom.doors)
	{
		if (direction == doors)
		{
			for (int i = 0; i < currentroom.doors.size(); ++i)
			{
				if (currentroom.doors[i] == direction)
				{
					index = i;
					break;
				}
			}
			if (index >= 0 && index < currentroom.connectedRoom.size())
			{
				Room::Name adjroom = currentroom.connectedRoom[index];
				if (adjroom == Room::Name::exit && player.hasKey == false)
				{
					player.health = player.health + 1;
					return false;
				}
				else
				{
					player.currentRoom = adjroom;
					return true;
				}
			}
		}
	}
	player.health = player.health + 1;
	return false;
}




//Check the end-of-game conditions.  i.e If the player is out
//of health or the player has reached the exit
bool gameIsNotDone(const Player& player)
{
	if (player.health == 0)
	{
		return false;
	}
	if (player.currentRoom == Room::Name::exit)
	{
		if (player.hasKey == true)
		{
			return false;
		}
	}

}