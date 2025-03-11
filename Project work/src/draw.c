#include "draw.h"
#include "addons.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

void draw_line(Point start, Point end, RGBColor color)
{
    glColor3f(color.blue, color.green, color.red);
    glBegin(GL_LINES);
        glVertex2f(start.x, start.y);
        glVertex2f(end.x, end.y);
    glEnd();
}