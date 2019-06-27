#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

int randX()//int Actor::randX()
{
	int pos;

	pos = rand() % (VIEW_WIDTH - SPRITE_WIDTH + 1 - 7);

	//MAX POS
	//pos = VIEW_WIDTH - SPRITE_WIDTH;

	//Max left side
	//pos = 30 - SPRITE_WIDTH;

	if (pos > 30 - SPRITE_WIDTH)
		pos += 7;
	return pos;
}

int randY()//int Actor::randY()
{
	int pos;

	pos = rand() % (VIEW_HEIGHT - 4 - SPRITE_HEIGHT + 1);
	
	//MAX POS
	//pos = VIEW_HEIGHT - SPRITE_HEIGHT;

	return pos;

}

bool Actor::isOverlappingArea(int x, int y) const
{
	for (int i = getX() - 3; i < getX() + 4; i++)
		for (int j = getY() -3; j < getY() + 4; j++)
			if (x == i && y == j)
				return true;

	return false;
}

bool Pickup::isTooOld()
{
	return (m_timeInTemp >= max(100U, 300 - 10 * getWorld()->getLevel()));
}

bool Human::annoy(unsigned int amt)
{
	m_hp -= amt;
	
	if (m_hp <= 0)
		setDead();

	return true;
}

bool Dirt::isOverlappingArea(int x, int y) const
{
	if (!isAlive())
		return false;

	if(getX() == x && getY() == y)
		return true;

	return false;
}

///////////////////////////////////////////////////////////////////
// _____ ____      _    ____ _  ____  __    _    _   _
//|  ___|  _ \    / \  / ___| |/ /  \/  |  / \  | \ | |
//| |_  | |_) |  / _ \| |   | ' /| |\/| | / _ \ |  \| |
//|  _| |  _ <  / ___ \ |___| . \| |  | |/ ___ \| |\  |
//|_|   |_| \_\/_/   \_\____|_|\_\_|  |_/_/   \_\_| \_|
//
///////////////////////////////////////////////////////////////////
void Frackman::giveSquirt()
{
	m_squirt += 5;
}

void Frackman::giveSonar()
{
	m_sonar++;
}

void Frackman::giveGold()
{
	m_gold++;
}

void Frackman::doSomething()
{
	if (isAlive() == false)
		return;

	//if (Frackman  is overlapping dirt)
	//		remove the dirt

	int ch;
	if (getWorld()->getKey(ch) == true)
	{
		// user hit a key this tick!
		switch (ch)
		{
		case KEY_PRESS_ESCAPE:
			annoy(100);
			break;
		case KEY_PRESS_LEFT:
			if (getDirection() != left)
			{
				setDirection(left);
				break;
			}
			if (getWorld()->closeToBoulder(getX() - 1, getY()))
				break;
			if (getX() == 0)
			{
				moveTo(getX(), getY());
				break;
			}
			moveTo(getX() - 1, getY());
			if (getWorld()->deleteDirtRow(getX(), getY(), true))
				getWorld()->playSound(SOUND_DIG);
			break;
		case KEY_PRESS_RIGHT:
			if (getDirection() != right)
			{
				setDirection(right);
				break;
			}
			if (getWorld()->closeToBoulder(getX() + 1, getY()))
				break;
			if (getX() == 60)
			{
				moveTo(getX(), getY());
				break;
			}
			moveTo(getX() + 1, getY());
			if (getWorld()->deleteDirtRow(getX() + 3, getY(), true))
				getWorld()->playSound(SOUND_DIG);
			break;
		case KEY_PRESS_UP:
			if (getDirection() != up)
			{
				setDirection(up);
				break;
			}
			if (getWorld()->closeToBoulder(getX(), getY() + 1))
				break;
			if (getY() == 60)
			{
				moveTo(getX(), getY());
				break;
			}
			moveTo(getX(), getY() + 1);
			if (getWorld()->deleteDirtRow(getX(), getY() + 3, false))
				getWorld()->playSound(SOUND_DIG);
			break;
		case KEY_PRESS_DOWN:
			if (getDirection() != down)
			{
				setDirection(down);
				break;
			}
			if (getWorld()->closeToBoulder(getX(), getY() - 1))
				break;
			if (getY() == 0)
			{
				moveTo(getX(), getY());
				break;
			}
			moveTo(getX(), getY() - 1);
			if (getWorld()->deleteDirtRow(getX(), getY(), false))
				getWorld()->playSound(SOUND_DIG);
		case KEY_PRESS_SPACE:
			//Add squirt in front of player
			;
			break;
		case KEY_PRESS_TAB:
			if (m_gold > 0)
				placeGold();
			
			break;
		case 'z':
		case 'Z':
			if (m_sonar > 0)
			{
				m_sonar--;
				getWorld()->playSound(SOUND_SONAR);
				getWorld()->revealAllNearbyObjects(getX(), getY(), 12);
			}
			break;
			// etc…
		}
	}


}

void Frackman::placeGold()
{
	Gold* newGold = new Gold(getWorld(), getX(), getY(), 1); //1: Protester can pickup
	getWorld()->addActor(newGold);
}

///////////////////////////////////////////////////////////////////
// ____   ___  _   _ _     ____  _____ ____ 
//| __ ) / _ \| | | | |   |  _ \| ____|  _ \
//|  _ \| | | | | | | |   | | | |  _| | |_) |
//| |_) | |_| | |_| | |___| |_| | |___|  _ <
//|____/ \___/ \___/|_____|____/|_____|_| \_\
//
///////////////////////////////////////////////////////////////////

void Boulder::doSomething()
{
	if (!isAlive())
	{
		return;
	}

	if (m_state == stable && getWorld()->canActorMoveTo(this, getX(), getY() - 1))
	{
		m_state = waiting;
	}
	else if (m_state == waiting && m_timeWaiting < 30)
	{
		m_timeWaiting++;
	}
	else if (m_state == waiting && m_timeWaiting == 30)
	{
		getWorld()->playSound(SOUND_FALLING_ROCK);
		m_state = falling;
	}
	else if (m_state == falling)
	{
		if (getWorld()->canActorMoveTo(this, getX(), getY() - 1))
		{
			this->moveTo(getX(), getY() - 1);
			getWorld()->annoyAllNearbyActors(this, 100, 3);
		}
		else
			setDead();	
	}
}


///////////////////////////////////////////////////////////////////
//    ___ ___ _
//   / _ \_ _| |
//  | | | | || |
//  | |_| | || |___
//   \___/___|_____|
//
///////////////////////////////////////////////////////////////////

void OilBarrel::doSomething()
{
	if (!isAlive())
		return;
	if (!isFound() && getWorld()->isNearFrackMan(this, 4))
	{
		setVisible(true);
		setFound();
		return;
		//add function to change found variable
	}
	//Figure out how to determine if visible without using isVisible();
	//can i make private int m_found? (copy of visible, basically)

	if (getWorld()->isNearFrackMan(this, 3))
	{
		setDead();
		getWorld()->playSound(SOUND_FOUND_OIL);
		getWorld()->increaseScore(1000);
		getWorld()->decOilLeft();
	}
	
}

///////////////////////////////////////////////////////////////////
//  __        ___  _____ _____ ____  
//  \ \      / / \|_   _| ____|  _ \
//   \ \ /\ / / _ \ | | |  _| | |_) |
//    \ V  V / ___ \| | | |___|  _ <
//     \_/\_/_/   \_\_| |_____|_| \_\
//
///////////////////////////////////////////////////////////////////

void WaterRefill::doSomething()
{
	if (!isAlive())
		return;

	if (getWorld()->isNearFrackMan(this, 3))
	{
		setDead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->giveFrackManWater();
		getWorld()->increaseScore(100);
	}
	else
	{
		if (isTooOld())
			setDead();
		else
			incTimeInTemp();
	}

}



///////////////////////////////////////////////////////////////////
//  ____   ___  _   _    _    ____ 
// / ___| / _ \| \ | |  / \  |  _ \
// \___ \| | | |  \| | / _ \ | |_) |
//  ___) | |_| | |\  |/ ___ \|  _ <
// |____/ \___/|_| \_/_/   \_\_| \_\
//
///////////////////////////////////////////////////////////////////

void Sonar::doSomething()
{
	if (!isAlive())
		return;

	if (getWorld()->isNearFrackMan(this, 3))
	{
		setDead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->giveFrackManSonar();
		getWorld()->increaseScore(75);
	}
	else
	{
		if (isTooOld())
			setDead();
		else
			incTimeInTemp();
	}
}




///////////////////////////////////////////////////////////////////
//    ____  ___  _     ____
//   / ___|/ _ \| |   |  _ \
//  | |  _| | | | |   | | | |
//  | |_| | |_| | |___| |_| |
//   \____|\___/|_____|____/
//
///////////////////////////////////////////////////////////////////

bool Gold::isTooOld()
{
	return (getAge() > 100);
}

void Gold::doSomething()
{
	if (!isAlive())
		return;
	if (canBePickedUpByFrackman())
	{
		if (!isFound() && getWorld()->isNearFrackMan(this, 4))
		{
			setVisible(true);
			setFound();
			return;
		}

		if (getWorld()->isNearFrackMan(this, 3))
		{
			setDead();
			getWorld()->playSound(SOUND_GOT_GOODIE);
			getWorld()->increaseScore(10);
			getWorld()->giveFrackManGold();
		}
	}
	else
	{
		Protest* pickerUpper = (Protest*) getWorld()->findNearbyPickerUpper(this, 3);
		if (pickerUpper != nullptr)
		{
			setDead();
			getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
			pickerUpper->getBribed();
			getWorld()->increaseScore(25);
		}
		else
		{
			if (isTooOld())
				setDead();
			else
				incTimeInTemp();
		}
	}
}