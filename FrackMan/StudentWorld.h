#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>


using namespace std;

void updateDisplayText();
bool theplayerCompletedTheCurrentLevel();
double distance(double x1, double y1, double x2, double y2);
int randX();
int randY();


class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir);
	~StudentWorld();
	void removeDeadGameObjects();
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	bool deleteDirtRow(int x, int y, bool vertical);
	bool closeToBoulder(int x, int y);
	void setDisplayText();
	void decOilLeft() { oilLeft--; }

	// Add an actor to the world.
	void addActor(Actor* a);

	bool isPositionEmpty(int x, int y);

	// Can actor move to x,y?
	bool canActorMoveTo(Actor* a, int x, int y) const;

	// Annoy all other actors within radius of annoyer, returning the
	// number of actors annoyed.
	int annoyAllNearbyActors(Actor* annoyer, int points, int radius);

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

	//Give Frackman some Gold
	void giveFrackManGold();

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

	void possiblyDropGoodie();

private:
	void addInitialActors(int num, int type);

	//Clean up dirt of all allocated boulders
	void cleanUpDirt();
	vector<Actor*> actors;
	Frackman* player;
	Dirt* dirt[64][64];
	int oilLeft;
	enum{boulder, gold, oil}; //type of initial pickups

};




#endif // STUDENTWORLD_H_
