#include <iostream>
#include <algorithm>
#include <cmath>
#include <string>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>

//additional header files
#include "Food.h"
#include "SnakeComponent.h"
#include "Vector2.h"

bool Initialize();
void RunLoop();
void ShutDown();

void ProcessInput();
void UpdateGame();
void GenerateOutput();

class Game {
    public:
        Game():mWindow(nullptr), mIsRunning(true) {
            mTicksCount = 0;
        }

        ~Game() {
            /* for (auto component : snake)
            {
                delete component;
            } 
            snake.clear(); */
         }

        bool Initialize();
        void RunLoop();
        void ShutDown();

    private:
        //used to track state of snake
        enum State {
            EActive,
            EPaused,
            EDead
        };

        //used to track direction of snake
        enum Direction {
            Up,
            Down,
            Right,
            Left
        };

        SDL_Window* mWindow;
        SDL_Renderer* mRenderer;

        Uint32 mTicksCount;

        Direction mSnakeDir;

        Vector2 foodPosition = Vector2(96,160);
        Vector2 snakeHeadPos = Vector2(0,0), snakeTailPos = Vector2(0,0);

        float thickness = 16.0f, deltaTime, moveSpeed = 100.0f, counter1, counter2;
        
        bool mIsRunning;

        std::vector<SnakeComponent> snake;

        Vector2 mSnakePos = Vector2(512,384);

        void ProcessInput();
        void UpdateGame();
        void GenerateOutput();
};

bool Game::Initialize() {
    //random number generator seed
    srand (time(NULL));

    //initialize SDL library
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);
    if(sdlResult != 0) {
        //initialization failed
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    //if initialization successful, create window
    mWindow = SDL_CreateWindow(
        "Snake",
        100,
        100,
        1024,
        768,
        0 // Flags
    );

    if(!mWindow) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    mRenderer = SDL_CreateRenderer(
        mWindow,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC //initialization flags. use accelerated renderer and enable vsync
    );

    //if window and initialization successful, return true
    return true;
}

/* 
* Step 1: Instantiate head + initiate movement
* Step 2: Instantiate head + implement smooth movement
*/

void Game::RunLoop() {
    while (mIsRunning) {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::ShutDown() {
    //destroy SDL_Window
    SDL_DestroyWindow(mWindow);

    //destroy SDL_Renderer
    SDL_DestroyRenderer(mRenderer);

    //close SDL
    SDL_Quit();
}

void Game::ProcessInput() {
    SDL_Event event;

    //go through all events and respond as desired/appropriate
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT:
                mIsRunning = false;
                break;
        }
    }

    // Retrieve the state of all of the keys then scan each as desired
    const Uint8* state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_ESCAPE]){
        mIsRunning = false;
    }

    //update snake movement
    if(state[SDL_SCANCODE_LEFT]) {
        mSnakeDir = Left;
    } else if(state[SDL_SCANCODE_RIGHT]) {
        mSnakeDir = Right;
    } else if(state[SDL_SCANCODE_UP]) {
        mSnakeDir = Up;
    } else if(state[SDL_SCANCODE_DOWN]) {
        mSnakeDir = Down;
    }
}

void Game::UpdateGame() {
    //update frame at fixed intervals (fixedDeltaTime)
    while(!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

    //get deltaTime
    deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    mTicksCount = SDL_GetTicks();

    //clamp deltaTime
    if(deltaTime > .05f) {
        deltaTime = 0.05f;
    }

    if(counter1 > 0.1f) {
        counter1 = 0.0f;
        if(mSnakeDir == Left) {
            mSnakePos.x -= 16;
        } else if(mSnakeDir == Right) {
            mSnakePos.x += 16;
        } else if(mSnakeDir == Up) {
            mSnakePos.y -= 16;
        } else if(mSnakeDir == Down) {
            mSnakePos.y += 16;
        }
    }

    if(snake.size() == 0) {
        for(int i = 0; i < 5; i++) {
            SnakeComponent snakeBody(mSnakePos + Vector2(i*16,0));
            snake.push_back(snakeBody);
        }
    }

    if(counter1 > 0.0f) {
        counter1 += deltaTime;
    } else {
        //update position
        snake[0].UpdatePosition(mSnakePos);
        snakeHeadPos = snake[0].GetPosition();

        //std::cout << snakeHeadPos.x << "," << snakeHeadPos.y << std::endl;
        for(int i = snake.size() - 1; i >= 0; i--) {
            snake[i].UpdatePosition(snake[i-1].GetPosition());
            if(i != 1) {
                if(snakeHeadPos.x == snake[i].GetPosition().x && snakeHeadPos.y == snake[i].GetPosition().y) {
                    //restart game
                    
                }
            }
        }

        snakeTailPos = snake[snake.size() - 1].GetPosition();

        counter1 += deltaTime;

        /* At every movement update, check if food location == snake head location.
        if so, add a segment and eliminate the food.*/
        Vector2 foodPosInt = Vector2(static_cast<int>(foodPosition.x),static_cast<int>(foodPosition.y));

        if(foodPosInt.x == snakeHeadPos.x && foodPosInt.y == snakeHeadPos.y) {
            //std::cout << "FOOD!" << std::endl;
            //find new position for food
            foodPosition.x = (rand() % 64 + 1) * 16;
            foodPosition.y = (rand() % 47 + 1) * 16;
             
            //add new segment to snake
            SnakeComponent snakeBody(snakeTailPos);
            snake.push_back(snakeBody);
        }
    }
}

void Game::GenerateOutput() {

    SDL_SetRenderDrawColor(mRenderer,0,0,0,0);

    SDL_RenderClear(mRenderer); //clear back buffer to current draw color

    SDL_SetRenderDrawColor(mRenderer,255,255,255,255);

    SDL_Rect topWall = {0,0,1024,thickness};
    SDL_Rect bottomWall = {0,752,1024,thickness};
    SDL_Rect leftWall = {0,0,thickness,768};
    SDL_Rect rightWall = {1008,0,thickness,768};

    SDL_RenderFillRect(mRenderer,&topWall);
    SDL_RenderFillRect(mRenderer,&rightWall);
    SDL_RenderFillRect(mRenderer,&leftWall);
    SDL_RenderFillRect(mRenderer,&bottomWall);

    //UPDATE POSITION OF THE SNAKE COMPONENTS!!!!!!!

    //draw snake
    for(int i = snake.size() - 1; i >= 0; i--) {
        snake[i].Draw(mRenderer);
    }

    //create food
    Food foodInstance(foodPosition);

    //draw food
    foodInstance.Draw(mRenderer);

    //std::cout << "Running 3" << std::endl;

    SDL_RenderPresent(mRenderer);
} 