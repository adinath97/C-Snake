#include <iostream>
#include "Vector2.h"

void Draw(SDL_Renderer* mRenderer);
void UpdatePosition(Vector2 newPosition);
Vector2 GetPosition();

const int HEAD_HEIGHT = 15;
const int HEAD_WIDTH = 15;

class SnakeComponent {
    public:
        SnakeComponent(Vector2 position): position(position) {
            SnakeRect.x = static_cast<int>(position.x);
		    SnakeRect.y = static_cast<int>(position.y);
		    SnakeRect.w = HEAD_WIDTH;
		    SnakeRect.h = HEAD_HEIGHT;
        }

        Vector2 position;
        SDL_Rect SnakeRect;

        void Draw(SDL_Renderer* mRenderer);
        void UpdatePosition(Vector2 newPosition);
        Vector2 GetPosition();
};

void SnakeComponent::Draw(SDL_Renderer* mRenderer) {
    SnakeRect.x = static_cast<int>(position.x);
    SnakeRect.y = static_cast<int>(position.y);

	SDL_RenderFillRect(mRenderer, &SnakeRect);
}

Vector2 SnakeComponent::GetPosition() {
    Vector2 output = Vector2(static_cast<int>(position.x),static_cast<int>(position.y));
    return output;
}

void SnakeComponent::UpdatePosition(Vector2 newPosition) {
    position = newPosition;
}