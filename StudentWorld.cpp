#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Board.h"
#include <string>
#include <sstream> 
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

// constructor
StudentWorld::StudentWorld(string assetPath)
    : GameWorld(assetPath), m_bank(0)
{
    peach = nullptr;
    yoshi = nullptr;
    squares.clear();
    baddies.clear();
    vortex.clear();
}

// destructor
StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init()
{
    startCountdownTimer(99);  
    
    Board bd;
    string file1 = "board0";
    string file2 = to_string(getBoardNumber());
    string file3 = ".txt";
    string boardFile = assetPath() + (file1.append(file2)).append(file3);
    Board::LoadResult result = bd.loadBoard(boardFile);

    if (result != Board::load_success)
    {
        return  GWSTATUS_BOARD_ERROR;
    }
    else
    {
        // create new actor for each starting position on the board file
        for (int a = 0; a < BOARD_WIDTH; a++) 
        {
            for (int b = 0; b < BOARD_HEIGHT; b++)
            {
                Board::GridEntry ge = bd.getContentsOf(a, b);
                switch (ge)
                {
                    case Board::empty:
                        break;
                    case Board::player:
                        yoshi = new Player(this, PLAYER_NUM_2, IID_YOSHI, a, b);
                        peach = new Player(this, PLAYER_NUM_1, IID_PEACH, a, b);
                        squares.push_back(new BlueCoinSquare(this, IID_BLUE_COIN_SQUARE, a, b));
                        break;
                    case Board::blue_coin_square:
                        squares.push_back(new BlueCoinSquare(this, IID_BLUE_COIN_SQUARE, a, b));
                        break;
                    case Board::red_coin_square:
                        squares.push_back(new RedCoinSquare(this, IID_RED_COIN_SQUARE, a, b));
                        break;
                    case Board::up_dir_square: 
                        squares.push_back(new DirectionalSquare(this, IID_DIR_SQUARE, a, b, UP_DIR));
                        break;
                    case Board::down_dir_square: 
                        squares.push_back(new DirectionalSquare(this, IID_DIR_SQUARE, a, b, DOWN_DIR));
                        break;
                    case Board::left_dir_square: 
                        squares.push_back(new DirectionalSquare(this, IID_DIR_SQUARE, a, b, LEFT_DIR));
                        break;
                    case Board::right_dir_square:
                        squares.push_back(new DirectionalSquare(this, IID_DIR_SQUARE, a, b, RIGHT_DIR));
                        break;
                    case Board::event_square:
                        squares.push_back(new EventSquare(this, IID_EVENT_SQUARE, a, b));
                        break;
                    case Board::bank_square:
                        squares.push_back(new BankSquare(this, IID_BANK_SQUARE, a, b));
                        break;
                    case Board::star_square:
                        squares.push_back(new StarSquare(this, IID_STAR_SQUARE, a, b));
                        break;
                    case Board::bowser:
                        baddies.push_back(new Bowser(this, IID_BOWSER, a, b));
                        squares.push_back(new BlueCoinSquare(this, IID_BLUE_COIN_SQUARE, a, b));
                        break;
                    case Board::boo:
                        baddies.push_back(new Boo(this, IID_BOO, a, b));
                        squares.push_back(new BlueCoinSquare(this, IID_BLUE_COIN_SQUARE, a, b));
                        break;
                }
            }
        }
    }

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit ESC.
    // Notice that the return value GWSTATUS_NOT_IMPLEMENTED will cause our framework to end the game.

    setGameStatText("Game will end in a few seconds");

    // scoreboard text on top of game
    // still need bank 
    ostringstream text;
    text << "P1 Roll: " << peach->dieRoll() << " Stars: " << peach->getStars() << " $$: " << peach->getCoins();
    if (peach->getVor())
    {
        text << " VOR";
    }
    text << " | Time: " << timeRemaining() << " | Bank: " << getBank() << " | P2 Roll: " << yoshi->dieRoll() << " Stars: " << yoshi->getStars() << " $$: " << yoshi->getCoins();
    if (yoshi->getVor())
    {
        text << " VOR";
    }
    setGameStatText(text.str());
    
    // calls all actors to do something every tick 
    peach->doSomething();
    yoshi->doSomething();

    for (size_t i = 0; i < squares.size(); i++)
    {
        squares.at(i)->doSomething(); 
    }

    for (size_t j = 0; j < baddies.size(); j++)
    {
        baddies.at(j)->doSomething();
    }

    for (size_t k = 0; k < vortex.size(); k++)
    {
        if (vortex.at(k)->getActive())
        {
            vortex.at(k)->doSomething();
        }
        else // vortex is not active, so it gets deleted
        {
            delete vortex.at(k);
            vortex.erase(vortex.begin() + k);
        }
    }

    // display winning screen
    int winner = 0;
    if (timeRemaining() <= 0)
    {
        playSound(SOUND_GAME_FINISHED);
        if (peach->getStars() == yoshi->getStars())
        {
            if (peach->getCoins() > yoshi->getCoins())
            {
                winner = PLAYER_NUM_1;
            }
            else if (peach->getCoins() < yoshi->getCoins())
            {
                winner = PLAYER_NUM_2;
            }
            else // tie (stars and coins are equal)
            {
                winner = randInt(1, 2); // randomly pick one
            }
        }
        else if (peach->getStars() > yoshi->getStars())
        {
            winner = PLAYER_NUM_1;
        }
        else // yoshi has more stars than peach
        {
            winner = PLAYER_NUM_2;
        }

        if (winner == PLAYER_NUM_1) // peach
        {
            setFinalScore(peach->getStars(), peach->getCoins());
            return GWSTATUS_PEACH_WON;
        }
        else if (winner == PLAYER_NUM_2) // yoshi
        {
            setFinalScore(yoshi->getStars(), yoshi->getCoins());
            return GWSTATUS_YOSHI_WON;
        }
    }

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (size_t i = 0; i < squares.size(); i++)
    {
        delete squares.at(i);
    }

    for (size_t j = 0; j < baddies.size(); j++)
    {
        delete baddies.at(j);
    }

    for (size_t k = 0; k < vortex.size(); k++)
    {
        delete vortex.at(k);
    }

    delete peach;
    delete yoshi;
    peach = nullptr; 
    yoshi = nullptr;
}

// returns true if there's a square on the board for a certain x,y position
bool StudentWorld::isValidPos(int x, int y)
{   
    for (size_t i = 0; i < squares.size(); i++)
    {
        if ((x == squares.at(i)->getX()) && (y == squares.at(i)->getY()))
        {
            return true;
        }
    }
    return false;
}

// return a pointer to a specific player
Player* StudentWorld::playerPtr(int playerNum)
{
    if (playerNum == 1)
    {
        return this->peach;
    }
    else // playerNum == 2
    {
        return this->yoshi;
    }
}

// chooses a random square
void StudentWorld::randomSquare(int& x, int& y)
{
    int random = randInt(0, squares.size());

    x = squares.at(random)->getX();
    y = squares.at(random)->getY();
}

// replace a square with a dropping square
void StudentWorld::dropSquare(int x, int y)
{
    for (size_t i = 0; i < squares.size(); i++)
    {
        if ((x == squares.at(i)->getX()) && (y == squares.at(i)->getY()))
        {
            delete squares.at(i); // deletes square bowser is standing on
            squares.erase(squares.begin() + i);
            squares.push_back(new DroppingSquare(this, IID_DROPPING_SQUARE, x / 16, y / 16));
            return;
        }
    }
}

// create a vortex at player's position
void StudentWorld::addVortex(int x, int y, int moveDir)
{
    vortex.push_back(new Vortex(this, IID_VORTEX, x / 16, y / 16, moveDir));
}

// returns true if vortex and a baddie overlap (make contact), false if not
bool StudentWorld::overlapVor(int x, int y, Baddie*& badPtr)
{
    for (size_t j = 0; j < baddies.size(); j++)
    {
        // absolute value is needed when vortex is coming from the right or above
        if (abs((baddies.at(j))->getY() - y) < 16 && abs((baddies.at(j))->getX() - x) < 16)
        {
            badPtr = baddies.at(j);
            return true;
        }
    }

    return false; 
}
