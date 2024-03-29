/*This code is a possible solution to last weeks power play, and will be used as the basis for this powerplay; solution given by instructor*/
/*Promt = There are 5 for loops in my solution, we are going to switch the ones we can to STL algorithms. For my solution:
one used std::generate 
note::you have to create a vector with the right number of elements.
std::vector<int> v(4); would create a vector with 4 elements.
one used std::remove_if 
three used std::for_each NOTE::this is for practice. Most of these would be just fine as a ranged based for loop
NOTE:: for the lambda parameter for player.inventory, you will want to use: std::pair<Item::Type, Item>*/
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <fstream>
#include <map>



struct Item
{
	enum class Type { sword, armor, shield, numTypes };
	Type clasification;
	int bonusValue;
};
 
struct Object
{
	enum class Type { player, slime, orc, sprite, dragon, numTypes };
	Type name;
	int strength{ 0 };
	int health{ 0 };
	int level{ 0 };
	std::map<Item::Type, Item> inventory;
};
std::vector<Object> createMonsters(const Object& player );
void monsterAttack(Object& player, const std::vector<Object>& monsters);
void bringOutYourDead(std::vector<Object>& monsters);void playerAttack(const Object& player, std::vector<Object>& monsters);
void levelUp(Object& player);
int calculateAC(const Object& object);
void heal(Object& object);void displayBattle(const Object& player, const std::vector<Object>& monsters);void printName(const Object& object);
void printItem(const Item & item);
int attack(const Object& object);
void defend(Object& object, int damage);std::random_device seed;
std::default_random_engine engine(seed());
int main()
{
	Object player{ Object::Type::player, 0,1,0, {} };	
	std::vector<Object> monsters(4);
	while (player.health > 0)
	{
		levelUp(player);
		monsters = createMonsters(player);		
		std::cout << monsters.size() << " monster(s) approaches!!" << std::endl;
		system("pause");
		system("cls");		
		while (player.health > 0 && monsters.size() > 0)
		{			displayBattle(player, monsters);			std::cout << "What do you do? (a)ttack (h)eal ";
			char command{ 'x' };
			std::cin >> command;
			switch (command)
			{
			case 'a':
			{
				playerAttack(player, monsters);
				break;
			}
			case 'h':
				heal(player);
				break;
			default:
				std::cout << "please enter a or h" << std::endl;
				break;
			}			bringOutYourDead(monsters);			monsterAttack(player, monsters);			system("PAUSE");
			system("CLS");
		}
	}	
	if (player.health <= 0)
	{
		std::cout << "You Have Died" << std::endl;
	}
	if (player.health <= 0 && monsters.size() == 0)
	{
		std::cout << "BUT" << std::endl;
	}
	if (monsters.size() == 0)
	{
		std::cout << "You have killed the monsters!!!" << std::endl;
	}
	system("PAUSE");}

void displayBattle(const Object& player, const std::vector<Object>& monsters)
{
	printName(player);
	std::cout << " h:" << player.health << std::endl;
	auto invfun = [&](const Item& item)
	{
		std::cout << "  ";
		printItem(item);
		std::cout << std::endl;
	};
	std::for_each(player.inventory.begin(), player.inventory.end(), invfun);

	auto printmonst = [&](const Object& monster)
	{
		std::cout << "   "<< ". ";
		printName(monster);
		std::cout << " h:" << monster.health << std::endl;
	};
	std::for_each(monsters.begin(),monsters.end(),printmonst);
}
std::vector<Object> createMonsters(const Object& player)
{
	std::vector<Object> monsters;
	std::normal_distribution<double> randomNumMonsters((double)player.level, player.level / 2.0);
	int numMonsters{ std::max(1, (int)randomNumMonsters(engine)) };	
	auto genmonst = [&] (const Object& monster)
	{
		std::normal_distribution<double> monsterLevel((float)player.level, player.level / 4.0);
		int level{ std::max(1, (int)monsterLevel(engine)) };		
		std::uniform_int_distribution<int> monsterType(1, (int)Object::Type::numTypes - 1);
		Object::Type name{ (Object::Type)monsterType(engine) };		
		double strengthVariance{ 0.0 };
		double healthVariance{ 0.0 };
		switch (name)
		{
		case Object::Type::slime:
			strengthVariance = level * 1.5;
			healthVariance = level * 1.25;
			break;
		case Object::Type::orc:
			strengthVariance = level * 2;
			healthVariance = level * level * 1.25;
			break;
		case Object::Type::sprite:
			strengthVariance = level * 1.75;
			healthVariance = level;
			break;
		case Object::Type::dragon:
			strengthVariance = level * 6;
			healthVariance = level * level * 3;
			break;
		}
		std::normal_distribution<double> randomStrength(strengthVariance, level / 4.0);
		std::normal_distribution<double> randomHealth(healthVariance*5, level / 2.0);	
		monsters.push_back(
			{ 
				name, 
				std::max(1, (int)randomStrength(engine)), 
				std::max(1, (int)randomHealth(engine)), 
				level, 
				{} 
			});
	return monsters;
	};
	std::generate(monsters.begin(),monsters.end(), genmonst);
	
};
/*
	for (int i{ 0 }; i < numMonsters; i++)
	{
		//set level based on player level
		std::normal_distribution<double> monsterLevel((float)player.level, player.level / 4.0);
		int level{ std::max(1, (int)monsterLevel(engine)) };		
		std::uniform_int_distribution<int> monsterType(1, (int)Object::Type::numTypes - 1);
		Object::Type name{ (Object::Type)monsterType(engine) };		
		double strengthVariance{ 0.0 };
		double healthVariance{ 0.0 };
		switch (name)
		{
		case Object::Type::slime:
			strengthVariance = level * 1.5;
			healthVariance = level * 1.25;
			break;
		case Object::Type::orc:
			strengthVariance = level * 2;
			healthVariance = level * level * 1.25;
			break;
		case Object::Type::sprite:
			strengthVariance = level * 1.75;
			healthVariance = level;
			break;
		case Object::Type::dragon:
			strengthVariance = level * 6;
			healthVariance = level * level * 3;
			break;
		}
		std::normal_distribution<double> randomStrength(strengthVariance, level / 4.0);
		std::normal_distribution<double> randomHealth(healthVariance*5, level / 2.0);	
		monsters.push_back(
			{ 
				name, 
				std::max(1, (int)randomStrength(engine)), 
				std::max(1, (int)randomHealth(engine)), 
				level, 
				{} 
			});
	}
	return monsters;
}*/
void monsterAttack(Object& player, const std::vector<Object>& monsters)
{
	std::bernoulli_distribution willAttack(.75);
	std::cout << std::endl;
	auto attackornot = [&](Object& monster, std::bernoulli_distribution& engine)
	{
				if (willAttack(engine))
		{
			printName(monster);
			std::cout << " attacks!" << std::endl;
			defend(player, attack(monster));
		}
		else
		{
			printName(monster);
			std::cout << " twiddles its thumbs" << std::endl;
		}
	};
	std::for_each(monsters.begin(),monsters.end(),attackornot);
}
void playerAttack(const Object& player, std::vector<Object>& monsters)
{
	std::cout << "Which Monster: ";
	int monsterNum{ 0 };
	std::cin >> monsterNum;
	if (monsterNum > 0 && monsterNum <= monsters.size())
	{
		defend(monsters[monsterNum - 1], attack(player));
	}
}
void levelUp(Object& player)
{
	player.level++;
	std::normal_distribution<double> randomHealth(20.0 + player.level * 5, 5.0);
	player.health += std::max(1, (int)randomHealth(engine));	std::normal_distribution<double> randomStrength(3.0 + player.level, 1.0);
	player.strength += std::max(1, (int)randomStrength(engine));	//grab new item.
	std::uniform_int_distribution<int> randomItem(0, (int)Item::Type::numTypes - 1);
	std::normal_distribution<double> randomBonus((double)player.level, (double)player.level / 2);
	Item found{ (Item::Type)randomItem(engine), std::max(1, (int)randomBonus(engine)) };	std::cout << "You found a ";
	printItem(found);
	std::cout << "!!!!" << std::endl;
	if (
		auto haveOne{ player.inventory.find(found.clasification) };
		haveOne == player.inventory.end()
		|| player.inventory[found.clasification].bonusValue < found.bonusValue
		)
	{
		std::cout << "You keep the shiny new toy!" << std::endl;
		player.inventory[found.clasification] = found;
	}
	else
	{
		std::cout << "You toss aside the ugly old thing!" << std::endl;
	}
}
int calculateAC(const Object& object)
{
	int AC{ 0 };
	if (auto armor{ object.inventory.find(Item::Type::armor) }; armor != object.inventory.end())
	{
		AC += armor->second.bonusValue;
	}
	if (auto shield{ object.inventory.find(Item::Type::shield) }; shield!= object.inventory.end())
	{
		AC += shield->second.bonusValue;
	}
	return AC;
}
void printName(const Object& object)
{
	std::cout << "L:" << object.level << " ";
	switch (object.name)
	{
	case Object::Type::player:
		std::cout << "Player";
		break;
	case Object::Type::slime:
		std::cout << "Slime";
		break;
	case Object::Type::orc:
		std::cout << "Orc";
		break;
	case Object::Type::sprite:
		std::cout << "Sprite";
		break;
	case Object::Type::dragon:
		std::cout << "Dragon";
		break;
	}	
}
void printItem(const Item& item)
{
	switch (item.clasification)
	{
	case Item::Type::armor:
		std::cout << "Armor";
		break;
	case Item::Type::shield:
		std::cout << "Shield";
		break;
	case Item::Type::sword:
		std::cout << "Sword";
		break;
	}
	std::cout << "+" << item.bonusValue;
}
int attack(const Object& object)
{
	int potentialDamage{ object.strength };
	if (auto sword{ object.inventory.find(Item::Type::sword) }; sword != object.inventory.end())
	{
		potentialDamage += sword->second.bonusValue;
	}
	std::normal_distribution<double> damageDealt(potentialDamage, 2.0);	printName(object);
	std::cout << " deals ";
	return std::max(1, (int)damageDealt(engine));
}
void defend(Object& object, int damage)
{
	std::normal_distribution<double> defense(calculateAC(object), 1.0 / object.level);
	damage = std::max(0, damage-(int)defense(engine));
	std::cout << damage << " damage to ";
	printName(object);
	std::cout  << "!!!" << std::endl;
	object.health -= damage;
}
void heal(Object& object)
{
	std::normal_distribution<double> randomHeal(object.strength, 3.0);
	int  amountHealed{ std::max(1, (int)randomHeal(engine)) };
	printName(object);
	std::cout << " is healed by " << amountHealed << "hp!" << std::endl;
	object.health += amountHealed;
}
void bringOutYourDead(std::vector<Object>& monsters)
{
	auto deathPrint = [](const Object& obj) {printName(obj); std::cout << " has died!!!" << std::endl << std::endl;};
	monsters.erase(std::remove_if(monsters.begin(), monsters.end(), [&deathPrint](const Object& monster) {
    	if (monster.health <= 0) 
		{
            deathPrint(monster);
                return true;
        }
    	return false;
        }),
         monsters.end());

}