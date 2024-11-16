#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>


void DrawPyramid() {
    int i = 0;
    for (float y = 0.0f; y < 1.0f; y+= 0.05f) {
            // Determine square color (alternating pattern)
            Color squareColor = (i % 2 == 0) ? RED : YELLOW;
            // Position the square
            Vector3 position = {
                20.0f + y / 32.0f,
                y + 0.2f,
                20.0f + y / 32.0f
            };
            // Draw
            DrawCube(position, 1.0f - y, 0.05f, 1.0f - y, squareColor); // Thin height for a flat square
            DrawCubeWires(position, 1.0f - y, 0.05f, 1.0f - y, BLACK);  // Outline

        i++;
    }

}
