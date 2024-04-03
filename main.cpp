#include "raylib.h"
#include "include/objects.h"

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1920/2;
    const int screenHeight = 1080/2;
    //initializing a pong ball and 2 player rectangles
    ball circle;
    player rect[2];
    //setting up pointers to my player objects and initializing their positions
    player* pL = &rect[0];
    player* pR = &rect[1];
    initializePlayerPositions(pL, pR);
    int winner;
    //SetWindowIcon(LoadImage("pong-icon.ico")); // Set icon for window
    InitWindow(screenWidth, screenHeight, "Pong!");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //---------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

        //Update
        updateBallPos(&circle);
        //Take care of collisions and see if anybody won 
        winner = detectCollisions(&circle, pL, pR);
        if (winner != 0) {
            break;
        }
        //----------------------------------------------------------------------------------
        //passing pointers to my 2 instances rect of player class so the controls() funct can take care of listening to kb input and updating their positions
        controls(pL, pR);
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);
            DrawCircle(circle.position.x, circle.position.y, circle.radius, circle.color);
            for (int i = 0; i < 2; i++) {
                DrawRectangleRec(rect[i].body, rect[i].color);
            }
            DrawFPS(0, 0);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    return 0;
}