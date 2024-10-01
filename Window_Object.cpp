#include "Window_Object.h"

Window_Object::Window_Object(char *TITLE, int width, int height) {
    Window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    quit = false;
}

void Window_Object::handle_input(SDL_Event event) {
    if (event.type == SDL_QUIT) {
        quit = true;
    }
}

void Window_Object::clear_background() {
    SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
    SDL_RenderClear(Renderer);
}

void Window_Object::swap_buffer() {
    SDL_RenderPresent(Renderer);
}

Window_Object::~Window_Object() {
    SDL_DestroyWindow(Window);
    SDL_DestroyRenderer(Renderer);
}
