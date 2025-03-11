#include "scene.h"
#include "addons.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

void hsv_to_rgb(float h, float s, float v, float* r, float* g, float* b)
{
    float c = v * s;
    float x = c * (1 - fabs(fmod(h / 60.0, 2) - 1));
    float m = v - c;

    if(h >= 0 && h < 60) 
    {
        *r = c; *g = x; *b = 0;
    } 
    else if(h >= 60 && h < 120) 
    {
        *r = x; *g = c; *b = 0;
    } 
    else if(h >= 120 && h < 180) 
    {
        *r = 0; *g = c; *b = x;
    } 
    else if(h >= 180 && h < 240) 
    {
        *r = 0; *g = x; *b = c;
    } 
    else if(h >= 240 && h < 300) 
    {
        *r = x; *g = 0; *b = c;
    }
    else 
    {
        *r = c; *g = 0; *b = x;
    }

    *r += m; *g += m; *b += m;
}

RGBColor black = {0, 0, 0};
RGBColor white = {255, 255, 255};

void draw_color_wheel(float h, float s, float v, float* r, float* g, float* b) 
{
    hsv_to_rgb(h, s, v, r, g, b);
}


RGBColor handle_color_wheel_click(Point click, int window_width, int window_height) 
{
    float sidebar_width = 0.34f;
    float wheel_radius = 0.15f;
    float wheel_center_x = -1.0f + wheel_radius + 0.02f;
    float wheel_center_y = 1.0f - wheel_radius - 0.02f;

    float x = (2.0f * click.x) / window_width - 1.0f;
    float y = 1.0f - (2.0f * click.y) / window_height;

    float dx = x - wheel_center_x;
    float dy = y - wheel_center_y;
    float distance = sqrt(dx*dx + dy*dy);

    if (distance <= wheel_radius) 
    {
        float angle = atan2(dy, dx);
        if (angle < 0) angle += 2 * M_PI;
        float hue = angle * (180.0f / M_PI);

        float saturation = distance / wheel_radius;

        float r, g, b;
        hsv_to_rgb(hue, saturation, 1.0f, &r, &g, &b);

        printf("Selected color: (%.2f, %.2f, %.2f)\n", r, g, b);
        return (RGBColor){(unsigned char)(r * 255), (unsigned char)(g * 255), (unsigned char)(b * 255)};
    }
}

void draw_sidebar()
{
    float sidebar_width = 0.34f;
    float wheel_radius = 0.15f;
    float wheel_center_x = -1.0f + wheel_radius + 0.02f;
    float wheel_center_y = 1.0f - wheel_radius - 0.02f;
    int segments = 60;

    glColor3f(white.red / 255.0f, white.green / 255.0f, white.blue / 255.0f);
    glBegin(GL_QUADS);
        glVertex2f(-1.0f, 1.0f);
        glVertex2f(-1.0f + sidebar_width, 1.0f);
        glVertex2f(-1.0f + sidebar_width, -1.0f);
        glVertex2f(-1.0f, -1.0f);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(wheel_center_x, wheel_center_y);
    
    for(int i = 0; i <= segments; i++) 
    {
        float angle = i * (2.0f * M_PI / segments);
        float hue = i * (360.0f / segments);
        float r, g, b;
        hsv_to_rgb(hue, 1.0f, 1.0f, &r, &g, &b);
        
        glColor3f(r, g, b);
        glVertex2f
        (
            wheel_center_x + wheel_radius * cosf(angle),
            wheel_center_y + wheel_radius * sinf(angle)
        );
    }
    glEnd();
}
