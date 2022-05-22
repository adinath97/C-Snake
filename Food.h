#include <iostream>
#include "Vector2.h"

void Draw(SDL_Renderer* mRenderer);
void UpdatePosition(Vector2 newPosition);
bool CheckForCollision(Vector2 snakeHeadPos);

const int FOOD_HEIGHT = 15;
const int FOOD_WIDTH = 15;

class Food {
    public:
        Food(Vector2 position): position(position) {
            FoodRect.x = static_cast<int>(position.x);
		    FoodRect.y = static_cast<int>(position.y);
		    FoodRect.w = FOOD_WIDTH;
		    FoodRect.h = FOOD_HEIGHT;
        }

        Vector2 position;
        SDL_Rect FoodRect;

        void Draw(SDL_Renderer* mRenderer);
        void UpdatePosition(Vector2 newPosition);
        bool CheckForCollision(Vector2 snakeHeadPos);
};

void Food::Draw(SDL_Renderer* mRenderer) {
    FoodRect.x = static_cast<int>(position.x);
    FoodRect.y = static_cast<int>(position.y);

	SDL_RenderFillRect(mRenderer, &FoodRect);
}

void Food::UpdatePosition(Vector2 newPosition) {
    position = newPosition;
}

bool Food::CheckForCollision(Vector2 snakeHeadPos) {
    if(snakeHeadPos.x == position.x && snakeHeadPos.y == position.y) { return true; }

    return false;
}
