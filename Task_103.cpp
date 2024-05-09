#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define CIRCLE_RADIUS 50
#define SECOND_CIRCLE_RADIUS 20
#define SECOND_CIRCLE_SPEED 5

bool initializeSDL(SDL_Window** window, SDL_Renderer** renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return false;
    }

    *window = SDL_CreateWindow("Circle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (*window == NULL) {
        printf("Window creation failed: %s\n", SDL_GetError());
        return false;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (*renderer == NULL) {
        printf("Renderer creation failed: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

void drawSolidCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
    for (int x = -radius; x <= radius; x++) {
        for (int y = -radius; y <= radius; y++) {
            if (x*x + y*y <= radius*radius) {
                SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
            }
        }
    }
}

bool checkCollision(int x1, int y1, int radius1, int x2, int y2, int radius2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int distanceSquared = dx*dx + dy*dy;
    int radiusSumSquared = (radius1 + radius2) * (radius1 + radius2);
    return distanceSquared <= radiusSumSquared;
}

int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    if (!initializeSDL(&window, &renderer)) {
        return 1;
    }

    SDL_Event event;
    bool running = true;

    int circle1X = 0; // Starting position of the first circle
    int circle1Y = SCREEN_HEIGHT / 2;
    int circle2X = SCREEN_WIDTH / 2; // Starting position of the second circle
    int circle2Y = 0;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Update positions
        circle1X++;
        if (circle1X > SCREEN_WIDTH + CIRCLE_RADIUS) {
            circle1X = -CIRCLE_RADIUS; // Reset if circle goes off-screen
        }

        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
        if (currentKeyStates[SDL_SCANCODE_UP]) {
            circle2Y -= SECOND_CIRCLE_SPEED;
        }
        if (currentKeyStates[SDL_SCANCODE_DOWN]) {
            circle2Y += SECOND_CIRCLE_SPEED;
        }
        if (currentKeyStates[SDL_SCANCODE_LEFT]) {
            circle2X -= SECOND_CIRCLE_SPEED;
        }
        if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
            circle2X += SECOND_CIRCLE_SPEED;
        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 255, 150, 220, 255);
        SDL_RenderClear(renderer);

        // Draw circles
        SDL_SetRenderDrawColor(renderer, 244, 42, 65, 255);
        drawSolidCircle(renderer, circle1X, circle1Y, CIRCLE_RADIUS);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        drawSolidCircle(renderer, circle2X, circle2Y, SECOND_CIRCLE_RADIUS);

        // Check for collision
        bool collision = checkCollision(circle1X, circle1Y, CIRCLE_RADIUS, circle2X, circle2Y, SECOND_CIRCLE_RADIUS);
        if (collision) {
            // Draw collision effect
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderDrawLine(renderer, circle1X, circle1Y, circle2X, circle2Y);
        }

        // Render the scene
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
