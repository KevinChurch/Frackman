#include "StudentWorld.h"
#include <string>
using namespace std;

Actor;


GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(std::string assetDir)
	: GameWorld(assetDir)
{
	for (size_t i = 0; i < 64; i++)
		for (size_t j = 0; j < 64; j++)
			dirt[j][i] = nullptr;
	player = nullptr;
	srand(time(NULL));
}

StudentWorld::~StudentWorld()
{
	for (size_t i = 0; i < 64; i++)
		for (size_t j = 0; j < 64; j++)
			if (dirt[j][i] != nullptr)
			{
				delete dirt[j][i];
				dirt[j][i] = nullptr;
			}
	delete player;
	while (!actors.empty())
	{
		delete actors[0];
		actors[0] = actors[actors.size() - 1];
		actors.pop_back();
	}
}

int StudentWorld::init()
{
	
	for (size_t i = 0; i < 64; i++)
		for (size_t j = 0; j < 64; j++)
			dirt[j][i] = nullptr;
	for (size_t i = 0; i < 4; i++)
		for (size_t j = 0; j < 64; j++)
			dirt[j][i] = new Dirt(j, i, this);
	for (size_t i = 4; i < 60; i++)
	{
		for (size_t j = 0; j < 30; j++)
			dirt[j][i] = new Dirt(j, i, this);
		for (size_t j = 34; j < 64; j++)
			dirt[j][i] = new Dirt(j, i, this);
	}
	
	player = new Frackman(this);

	size_t B = min(getLevel() / 2 + 2, 6U);
	size_t G = max(5U - getLevel() / 2, 2U);
	size_t L = min(2 + getLevel(), 20U);

	addInitialActors(B, boulder);
	addInitialActors(L, oil);
	addInitialActors(30, gold);
	cleanUpDirt();

	oilLeft = L;

	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	// Update the Game Status Line
	setDisplayText(); // update the score/lives/level text at screen top
						 // The term “Actors” refers to all Protesters, the player, Goodies,
						 // Boulders, Barrels of oil, Holes, Squirts, the Exit, etc.
						 // Give each Actor a chance to do something
						 //for each of the actors in the game world

	//Allow the player to make a move
	player->doSomething();

	// If the player has collected all of the Barrels on the level, then
	// return the result that the player finished the level
	if (oilLeft == 0)
	{
		playSound(SOUND_FINISHED_LEVEL);
		return GWSTATUS_FINISHED_LEVEL;
	}

	for (size_t i = 0; i < actors.size(); i++)
	{
		if (actors[i]->isAlive()) //actor[i] is still active / alive
		{
			// ask each actor to do something (e.g. move)
			actors[i]->doSomething();		
		}
	}
	// Remove newly-dead actors after each tick
		removeDeadGameObjects(); // delete dead game objects
	// return the proper result
	if (player->isAlive() == false)
	{
		decLives();
		playSound(SOUND_PLAYER_GIVE_UP);
		return GWSTATUS_PLAYER_DIED;
	}

	possiblyDropGoodie();
	// the player hasn’t completed the current level and hasn’t died
	// let them continue playing the current level
	return GWSTATUS_CONTINUE_GAME;
}


void StudentWorld::cleanUp()
{
	for (size_t i = 0; i < 64; i++)
		for (size_t j = 0; j < 64; j++)
			if (dirt[j][i] != nullptr)
			{
				delete dirt[j][i];
				dirt[j][i] = nullptr;
			}
	delete player;
	while (!actors.empty())
	{
		delete actors[0];
		actors[0] = actors[actors.size() - 1];
		actors.pop_back();
	}
}

void StudentWorld::removeDeadGameObjects()
{
	for (size_t i = 0; i < actors.size(); i++)
	{
		if (!actors[i]->isAlive())
		{
			delete actors[i];
			actors[i] = actors[actors.size() - 1];
			actors.pop_back();
		}
	}
}

bool StudentWorld::deleteDirtRow(int x, int y, bool vertical)
{
	bool dugDirt = false;

	if (vertical)
	{
		for (int i = y; i < y + 4; i++)
		{
			if (dirt[x][i])
			{
				dugDirt = true;
				delete dirt[x][i];
				dirt[x][i] = nullptr;
			}
		}
	}
	else
	{
		for (int i = x; i < x + 4; i++)
		{
			if (dirt[i][y])
			{
				dugDirt = true;
				delete dirt[i][y];
				dirt[i][y] = nullptr;
			}
		}
	}

	return dugDirt;
}

void StudentWorld::addActor(Actor* a)
{
	actors.push_back(a);
}

void StudentWorld::addInitialActors(int num, int type)
{
	Actor* temp;

	int x, y;


	for (size_t i = 0; i < num; i++)
	{
		bool tooShort = false;
		do {
			x = randX();
			y = randY();
			tooShort = false;
			for (size_t i = 0; i < actors.size() && !tooShort; i++)
			{
				if (distance(x, y, actors[i]->getX(), actors[i]->getY()) < 6) //If items are less than 6 squares away from each other
					tooShort = true;
			}
		} while (tooShort);
	

		if (type == boulder)
			addActor(new Boulder(this, x, y));
		else if (type == gold)
			addActor(new Gold(this, x, y, 0));//0 : frackman can pickup
		else if (type == oil)
			addActor(new OilBarrel(this, x, y));

	}

}

void StudentWorld::cleanUpDirt()
{
	for (size_t i = 0; i < actors.size(); i++)
		if (!actors[i]->canActorsPassThroughMe())
		{
			for (int x = actors[i]->getX(); x < actors[i]->getX() + 4; x++)
				for (int y = actors[i]->getY(); y < actors[i]->getY() + 4; y++)
				{
					delete dirt[x][y];
					dirt[x][y] = nullptr;
				}
		}

}


bool StudentWorld::closeToBoulder(int x, int y)
{
	int tooClose = false;

	for (size_t i = 0; i < actors.size(); i++)
	{
		if (distance(x, y, actors[i]->getX(), actors[i]->getY()) <= 3 && !actors[i]->canActorsPassThroughMe())
			tooClose = true;
	}

	return tooClose;
}

// Can actor move to x,y?
bool StudentWorld::canActorMoveTo(Actor* a, int x, int y) const
{
	if (y < 0  || y > 63 || x < 0 || x > 63)
		return false;

	for (int i = x; i < x + 4; i++)
	{
		if (dirt[i][y])
			return false;
	}
	for (int i = 0; i < actors.size(); i++)
	{
		if ((actors[i]->getX() == x - 3 || actors[i]->getX() == x - 2 ||
			actors[i]->getX() == x - 1 || actors[i]->getX() == x ||
			actors[i]->getX() == x + 1 || actors[i]->getX() == x + 2 ||
			actors[i]->getX() == x + 3) && actors[i]->getY() == y - 3
			&& !actors[i]->canActorsPassThroughMe())
			return false;
	}
			

	return true;
}

// Annoy all other actors within radius of annoyer, returning the
// number of actors annoyed.
int StudentWorld::annoyAllNearbyActors(Actor* annoyer, int points, int radius)
{
	int numOfAnnoyed = 0;

	if (distance(annoyer->getX(), annoyer->getY(), player->getX(), player->getY()) <= radius)
	{
		player->annoy(points);
		numOfAnnoyed++;
	}

	for (int i = 0; i < actors.size(); i++)
	{
		if (distance(annoyer->getX(), annoyer->getY(), actors[i]->getX(), actors[i]->getY()) <= radius)
		{
			if (actors[i]->annoy(points))
				numOfAnnoyed++;
		}
	}

	return numOfAnnoyed;
}

void StudentWorld::revealAllNearbyObjects(int x, int y, int radius)
{
	for (size_t i = 0; i < actors.size(); i++)
	{
		if (distance(actors[i]->getX(), actors[i]->getY(), x, y) <= 12)
			actors[i]->setVisible(true);
	}
}

bool StudentWorld::isNearFrackMan(Actor* a, int radius) const
{
	return (distance(a->getX(), a->getY(), player->getX(), player->getY()) <= radius);
}

void StudentWorld::giveFrackManWater()
{
	player->giveSquirt();
}

void StudentWorld::giveFrackManSonar()
{
	player->giveSonar();
}

void StudentWorld::giveFrackManGold()
{
	player->giveGold();
}

void StudentWorld::possiblyDropGoodie()
{
	int G = getLevel() * 25 + 300;

	int x = 0, y = 0;
	if (rand() % G == 0)
	{
		if (rand() % 5 == 0)
		{
			addActor(new Sonar(this));
		}
		else
		{
			while (!isPositionEmpty(x = rand() % VIEW_WIDTH - SPRITE_WIDTH + 1,
				y = rand() % VIEW_HEIGHT - SPRITE_HEIGHT + 1))
				;
			addActor(new WaterRefill(this, x, y));
		}
	}
}

bool StudentWorld::isPositionEmpty(int x, int y)
{

	for (int i = x; i < x + 4; i++)
	{
		for (int j = y; j < y + 4; j++)
		{
			if (dirt[i][j] != nullptr)
				return false;
		}
	}
	/*
	for (int i = 0; i != actors.size(); i++)
	{
		if (actors[i]->isOverlappingArea(x, y))
			return false;
	}
	*/
	return true;

}

Actor* StudentWorld::findNearbyPickerUpper(Actor* a, int radius) const
{
	for (int i = 0; i < actors.size(); i++)
	{
		if (distance(a->getX(), a->getY(), actors[i]->getX(), actors[i]->getY()) <= radius
			&& actors[i]->canPickThingsUp())
			return actors[i];
	}

	return nullptr;
}
/*
// Add an actor to the world.
void addActor(Actor* a);

// Can actor move to x,y?
bool canActorMoveTo(Actor* a, int x, int y) const;



// Reveal all objects within radius of x,y.
void revealAllNearbyObjects(int x, int y, int radius);

// If the FrackMan is within radius of a, return a pointer to the
// FrackMan, otherwise null.
Actor* findNearbyFrackMan(Actor* a, int radius) const;

// If at least one actor that can pick things up is within radius of a,
// return a pointer to one of them, otherwise null.
Actor* findNearbyPickerUpper(Actor* a, int radius) const;

// Annoy the FrackMan.
void annoyFrackMan();

// Give FrackMan some sonar charges.
void giveFrackManSonar();

// Give FrackMan some water.
void giveFrackManWater();

// Is the Actor a facing toward the FrackMan?
bool facingTowardFrackMan(Actor* a) const;

// If the Actor a has a clear line of sight to the FrackMan, return
// the direction to the FrackMan, otherwise GraphObject::none.
GraphObject::Direction lineOfSightToFrackMan(Actor* a) const;

// Return whether the Actor a is within radius of FrackMan.
bool isNearFrackMan(Actor* a, int radius) const;

// Determine the direction of the first move a quitting protester
// makes to leave the oil field.
GraphObject::Direction determineFirstMoveToExit(int x, int y);

// Determine the direction of the first move a hardcore protester
// makes to approach the FrackMan.
GraphObject::Direction determineFirstMoveToFrackMan(int x, int y);
*/

string formatStats(int score, int level, int lives, int health, int squirts, int gold, int sonar, int barrelsLeft)
{
	string stats, hold;

	stringstream ss;
	//ss.flags();


	ss << "Scr: ";
	ss.width(6);
	ss.fill('0');
	ss << score;
	ss << "  Lvl: ";
	ss.fill(' ');
	ss.width(2);
	ss << level;
	ss << "  Lives: ";
	ss.width(1);
	ss << lives;
	ss << "  Hlth: ";
	ss.width(2);
	ss << health*10 << "%  Wtr: " << setw(2) << squirts << "  Gld: " << setw(2) << gold << "  Sonar: " << setw(2) << sonar << "  Oil Left: " << setw(2) << barrelsLeft;
	
	stats = ss.str();
	return stats;
}

void StudentWorld::setDisplayText()
{
	int score = getScore();//getCurrentScore();
	int level = getLevel(); //getCurrentGameLevel();
	int lives = getLives(); //getNumLivesLeft();
	int health = player->getHealth(); //getCurrentHealth();
	int squirts = player->getSquirt(); //getSquirtsLeftInSquirtGun();
	int gold = player->getGold(); //getPlayerGoldCount();
	int sonar = player->getSonar(); //getPlayerSonarChargeCount();
	int barrelsLeft = oilLeft; //getNumberOfBarrelsRemainingToBePickedUp();
	// Next, create a string from your statistics, of the form:
	// Scr:	321000		Lvl:	52		Lives:	3		Hlth:		80%		Wtr:	20		Gld:		3		Sonar:		1		Oil	Left:	2
	string s = formatStats(score, level, lives, health, squirts, gold, sonar, barrelsLeft);
	// Finally, update the display text at the top of the screen with your
	// newly created stats
	setGameStatText(s); // calls our provided GameWorld::setGameStatText
}

double distance(double x1, double y1, double x2, double y2)
{
	return (sqrtf(pow(x1 - x2, 2) + pow(y1 - y2, 2)));
}