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
        virtual int init();
        virtual int move();
        virtual void cleanUp();
   

    private:
    std::string getBoardFile();
    void populateBoard(Board bd);
    Actor* peach;
    std::vector<Actor*> actors;
};

#endif // STUDENTWORLD_H_
