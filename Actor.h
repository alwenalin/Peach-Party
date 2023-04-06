#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject
{
public:
	Actor(StudentWorld* world, int imageID, int startX, int startY, int startDirection, int depth)
		: GraphObject(imageID, SPRITE_WIDTH * startX, SPRITE_HEIGHT * startY, startDirection, depth, 1), 
		m_world(world), direction(startDirection) {};
	virtual void doSomething() = 0;
	StudentWorld* world() { return m_world; };
private:
	StudentWorld* m_world;
	int direction;
};

class movableActor : public Actor
{
public:
	movableActor(StudentWorld* world, int imageID, int startX, int startY, int depth)
		: Actor(world, imageID, startX, startY, 0, depth), m_direction(0),
		ticks_to_move(0) {};
	virtual void doSomething() = 0;
	void moveTwoTicks();
	void movePerp();
	int forkCounter();
	bool moveForward();

	int getMoveDir() { return m_direction; };
	int setMoveDir(int d) { return m_direction = d; };
	int getTicks() { return ticks_to_move; };
	int changeTicks(int t) { return ticks_to_move = ticks_to_move + t; };
	int setTicks(int x) { return ticks_to_move = x; };
private:
	int ticks_to_move;
	int m_direction;
};

class Player : public movableActor
{
public:
	Player(StudentWorld* world, int playerNum, int imageID, int startX, int startY)
		: movableActor(world, imageID, startX, startY, 0), m_waiting(true),
		m_playerNum(playerNum), m_stars(0), m_coins(0), m_isDirectional(false),
		m_directionalDir(0), m_vortex(false), m_didEvent(false), m_didBoo(false),
		m_didBowser(false) {};
	virtual void doSomething();
	
	bool isDirectional(bool x) { return m_isDirectional = x; };
	int setDirectional(int dir) { return m_directionalDir = dir; }
	int dieRoll() { return (getTicks() + 7) / 8; };
	
	bool getWaiting() { return m_waiting; };
	bool changeWaiting(bool x) { return m_waiting = x; };
	bool getDidEvent() { return m_didEvent; };
	bool changeDidEvent(bool x) { return m_didEvent = x; }; 
	bool getDidBoo() { return m_didBoo; };
	bool changeDidBoo(bool x) { return m_didBoo = x; };
	bool getDidBowser() { return m_didBowser; };
	bool changeDidBowser(bool x) { return m_didBowser = x; };
	bool getVor() { return m_vortex; };
	bool changeVor(bool x) { return m_vortex = x; };
	int getCoins() { return m_coins; };
	int changeCoins(int x) { return m_coins = getCoins() + x; };
	int setCoins(int x) { return m_coins = x; };
	int getStars() { return m_stars; };
	int changeStars(int x) { return m_stars = getStars() + x; };
	int setStars(int x) { return m_stars = x; };
private:
	bool m_waiting; // true if waiting, false if walking
	int m_playerNum;
	int m_stars;
	int m_coins;
	bool m_isDirectional;
	int m_directionalDir;  
	bool m_vortex;
	bool m_didEvent;
	bool m_didBoo;
	bool m_didBowser;
};

class Baddie : public movableActor
{
public:
	Baddie(StudentWorld* world, int imageID, int startX, int startY)
		: movableActor(world, imageID, startX, startY, 1), m_travel(0), m_paused(true),
		m_counter(180), m_teleport(false) {};
	virtual void doSomething() = 0; 
	int randDirection(); 
	void teleport();

	int getTravel() { return m_travel; }; 
	int changeTravel(int x) { return m_travel + x; };
	bool getPaused() { return m_paused; };
	bool changePaused(bool x) { return m_paused = x; }
	int getCounter() { return m_counter; };
	int changeCounter(int x) { return m_counter = m_counter + x; };
	int setCounter(int x) { return m_counter = x; };
	bool getTeleport() { return m_teleport; };
	bool changeTeleport(bool x) { return m_teleport = x; };
private: 
	int m_travel;
	bool m_paused; // true if paused, false if walking
	int m_counter; 
	bool m_teleport; // true if impacacted, false if not
};

class Vortex : public movableActor
{
public:
	Vortex(StudentWorld* world, int imageID, int startX, int startY, int moveDir)
		: movableActor(world, imageID, startX, startY, 1), m_moveDir(moveDir), m_fireDir(0), 
		m_active(true) {};
	virtual void doSomething();
	bool getActive() { return m_active; };
	bool changeActive(bool x) { return m_active = x; };
private:
	int m_moveDir;
	int m_fireDir;
	bool m_active; // true if active, false if inactive
};

class Bowser : public Baddie
{
public: 
	Bowser(StudentWorld* world, int imageID, int startX, int startY)
		: Baddie(world, imageID, startX, startY) {};
	virtual void doSomething();
};

class Boo : public Baddie
{
public:
	Boo(StudentWorld* world, int imageID, int startX, int startY)
		: Baddie(world, imageID, startX, startY) {};
	virtual void doSomething();
};

class Square : public Actor
{
public:
	Square(StudentWorld* world, int imageID, int startX, int startY, int startDirection)
		: Actor(world, imageID, startX, startY, startDirection, 2), peachHere(true), yoshiHere(true) {};
	virtual void doSomething() = 0;
	int onSquare();
	bool playerState(int playerNum); 
	void newPlayer(int x);
private:
	bool peachHere;
	bool yoshiHere;
};

// all types of squares
class CoinSquare : public Square
{
public:
	CoinSquare(StudentWorld* world, int imageID, int startX, int startY)
		: Square(world, imageID, startX, startY, 0) {};
	virtual void doSomething(int x);
};

class BlueCoinSquare : public CoinSquare
{
public:
	BlueCoinSquare(StudentWorld* world, int imageID, int startX, int startY)
		: CoinSquare(world, imageID, startX, startY) {};
	virtual void doSomething();
};

class RedCoinSquare : public CoinSquare
{
public:
	RedCoinSquare(StudentWorld* world, int imageID, int startX, int startY)
		: CoinSquare(world, imageID, startX, startY) {};
	virtual void doSomething();
};

class StarSquare : public Square
{
public:
	StarSquare(StudentWorld* world, int imageID, int startX, int startY)
		: Square(world, imageID, startX, startY, 0) {};
	virtual void doSomething();
};
class EventSquare : public Square
{
public:
	EventSquare(StudentWorld* world, int imageID, int startX, int startY)
		: Square(world, imageID, startX, startY, 0) {};
	virtual void doSomething();
};
class BankSquare : public Square
{
public:
	BankSquare(StudentWorld* world, int imageID, int startX, int startY)
		: Square(world, imageID, startX, startY, 0) {};
	virtual void doSomething();
};
class DirectionalSquare : public Square
{
public:
	DirectionalSquare(StudentWorld* world, int imageID, int startX, int startY, int startDirection)
		: Square(world, imageID, startX, startY, startDirection), m_startDir(startDirection) {};
	virtual void doSomething();
private: 
	int m_startDir;
};
class DroppingSquare : public Square
{
public:
	DroppingSquare(StudentWorld* world, int imageID, int startX, int startY)
		: Square(world, imageID, startX, startY, 0) {};
	virtual void doSomething();
};

#endif // ACTOR_H_

