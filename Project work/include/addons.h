#ifndef ADDONS_H
#define ADDONS_H

typedef enum 
{
    SHAPE_POINT,
    SHAPE_LINE,
    SHAPE_RECTANGLE,
    SHAPE_CIRCLE,
    SHAPE_TRIANGLE,
    SHAPE_SQUARE
} ShapeType;

typedef struct RGBColor 
{
    unsigned char red, green, blue;
} RGBColor;

typedef struct Point 
{
    float x;
    float y;
} Point;

#endif