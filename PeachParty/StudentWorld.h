#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include <string>
#include <vector>

class Actor;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
    public:
        StudentWorld(std::string assetPath);
        ~StudentWorld();
        virtual int init();
        virtual int move();
        virtual void cleanUp();
        Board::GridEntry getActorAt(int x, int y);

    private:
    std::string getBoardFile();
    void populateBoard(Board bd);
    Actor* peach;
    Board board;
    std::vector<Actor*> actors;
};

#endif // STUDENTWORLD_H_
