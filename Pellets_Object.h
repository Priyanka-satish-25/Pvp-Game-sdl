#ifndef DRAMA_CLUB_PELLETS_OBJECT_H
#define DRAMA_CLUB_PELLETS_OBJECT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

class Pellets_Object {
public:
    std::vector<SDL_Rect> pellets;
    int count{};

    SDL_Renderer *Renderer;

    Pellets_Object(std::vector<SDL_Rect> v, SDL_Renderer *Renderer);

    void draw();

    bool is_empty();

    void update(SDL_Rect rect, int &score);

};


#endif //DRAMA_CLUB_PELLETS_OBJECT_H
