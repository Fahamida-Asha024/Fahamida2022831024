#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>

int windowWidth = 640;
int windowHeight = 640;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
bool isRunning = false;

int cirRad = 20;
int cirCenX = windowWidth / 2;
int cirCenY = windowHeight / 2;

bool initializeSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("first error: %s\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow(" Circle Task102", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    
    if (!window) {
        printf("error: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!renderer) {
        printf("renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    return true;
}

void setup() {
    SDL_SetRenderDrawColor(renderer,250,225,235, 255);
    SDL_RenderClear(renderer);
    SDL_Color color = { 0, 0, 0, 255 };
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void drawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
    for (int x = -radius; x <= radius; x++) {
        for (int y = -radius; y <= radius; y++) {
            if (x * x + y * y <= radius * radius) {
                SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
            }
        }
    }
}

void update() {
    cirRad += 2;

    if (cirRad >= 320) {
        cirRad = 20;
    }

    SDL_SetRenderDrawColor(renderer, 255, 150, 220, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer,255,255,255,0);
}

void render() {
    drawCircle(renderer, cirCenX, cirCenY, cirRad);
    SDL_RenderPresent(renderer);
}

void cleanup() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
int main(int argc, char* argv[]) {
    isRunning = initializeSDL();

    if (!isRunning) {
        return 1;
    }

    setup();

    while (isRunning) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
        }

        update();
        render();
    }

    cleanup();
    return 0;
}

