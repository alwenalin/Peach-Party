#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

// return true if the player can move in the direction it's facing (there are squares)
bool movableActor::moveForward()
{
	if ((getMoveDir() == RIGHT_DIR && world()->isValidPos(getX() + BOARD_WIDTH, getY()))
		|| (getMoveDir() == UP_DIR && world()->isValidPos(getX(), getY() + BOARD_HEIGHT))
		|| (getMoveDir() == LEFT_DIR && world()->isValidPos(getX() - BOARD_WIDTH, getY()))
		|| (getMoveDir() == DOWN_DIR && world()->isValidPos(getX(), getY() - BOARD_HEIGHT)))
	{
		return true;
	}
	return false;
}

// returns the # of possible valid directions. If > 2, it's a fork
int movableActor::forkCounter()
{
	int counter = 0;
	if (world()->isValidPos(getX() + BOARD_WIDTH, getY()))
	{
		counter++;
	}
	if (world()->isValidPos(getX(), getY() + BOARD_HEIGHT))
	{
		counter++;
	}
	if (world()->isValidPos(getX() - BOARD_WIDTH, getY()))
	{
		counter++;
	}
	if (world()->isValidPos(getX(), getY() - BOARD_HEIGHT))
	{
		counter++;
	}

	return counter;
}

// moves the actor 2 pixels in the direction every tick
void movableActor::moveTwoTicks()
{
	if (getMoveDir() == RIGHT_DIR)
	{
		moveTo(getX() + 2, getY());
	}
	else if (getMoveDir() == LEFT_DIR)
	{
		moveTo(getX() - 2, getY());
	}
	else if (getMoveDir() == UP_DIR)
	{
		moveTo(getX(), getY() + 2);
	}
	else if (getMoveDir() == DOWN_DIR)
	{
		moveTo(getX(), getY() - 2);
	}
}

// when an actor can't move forward, choose a perpendicular direction
void movableActor::movePerp()
{
	// prefers moving up over down 
	if ((getMoveDir() == RIGHT_DIR) || (getMoveDir() == LEFT_DIR))
	{
		if (world()->isValidPos(getX(), getY() + BOARD_HEIGHT))
		{
			setMoveDir(UP_DIR);
		}
		else if (world()->isValidPos(getX(), getY() - BOARD_HEIGHT))
		{
			setMoveDir(DOWN_DIR);
		}
	}
	// prefers moving right over left 
	else if ((getMoveDir() == UP_DIR) || (getMoveDir() == DOWN_DIR))
	{
		if (world()->isValidPos(getX() + BOARD_WIDTH, getY()))
		{
			setMoveDir(RIGHT_DIR);
		}
		else if (world()->isValidPos(getX() - BOARD_WIDTH, getY()))
		{
			setMoveDir(LEFT_DIR);
		}
	}

	// if moving left, player's image is left. otherwise it's right
	if (getMoveDir() == LEFT_DIR)
	{
		setDirection(LEFT_DIR);
	}
	else
	{
		setDirection(RIGHT_DIR);
	}
}

// let's the player do something every tick
void Player::doSomething()
{
	// if the player is waiting to roll 
	if (getWaiting())
	{	
		int action1 = world()->getAction(m_playerNum);
		if (action1 != ACTION_NONE)
		{
			if (action1 == ACTION_ROLL)
			{
				int dieRoll = randInt(1, 10);
				changeTicks(dieRoll * 8);
				changeWaiting(false);
				changeDidEvent(false);
			}
			else if (action1 == ACTION_FIRE)
			{
				if (getVor()) // has a vortex to fire
				{
					world()->addVortex(getX(), getY(), getMoveDir());
					changeVor(false); // used up the 1 vortex
					world()->playSound(SOUND_PLAYER_FIRE);
				}
			}
			else
			{
				return;
			}
		}
	}

	// if the player is walking (not waiting to roll)
	if (!getWaiting())
	{
		changeDidBoo(false);
		changeDidBowser(false);
		if (m_isDirectional == true)
		{
			if (m_directionalDir == RIGHT_DIR)
			{
				setMoveDir(RIGHT_DIR);
			}
			else if (m_directionalDir == UP_DIR)
			{
				setMoveDir(UP_DIR);
			}
			else if (m_directionalDir == LEFT_DIR)
			{
				setMoveDir(LEFT_DIR);
			}
			else if (m_directionalDir == DOWN_DIR)
			{
				setMoveDir(DOWN_DIR);
			}

			if (getMoveDir() == LEFT_DIR)
			{
				setDirection(LEFT_DIR);
			}
			else
			{
				setDirection(RIGHT_DIR);
			}

			if (!getWaiting())
			{
				m_isDirectional = false;
			}
		}
		else if (forkCounter() > 2) // if player is at a fork
		{
			int action2 = world()->getAction(m_playerNum);
			if ((action2 == ACTION_LEFT) || (action2 == ACTION_RIGHT)
				|| (action2 == ACTION_UP) || (action2 == ACTION_DOWN))
			{
				// player can't try to move backwards
				if (((getMoveDir() == RIGHT_DIR) && (action2 == ACTION_LEFT)) || ((getMoveDir() == UP_DIR) && (action2 == ACTION_DOWN))
					|| ((getMoveDir() == LEFT_DIR) && (action2 == ACTION_RIGHT)) || ((getMoveDir() == DOWN_DIR) && (action2 == ACTION_UP)))
				{
					return;
				}

				// player can't try to move in a nonvalid position on the board (no squares)
				if ((action2 == ACTION_UP) && !(world()->isValidPos(getX(), getY() + BOARD_HEIGHT))
					|| (action2 == ACTION_DOWN) && !(world()->isValidPos(getX(), getY() - BOARD_HEIGHT))
					|| (action2 == ACTION_LEFT) && !(world()->isValidPos(getX() - BOARD_WIDTH, getY()))
					|| (action2 == ACTION_RIGHT) && !(world()->isValidPos(getX() + BOARD_WIDTH, getY())))
				{
					return;
				}

				// changes player direction to the direction they want to go in the fork
				if ((action2 == ACTION_LEFT) && (world()->isValidPos(getX() - BOARD_WIDTH, getY())))
				{
					setMoveDir(LEFT_DIR);
				}
				else if ((action2 == ACTION_RIGHT) && (world()->isValidPos(getX() + BOARD_WIDTH, getY())))
				{
					setMoveDir(RIGHT_DIR);
				}
				else if ((action2 == ACTION_UP) && (world()->isValidPos(getX(), getY() + BOARD_HEIGHT)))
				{
					setMoveDir(UP_DIR);
				}
				else if ((action2 == ACTION_DOWN) && (world()->isValidPos(getX(), getY() - BOARD_HEIGHT)))
				{
					setMoveDir(DOWN_DIR);
				}

				// if moving left, player's image is left. otherwise it's right
				if (getMoveDir() == LEFT_DIR)
				{
					setDirection(LEFT_DIR);
				}
				else
				{
					setDirection(RIGHT_DIR);
				}
			}
			else // player didn't press a direcitonal key
			{
				return;
			}
		}
		else if (!moveForward()) // if player can't move forward
		{
			movePerp(); // move perpendicularly (corner)
		}

		moveTwoTicks();

		changeTicks(-1);

		if (getTicks() == 0)
		{
			changeWaiting(true);
		}
	}
}

// returns a random valid direction to move in
int Baddie::randDirection()
{
	bool valid = false;
	int dir = 0;

	// keeps randomly picking a direction until it's valid
	while (valid == false)
	{
		int moveDir = randInt(1, 4);
		if (moveDir == 1)
		{
			dir = RIGHT_DIR;
		}
		else if (moveDir == 2)
		{
			dir = UP_DIR;
		}
		else if (moveDir == 3)
		{
			dir = LEFT_DIR;
		}
		else if (moveDir == 4)
		{
			dir = DOWN_DIR;
		}

		if (((dir == RIGHT_DIR) && world()->isValidPos(getX() + BOARD_WIDTH, getY()))
			|| ((dir == UP_DIR) && world()->isValidPos(getX(), getY() + BOARD_HEIGHT))
			|| ((dir == LEFT_DIR) && world()->isValidPos(getX() - BOARD_WIDTH, getY()))
			|| ((dir == DOWN_DIR) && world()->isValidPos(getX(), getY() - BOARD_HEIGHT)))
		{
			valid = true;
			if (dir == LEFT_DIR)
			{
				setDirection(LEFT_DIR);
			}
			else
			{
				setDirection(RIGHT_DIR);
			}
		}
	}
	return dir;
}

// teleport for impacted baddies
void Baddie::teleport()
{
	int x;
	int y;
	world()->randomSquare(x, y);
	moveTo(x, y);

	setMoveDir(RIGHT_DIR);
	setDirection(RIGHT_DIR);
	changePaused(true);
	setCounter(180);

	changeTeleport(false);
}

void Bowser::doSomething()
{
	if (getTeleport())
	{
		teleport();
	}
	
	if (getPaused()) // if bowser is paused
	{
		// checks for both players (peach is 1 and yoshi is 2)
		for (int i = 2; i > 0; i--)
		{
			// if bowser and player lands on the same square
			if ((getX() == world()->playerPtr(i)->getX()) && (getY() == world()->playerPtr(i)->getY())
				&& (world()->playerPtr(i)->getWaiting()) && (world()->playerPtr(i)->getDidBowser() == false))
			{
				int option = randInt(1, 2);
				if (option == 1) // 50% chance of losing all coins and stars
				{
					world()->playerPtr(i)->setCoins(0);
					world()->playerPtr(i)->setStars(0);
					world()->playSound(SOUND_BOWSER_ACTIVATE);
				}
				world()->playerPtr(i)->changeDidBowser(true);
			}
		}
		changeCounter(-1);

		if (getCounter() == 0)
		{
			int squares_to_move = randInt(1, 10);
			setTicks(squares_to_move * 8);
			setMoveDir(randDirection());
			changePaused(false);
		}
	}

	if (!getPaused())
	{
		if (forkCounter() > 2) // if bowser is at a fork
		{
			setMoveDir(randDirection()); // move in a random direction
		}
		else if (!moveForward()) // if bowser can't move forward
		{
			movePerp(); // move perpendicularly (corner)
		}

		moveTwoTicks();

		changeTicks(-1);

		if (getTicks() == 0)
		{
			changePaused(true);
			setCounter(180);

			int drop = randInt(1, 4);
			if (drop == 1) // 25% chance to drop a dropping square
			{
				world()->dropSquare(getX(), getY());
				world()->playSound(SOUND_DROPPING_SQUARE_CREATED);
			}
		}
	}
}

void Boo::doSomething()
{
	if (getTeleport())
	{
		teleport();
	}
	
	int other;
	if (getPaused()) // if boo is paused
	{
		// checks for both players (peach is 1 and yoshi is 2)
		for (int i = 2; i > 0; i--)
		{
			if (i == 1)
			{
				other = 2;
			}
			else
			{
				other = 1;
			}
			
			// if boo and player lands on the same square
			if ((getX() == world()->playerPtr(i)->getX()) && (getY() == world()->playerPtr(i)->getY())
				&& (world()->playerPtr(i)->getWaiting()) && (world()->playerPtr(i)->getDidBoo() == false))
			{
				int option = randInt(1, 2); // 50% chance
				if (option == 1) // swap coins
				{
					int tempCoins = world()->playerPtr(i)->getCoins();
					world()->playerPtr(i)->setCoins(world()->playerPtr(other)->getCoins());
					world()->playerPtr(other)->setCoins(tempCoins);
				}
				else if (option == 2) // swap stars
				{
					int tempStars = world()->playerPtr(i)->getStars();
					world()->playerPtr(i)->setStars(world()->playerPtr(other)->getStars());
					world()->playerPtr(other)->setStars(tempStars);
				}

				world()->playerPtr(i)->changeDidBoo(true);
				world()->playSound(SOUND_BOO_ACTIVATE);
			}
		}
		changeCounter(-1);

		if (getCounter() == 0)
		{
			int squares_to_move = randInt(1, 3);
			setTicks(squares_to_move * 8);
			setMoveDir(randDirection());
			changePaused(false);
		}
	}

	if (!getPaused())
	{
		if (forkCounter() > 2) // if boo is at a fork
		{
			setMoveDir(randDirection()); // move in a random direction
		}
		else if (!moveForward()) // if boo can't move forward
		{
			movePerp(); // move perpendicularly (corner)
		}

		moveTwoTicks();

		changeTicks(-1);

		if (getTicks() == 0)
		{
			changePaused(true);
			setCounter(180);
		}
	}
}

void Vortex::doSomething()
{
	if (!getActive())
	{
		return;
	}
	
	setMoveDir(m_moveDir);
	moveTwoTicks();

	// if vortex goes out of game bounds, it's inactive (gets deleted)
	if (getX() < 0 || getX() >= VIEW_WIDTH || getY() < 0 || getY() >= VIEW_HEIGHT)
	{
		changeActive(false);
	}

	Baddie* badPtr;
	if (world()->overlapVor(getX(), getY(), badPtr))
	{
		badPtr->changeTeleport(true);
		changeActive(false);
		world()->playSound(SOUND_HIT_BY_VORTEX);
	}
}

// returns an int to represent which player(s) is on the square 
int Square::onSquare()
{
	// if no players are on the square, players = 0
	int players = 0;

	// if only peach is on the square, players = 1
	if ((world()->playerPtr(1)->getX() == this->getX()) &&
		(world()->playerPtr(1)->getY() == this->getY()))
	{
		players = players + 1;
	}
	// if only yoshi is on the square, players = 2
	// if both yoshi and peach are on the square, players = 3
	if ((world()->playerPtr(2)->getX() == this->getX()) &&
		(world()->playerPtr(2)->getY() == this->getY()))
	{
		players = players + 2;
	}

	return players;
}

// returns state of specific player (newplayer is false, not newplayer is true)
bool Square::playerState(int playerNum)
{
	if (playerNum == 1)
	{
		return peachHere;
	}
	else // if (playerNum == 2)
	{
		return yoshiHere;
	}
}

// changes player to true if it's a new player on a square
// changes player to false if it's not a new player, it's lingering 
void Square::newPlayer(int players)
{
	if (players == 0)
	{
		peachHere = true;
		yoshiHere = true;
	}
	else if (players == 1)
	{
		peachHere = false;
		yoshiHere = true;
	}
	else if (players == 2)
	{
		peachHere = true;
		yoshiHere = false;
	}
	else if (players == 3)
	{
		peachHere = false;
		yoshiHere = false;
	}
}

void CoinSquare::doSomething(int x)
{
	int players = Square::onSquare();
	int i = players;

	if (players == 3)
	{
		i--;
	}

	for (; i > 0; i--)
	{
		// if player landed on the coin square
		if ((world()->playerPtr(i)->getWaiting()) && (Square::playerState(i)))
		{
			if (x == -3) // square is red
			{
				// coins can never be negative so -3 for anything < 3 will be 0
				if (world()->playerPtr(i)->getCoins() >= 3)
				{
					world()->playerPtr(i)->changeCoins(x);
				}
				else
				{
					world()->playerPtr(i)->setCoins(0);
				}
				world()->playSound(SOUND_TAKE_COIN);
			}
			else // square is blue
			{
				world()->playerPtr(i)->changeCoins(x);
				world()->playSound(SOUND_GIVE_COIN);
			}
		}
		
		if (players == 2)
		{
			break;
		}
	}

	Square::newPlayer(players);
}

void BlueCoinSquare::doSomething()
{
	CoinSquare::doSomething(3);
}

void RedCoinSquare::doSomething()
{
	CoinSquare::doSomething(-3);
}

void StarSquare::doSomething()
{
	int players = Square::onSquare();
	int i = players;

	if (players == 3)
	{
		i--;
	}

	for (; i > 0; i--)
	{
		// if player landed or moved onto the star square
		if ((world()->playerPtr(i)->getWaiting() && Square::playerState(i))
			|| world()->playerPtr(i)->getWaiting() == false)
		{
			if (world()->playerPtr(i)->getCoins() < 20)
			{
				return;
			}
			else // if player can exchange 20 coins for a star
			{
				world()->playerPtr(i)->changeCoins(-20);
				world()->playerPtr(i)->changeStars(1);
				world()->playSound(SOUND_GIVE_STAR);
			}
		}

		if (players == 2)
		{
			break;
		}
	}

	Square::newPlayer(players);
}

void EventSquare::doSomething()
{
	int players = Square::onSquare();
	int option = randInt(1, 3);
	int other;
	int i = players;

	if (players == 3)
	{
		i--;
	}

	for (; i > 0; i--)
	{
		// if player landed on the event square
		if ((world()->playerPtr(i)->getWaiting()) && (Square::playerState(i)) && (world()->playerPtr(i)->getDidEvent() == false))
		{
			if (i == 1)
			{
				other = 2;
			}
			else
			{
				other = 1;
			}

			if (option == 1) // teleports player to random square
			{
				int teleportX;
				int teleportY;
				world()->randomSquare(teleportX, teleportY);
				world()->playerPtr(i)->moveTo(teleportX, teleportY);
				world()->playSound(SOUND_PLAYER_TELEPORT);
			}
			else if (option == 2) // swap players
			{
				// swap x y coordinates
				int tempX = world()->playerPtr(i)->getX(); 
				int tempY = world()->playerPtr(i)->getY();
				world()->playerPtr(i)->moveTo(world()->playerPtr(other)->getX(), world()->playerPtr(other)->getY());
				world()->playerPtr(other)->moveTo(tempX, tempY);

				// swap ticks to move
				int tempTicks = world()->playerPtr(i)->getTicks(); 
				world()->playerPtr(i)->setTicks(world()->playerPtr(other)->getTicks());
				world()->playerPtr(other)->setTicks(tempTicks); 

				// swap walk direction
				int tempMoveDir = world()->playerPtr(i)->getMoveDir();
				world()->playerPtr(i)->setMoveDir(world()->playerPtr(other)->getMoveDir()); 
				world()->playerPtr(other)->setMoveDir(tempMoveDir);

				// swap sprite direction
				int tempSpriteDir = world()->playerPtr(i)->getDirection();
				world()->playerPtr(i)->setDirection(world()->playerPtr(other)->getDirection()); 
				world()->playerPtr(other)->setDirection(tempSpriteDir);

				// swap waiting/walking state
				int tempState = world()->playerPtr(i)->getWaiting(); 
				world()->playerPtr(i)->changeWaiting(world()->playerPtr(other)->getWaiting());
				world()->playerPtr(other)->changeWaiting(tempState);

				world()->playSound(SOUND_PLAYER_TELEPORT);
			}
			else if (option == 3) // vortex
			{
				// gives player a vortex if they didn't already have one
				world()->playerPtr(i)->changeVor(true);
				world()->playSound(SOUND_GIVE_VORTEX); // plays sound either way
			}

			world()->playerPtr(other)->changeDidEvent(true);
			world()->playerPtr(i)->changeDidEvent(true);
		}

		if (players == 2)
		{
			break;
		}
	}

	Square::newPlayer(players);
}

void BankSquare::doSomething()
{
	int players = Square::onSquare();
	int i = players;

	if (players == 3)
	{
		i--;
	}

	for (; i > 0; i--)
	{
		// if player landed on the bank square
		if (world()->playerPtr(i)->getWaiting() && Square::playerState(i))
		{
			world()->playerPtr(i)->changeCoins(world()->getBank());
			world()->changeBank(-(world()->getBank())); // sets bank to 0
			world()->playSound(SOUND_WITHDRAW_BANK);
		}
		// if player passed over the bank square
		else if (world()->playerPtr(i)->getWaiting() == false)
		{
			if (world()->playerPtr(i)->getCoins() < 5) // if player has less than 5 coins
			{
				world()->changeBank(world()->playerPtr(i)->getCoins()); // add all coins to the bank
				world()->playerPtr(i)->changeCoins(-(world()->playerPtr(i)->getCoins())); // sets player's coins to 0
			}
			else // takes 5 coins from player and add it to bank
			{
				world()->changeBank(5);
				world()->playerPtr(i)->changeCoins(-5);
			}
			world()->playSound(SOUND_DEPOSIT_BANK);
		}

		if (players == 2)
		{
			break;
		}
	}

	Square::newPlayer(players);
}

void DirectionalSquare::doSomething()
{
	int players = Square::onSquare();
	int i = players;

	if (players == 3)
	{
		i--;
	}

	for (; i > 0; i--)
	{
		// if player is ever on/passed a directional square
		world()->playerPtr(i)->setDirectional(m_startDir);
		world()->playerPtr(i)->isDirectional(true);

		if (players == 2)
		{
			break;
		}
	}
}

void DroppingSquare::doSomething()
{
	int players = Square::onSquare();
	int i = players;

	if (players == 3)
	{
		i--;
	}

	for (; i > 0; i--)
	{
		// if player landed on the dropping square
		if ((world()->playerPtr(i)->getWaiting()) && (Square::playerState(i)))
		{
			int option = randInt(1, 2); // 50% chance
			if (option == 1) // lost 10 coins 
			{
				// if player has less than 10 coins, set to 0
				if (world()->playerPtr(i)->getCoins() < 10)
				{
					world()->playerPtr(i)->setCoins(0); 
				}
				else
				{
					world()->playerPtr(i)->changeCoins(-10);
				}
			}
			else if (option == 2) // lose 1 star 
			{
				// if player has no stars, it stays as 0
				if (world()->playerPtr(i)->getStars() > 0)
				{
					world()->playerPtr(i)->changeStars(-1);
				}
			}
			world()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
		}

		if (players == 2)
		{
			break;
		}
	}

	Square::newPlayer(players);
}