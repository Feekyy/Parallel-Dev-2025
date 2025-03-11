#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>
#include <stdbool.h>
#include <stdio.h>

#include "app.h"
#include "addons.h"

int main(int argc, char* argv[])
{
    int error_code;
    SDL_Window* window;
    SDL_GLContext gl_context;

    error_code = initialize_app(&window, &gl_context);
    if (error_code != 0)
    {
        printf("[ERROR] App initialization failed: %d\n", error_code);
        return error_code;
    }
    run_app(window);
    cleanup_app(window, gl_context);

    return 0;

}


