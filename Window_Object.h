#ifndef DRAMA_CLUB_WINDOW_OBJECT_H
#define DRAMA_CLUB_WINDOW_OBJECT_H

#include <SDL2/SDL.h>


class Window_Object {
public:
    SDL_Window *Window;
    SDL_Renderer *Renderer;
    bool quit;

    Window_Object(char *TITLE, int width, int height);

    void handle_input(SDL_Event event);

    void clear_background();

    void swap_buffer();

    ~Window_Object();
};


#endif //DRAMA_CLUB_WINDOW_OBJECT_H
