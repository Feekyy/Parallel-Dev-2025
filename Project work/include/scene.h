#ifndef SCENE_H
#define SCENE_H

#include "addons.h"

void draw_sidebar();

RGBColor handle_color_wheel_click(Point click, int window_width, int window_height);

void hsv_to_rgb(float h, float s, float v, float* r, float* g, float* b);

#endif


