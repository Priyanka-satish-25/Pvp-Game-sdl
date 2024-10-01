#include "Pellets_Object.h"

Pellets_Object::Pellets_Object(std::vector<SDL_Rect> v, SDL_Renderer *Renderer)
        : Renderer(Renderer), pellets(v) {}

void Pellets_Object::draw() {
    SDL_SetRenderDrawColor(Renderer, 217, 2, 125, 255);
    for (auto rect: pellets) {
        SDL_RenderFillRect(Renderer, &rect);
    }
}

void Pellets_Object::update(SDL_Rect rect, int &score) {
    for (auto &pellet: pellets) {
        if (SDL_HasIntersection(&rect, &pellet)) {
            score += 10;
            pellet.x = -50;
            pellet.y = -50;
            count++;
        }
    }
}

bool Pellets_Object::is_empty() {
    return count == pellets.size();
}
