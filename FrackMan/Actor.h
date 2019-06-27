#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

#include <ctime>
#include <random>
#include <cmath>


class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#define PLAYER_STARTX 30
#define PLAYER_STARTY 60

#define SPRITE_HEIGHT 4
#define SPRITE_WIDTH 4

#define VIEW_HEIGHT 64
#define VIEW_WIDTH 64

int randX();
int randY();

class Actor : public GraphObject
{
public:
	Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* world) :
		GraphObject(imageID, startX, startY, dir, size, depth), m_world (world)
	{
		m_alive = true;
		//setVisible(true);
	}
	virtual ~Actor() 
	{
		m_world = nullptr;
	}

	// Action to perform each tick.
//	virtual void move(); // = 0

	// Is this actor alive?
	virtual bool isAlive() const { return m_alive; }

	// Get this actor's world.
	StudentWorld* getWorld() const { return m_world; }

	// Mark this actor as dead.
	void setDead() { m_alive = false; }

	// Annoy this actor.
	virtual bool annoy(unsigned int amt) { return false; }

	// Can other actors pass through this actor?
	virtual bool canActorsPassThroughMe() const { return true; }

	// Can this actor need to be picked up to finish the level?
	virtual bool neededToFinishLevel() const { return false; }

	// Can this actor pick items up?
	virtual bool canPickThingsUp() const { return false; }

	// Can be Picked up?
	virtual bool canBePickedUpByFrackman() const { return false; }

	//is actor overlapping 4x4 area
	virtual bool isOverlappingArea(int x, int y) const;

	// Does this actor hunt the FrackMan?
	virtual bool huntsFrackMan() const { return false; }

/*	// Can this actor dig through dirt?
	virtual bool canDigThroughDirt() const;

	// Move this actor to x,y if possible, and return true; otherwise,
	// return false without moving.
	bool moveToIfPossible(int x, int y);
*/



	virtual void doSomething() { return; };
	//int randX();
	//int randY();
private:
	StudentWorld* m_world;
	bool m_alive;

};

class Human : public Actor
{
public:
	Human(int imageID, int startX, int startY, Direction dir, int health, StudentWorld* world) :
		Actor(imageID, startX, startY, dir, 1, 0, world), m_hp(health)
	{
		setVisible(true);
	}
	virtual ~Human()
	{}
	virtual bool annoy(unsigned int amt);
	int getHealth() const { return m_hp; }
	virtual bool canPickThingsUp() const { return true; }
private:
	bool closeToBoulder();
	int m_hp;
};

class Boulder : public Actor
{
public:
	Boulder(StudentWorld* world, int x, int y) :
		Actor(IID_BOULDER, x, y, down, 1, 1, world)
	{
		m_state = stable;
		m_timeWaiting = 0;
		setVisible(true);
	}
	virtual ~Boulder() { }
	virtual bool canActorsPassThroughMe() const { return false; }
	virtual void doSomething();//IMPLEMENT
private:
	enum { stable, waiting, falling };
	int m_state;
	short m_timeWaiting;
};

class Frackman : public Human
{
public:
	Frackman(StudentWorld* w) :
		Human(IID_PLAYER, PLAYER_STARTX, PLAYER_STARTY, right, 10, w)
	{
		m_gold = 0;
		m_sonar = 1;
		m_squirt = 5;
	}
	virtual ~Frackman() 
	{ }
	virtual void doSomething();
	int getGold() const { return m_gold; }
	int getSonar() const { return m_sonar; }
	int getSquirt() const { return m_squirt; }
	void giveSquirt();
	void giveSonar();
	void giveGold();
	void placeGold();
	void makeSquirt();
private:
	int m_gold;
	int m_sonar;
	int m_squirt;
};

class Protest: public Human
{
public:
	virtual bool huntsFrackman() const { return true; }
	virtual void getBribed() = 0;
private:

};

class RegProtest : public Protest
{
public:
	virtual void getBribed();
private:

};

class HardCoreProtest : public Protest
{
public:
	virtual void getBribed();
private:

};

class Dirt : public Actor
{
public:
	Dirt( int startX, int startY, StudentWorld* world) :
		Actor(IID_DIRT, startX, startY, right, 0.25, 3, world)
	{
		setVisible(true);
	}
	virtual ~Dirt() {}
	virtual bool canActorsPassThroughMe() { return false; }
	virtual bool isOverlappingArea(int x, int y) const;
private:

};

class Squirt : public Actor
{
public:

private:

};

class Pickup : public Actor
{
public:
	Pickup(int ID, int x, int y, bool found, int state, int pickerUpper, StudentWorld* world) : Actor(ID, x, y, right, 1, 2, world),
		m_found(found), m_state(state), m_pickupableBy(pickerUpper)
	{

		if (found || pickerUpper == protester)
			setVisible(true);
		m_timeInTemp = 0;
	}
	~Pickup() {}
	virtual void doSomething() {}
	virtual bool canBePickedUpByFrackman() const { return true; }
	bool isFound() const { return m_found; }
	void setFound() { m_found = true; }
	virtual bool isTooOld();
	virtual void incTimeInTemp() { m_timeInTemp++; }
	int getAge() const { return m_timeInTemp; }
	int getPickerUpper() const { return m_pickupableBy; }
	enum { temporary, permanent };
	enum { frackman, protester };
private:
	bool m_found;
	int m_state;
	int m_pickupableBy;
	unsigned int m_timeInTemp;
	
};

class Gold : public Pickup
{
public:
	Gold(StudentWorld* world, int x, int y, int pickerUpper) : Pickup(IID_GOLD, x, y, false, permanent, pickerUpper, world) {}
	virtual ~Gold() {}
	virtual void doSomething();
	virtual bool isTooOld();
	virtual bool canBePickedUpByFrackman() const { return (getPickerUpper() == frackman); }
private:
};

class Sonar : public Pickup
{
public:
	Sonar(StudentWorld* world) : Pickup(IID_SONAR, 0, 60, true, temporary, frackman, world) {}
	virtual ~Sonar() {}
	virtual void doSomething();
private:
};

class WaterRefill : public Pickup
{
public:
	WaterRefill(StudentWorld* world, int x, int y) : Pickup(IID_WATER_POOL, x, y, true, temporary, frackman, world)
	{}
	virtual ~WaterRefill() {}
	virtual void doSomething();
private:
};

class OilBarrel : public Pickup
{
public:
	OilBarrel(StudentWorld* world, int x, int y) : Pickup(IID_BARREL, x, y, false, permanent, frackman, world) {}
	~OilBarrel() {}
	virtual void doSomething(); 
	virtual bool neededToFinishLevel() const { return true; }
private:
};
#endif // ACTOR_H_
