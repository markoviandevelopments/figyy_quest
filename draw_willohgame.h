#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

extern const Color LOLLIPOP_COLOR_LIST[];
extern const int LOLLIPOP_COLOR_COUNT;

int iteration = 0;

Vector3 blockPosition; //= {14.0f, 1.9f, 13.0f}; 

Color blockColor;

float blockPosition_x;
float blockPosition_y;
float blockPosition_z;

void DrawWillohgame() {

    Color cubeColor;

    if (iteration % 100 == 0) {
        int lil_block = rand() % 142;
        for (int i = 0; i < lil_block + 1; i++) {
                blockPosition.x = (rand() % 32) * 1.0f - (16) * 1.0f;
                blockPosition.y = 1.4f + (rand() % 10) * 0.1f;
                blockPosition.z = (rand() % 32) * 1.0f - (16) * 1.0f;
                blockColor = LOLLIPOP_COLOR_LIST[rand() % LOLLIPOP_COLOR_COUNT];
        }

    }

    //blockPosition.x = position.x;
    //blockPosition.y = position.y;
    //blockPosition.z = position.z;
    //blockColor = cubeColor;

    printf("iteration %d blockPosition: %f %f %f\n",iteration, blockPosition.x, blockPosition.y, blockPosition.z);

    Vector3 blockPosition = (Vector3) {blockPosition.x, blockPosition.y, blockPosition.z};

    // Draw the square as a cube
    DrawCube(blockPosition, 0.2f, 0.2f, 0.2f, blockColor);
    DrawCubeWires(blockPosition, 0.2f, 0.2f, 0.2f, DARKGRAY);

    iteration++;
}

