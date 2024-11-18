#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>


void DrawPrestongame() {
    Color squareColor = RED;
    Vector3 position = {
       13.0f,
       0.9f,
       13.0f
       };

    // Draw the square as a cube
    DrawCube(position, 0.2f, 0.2f, 0.2f, squareColor); // Thin height for a flat square
    DrawCubeWires(position, 0.2f, 0.2f, 0.2f, BLACK);  // Outline

}
