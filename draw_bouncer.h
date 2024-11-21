#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>



float server_frac = 0.2f;

int DrawBouncer(int type_in, int state_in, float information_in, float memory_list[10]) {

    //Color squareColor = RED;

    if (type_in == 3) {
        if (state_in == 0) {
            memory_list[4] = information_in;
            memory_list[5] += memory_list[7];
        }
        if (state_in == 1) {
            memory_list[5] = information_in;
            memory_list[4] += memory_list[6];
        }
        if (state_in == 2) {
            memory_list[6] = information_in;
        }
        if (state_in == 3) {
            memory_list[7] = information_in;
        }

    }




    Vector3 position = {
       memory_list[4],
       -0.5f,
       memory_list[5]
    };

    // Draw the square as a cube
    DrawCube(position, 0.2f, 0.2f, 0.2f, (Color) {255, 0, 0, 255}); // Thin height for a flat square
    DrawCubeWires(position, 0.2f, 0.2f, 0.2f, BLACK);  // Outline



    return 3;


}
