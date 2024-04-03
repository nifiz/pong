#pragma once
#include "raylib.h"

//Enum created to keep track od player's movement - used for ball bouncing (no pun intended)
enum MOVE {UP, DOWN, STILL};

class object {
    public:
        //data
        Vector2 position;
        Color color;
        //functions
};

class ball: public object {
    public:
    Vector2 motion;
    float radius;
    float velocity;
        void onImpact(void);
    ball(Color clr = WHITE);
};

class player: public object {
    public:
        float prevPosition;
        Rectangle body;
        player(Color clr = WHITE);
        MOVE dir;
        void asessMovement(void);
        bool left;
};

void controls(player* playerL, player* playerR, int upperBound = 0, int lwrBound = 1080/2);
void initializePlayerPositions(player *pL, player *pR);
void onImpact(ball *ball, player *body);
void onImpact(ball *ball);
int detectCollisions(ball* circle,player *pL, player *pR);
void updateBallPos(ball *ball);