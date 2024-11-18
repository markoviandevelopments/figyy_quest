#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>


void DrawWillohgame() {
    float x_pos = 0.2f;
    float y_pos = 1.2f;
    float z_pos = 0.2f;

    Color squareColor = PINK;
    Vector3 position = {
       14.0f,
       0.9f,
       13.0f
       };
       

    // Draw the square as a cube
    DrawCube(position, x_pos, y_pos, z_pos, squareColor); // Thin height for a flat square
    DrawCubeWires(position, x_pos, y_pos, z_pos, BLACK);  // Outline

}
