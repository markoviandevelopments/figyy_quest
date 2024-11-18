#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>


#define MAX_PLAYERS 10

typedef struct {
    int id;
    Vector3 position;
} RemotePlayerIn;


void DrawPrestongame(RemotePlayerIn players[10]) {

    Color squareColor = RED;

    for (int i=0;i < 10; i++) {
        float x=players[i].position.x;
        float z=players[i].position.z;
        float dist = pow(pow(x - 13, 2) + pow(z - 13, 2), 0.5f);
        if (dist < 2.0f){
            squareColor = GREEN;
        }
    }

    Vector3 position = {
       13.0f,
       0.9f,
       13.0f
       };

    // Draw the square as a cube
    DrawCube(position, 0.2f, 0.2f + 1.0f + 1.0f * sin(3.14f * sin(GetTime())), 0.2f, squareColor); // Thin height for a flat square
    DrawCubeWires(position, 0.2f, 0.2f + 1.0f + 1.0f * sin(3.14f * sin(GetTime())), 0.2f, BLACK);  // Outline

}
