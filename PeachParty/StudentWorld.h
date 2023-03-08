#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include <string>
#include <vector>

class Actor;
class PlayerAvatar;
class Square;
class Enemy;
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
    public:
        StudentWorld(std::string assetPath);
        ~StudentWorld();
        virtual int init();
        virtual int move();
        virtual void cleanUp();
        Board::GridEntry getActorTypeAt(int x, int y);
        PlayerAvatar* getPeach() { return peach; }
        PlayerAvatar* getYoshi() { return yoshi; }
        std::vector<int> getValidDirsFromPos(int x, int y);
        int getBankBalance() const { return bankBalance; }
        void setBankBalance(int amt) { bankBalance = amt; }
        Actor* chooseRandomSquare() const;
        void replaceSquareWithDropping(int x, int y);
        std::string getGameText();
        Enemy* getOverlapEnemy(int x, int y);
        void addNewActor(Actor* actor);

    private:
        PlayerAvatar* peach; //pointer to peach
        PlayerAvatar* yoshi; //pointer to yoshi
        Board board; //board that player is playing on
        std::vector<Actor*> actors; //all other objects
        int bankBalance; //bank balance
    
        std::string getBoardFile();
        void populateBoard(Board bd);
        Actor* getSquareActorAt(int x, int y, int& index);
};

#endif // STUDENTWORLD_H_
