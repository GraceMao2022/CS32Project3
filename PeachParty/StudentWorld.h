#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include <string>
#include <vector>

class Actor;
class PlayerAvatar;
class Square;
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
        int getBankBalance() { return bankBalance; }
        void setBankBalance(int amt) { bankBalance = amt; }
        Actor* chooseRandomSquare() const;

    private:
    std::string getBoardFile();
    void populateBoard(Board bd);
    PlayerAvatar* peach;
    PlayerAvatar* yoshi;
    Board board;
    std::vector<Actor*> actors;
    Actor* getActorAt(int x, int y);
    int bankBalance;
};

#endif // STUDENTWORLD_H_
