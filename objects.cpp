#include "objects.h"
#include "raylib.h"
#include <cmath>
#include <iostream>
//CONSTRUCTORS TO OBJECT CLASS CHILDREN AND INITIALIZATION
ball::ball(Color clr) {
    position = {(float)1920/4, (float)1080/4};
    color = clr;
    radius = 15.0f;
    //motion.x = (float)GetRandomValue(-75, 75)/10;
    motion.x = 7.5f;
    motion.y = 0.0f;
    velocity = sqrt(motion.x*motion.x + motion.y*motion.y);
    //motion.y = (float)GetRandomValue(-75, 75)/10;
}
//=================================
//=================================
player::player(Color clr) {
    color = clr;
    body = {0.0f, 0.0f, 20.0f, 100.0f};
}

void player::asessMovement(void) {
    float x = body.y - prevPosition;
    if (x==0)
        dir = STILL;
    if (x<0)
        dir = UP;
    if (x>0)
        dir = DOWN;
}
//=================================
//=================================
void initializePlayerPositions(player *pL, player *pR) {
    //let's put the players in the middle of window's height and 3*body.x pixels from the borders
    pL->body.y= (1080/4-(pL->body.height/2));
    pR->body.y = pL->body.y;
    pL->body.x = 3*pL->body.width;
    pR->body.x = 1920/2 - 4*(pR->body.width);
    pL->left = true;
    pR->left = false;
    return;
}
//END OF CONSTRUCTORS
//=================================
//=================================
//COLLISION RELATED STUFF BELOW !!!
void onImpact(ball *ball) {
    //function with only the ball ptr passed into it - it bounced off of the ceiling or floor
    ball->motion.y = ball->motion.y * (-1) + ((float)GetRandomValue(-100, 100)/100);
    return;
}

void onImpact(ball *ball, player *body) {

    //we're gonna teleport the ball 1 px outside the rectangle to prevent clipping lol
    //based on whether the player was going up, down or not moving at all, we change the bounce angle
    //left player
    std::cout << '\a';
    float bounceAngle = PI/4;
    if (body->left == true) {
        ball->position.x = body->body.x + body->body.width + 1 + ball->radius;
        switch(body->dir) {
            case UP:
                ball->motion.y = ball->motion.x * tan((atan(ball->motion.y/ball->motion.x + bounceAngle)));
                break;
            case DOWN:
                ball->motion.y = ball->motion.x * tan((atan(ball->motion.y/ball->motion.x - bounceAngle)));
                break;
            case STILL:
                break;
        }
    }
    //right player
    if (body->left == false) {
        ball->position.x = body->body.x -1 - ball->radius;
        switch(body->dir) {
            case UP:
                ball->motion.y = ball->motion.x * tan((atan(ball->motion.y/ball->motion.x - bounceAngle)));
                break;
            case DOWN:
                ball->motion.y = ball->motion.x * tan((atan(ball->motion.y/ball->motion.x + bounceAngle)));
                break;
            case STILL:
                break;
        }
    }
    ball->motion.x*=-1;
    float v2 = sqrt(pow(ball->motion.x, 2) + pow(ball->motion.y, 2));
    float modifier = (ball->velocity/v2); 
    ball->motion.x*= modifier;
    ball->motion.y*= modifier;

    return;
}

int detectCollisions(ball* circle,player *pL, player *pR) {
    //collided with left player - pL is passed as an indication it was the left player
    if(CheckCollisionCircleRec(circle->position, circle->radius, pL->body)) {
        onImpact(circle, pL);
    }
    //colided with right player - pR is passed as an indication it was the player on the right
    if(CheckCollisionCircleRec(circle->position, circle->radius, pR->body)) {
        onImpact(circle, pR);
    }

    //collided with floor - let's also teleport the ball so it's 1 pixel away from the surface it collided with to avoid clipping
    if(circle->position.y + circle->radius > 1080/2) {
        
        circle->position.y = 1080/2 - circle->radius -1;
        onImpact(circle);
    }
    //collided with ceiling
    if(circle->position.y - circle->radius < 0) {
        
        circle->position.y = 1 + circle->radius;
        onImpact(circle);
    }
    //collided with either horizontal border - resulting in the end of the game
    if (circle->position.x - circle->radius <= 0) {
        // touching left border - p2 wins!
        return 2;
    }
    if (circle->position.x + circle->radius >= 1920/2) {
        //touching the right border - p1 wins!
        return 1;
    }
    return 0;
}
//END OF COLLISION RELATED STUFF
//=================================
//=================================
//CONTROLS
void controls(player* playerL, player* playerR, int upperBound, int lwrBound) {

    //If a key w, s or either arrows are pressed, position is changed - if new position is beyond the window - it's set to the max available position that
    //still displays entire player rectangle
    float playerMovementSpeed = 6;
    //saving player's positions in order to asess their movement
    playerL->prevPosition = playerL->body.y;
    playerR->prevPosition = playerR->body.y;

    if (IsKeyDown(87)) 
    {
        playerL->body.y-=playerMovementSpeed;
        if (playerL->body.y < (float)upperBound)
            playerL->body.y = (float)upperBound;
    }
    if (IsKeyDown(83))
    {
        playerL->body.y+=playerMovementSpeed;
        if (playerL->body.y + playerL->body.height > (float)lwrBound)
            playerL->body.y = (float)lwrBound - playerL->body.height;
    }
    if (IsKeyDown(KEY_UP))
    {
        playerR->body.y-=playerMovementSpeed;
        if (playerR->body.y < (float)upperBound)
            playerR->body.y = (float)upperBound;

    }
    if (IsKeyDown(KEY_DOWN))
    {
        playerR->body.y+=playerMovementSpeed;
        if (playerR->body.y + playerR->body.height > (float)lwrBound)
            playerR->body.y = (float)lwrBound - playerR->body.height;

    }
    playerL->asessMovement();
    playerR->asessMovement();
    return;
}

void updateBallPos(ball *ball) {
    ball->position.x += ball->motion.x + 0.001;
    ball->position.y += ball->motion.y + 0.001;
    return;
}