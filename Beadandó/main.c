#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image_import.h"
#include "countor.h"

int main(void)
{
    int i;
    
    Image *ima = (Image*)malloc(sizeof(Image));

    import(ima, "horse.png");

    edgeDetection(ima);

    free(ima);
}
