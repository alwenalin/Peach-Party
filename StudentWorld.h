#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include "Actor.h"
#include <string>
#include <vector>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

// global constants 
const int PLAYER_NUM_1 = 1; // peach
const int PLAYER_NUM_2 = 2; // yoshi
const int RIGHT_DIR = 0;
const int UP_DIR = 90;
const int LEFT_DIR = 180;
const int DOWN_DIR = 270;

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
  ~StudentWorld();
  virtual int init();
  virtual int move();
  virtual void cleanUp();
  bool isValidPos(int x, int y);
  Player* playerPtr(int playerNum);
  void randomSquare(int &x, int &y);
  void dropSquare(int x, int y);
  void addVortex(int x, int y, int moveDir);
  bool overlapVor(int x, int y, Baddie* &badPtr);

  int changeBank(int x) { return m_bank = m_bank + x; };
  int getBank() { return m_bank; };
private:
	vector<Square*> squares; // squares
	vector<Baddie*> baddies; // boos, bowser
	vector<Vortex*> vortex; // vortexes
	Player* peach;
	Player* yoshi;
	int m_bank; // peach and yoshi share same bank
};

#endif // STUDENTWORLD_H_
