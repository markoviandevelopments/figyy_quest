#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>


int iteration = 0;

void AlterBouncer(float memory_list[10]) {




    if (memory_list[4] < 0.0f && memory_list[5] < 0.0f) {
        memory_list[4] = 38.0f;
        memory_list[5] = 20.0f;
        memory_list[6] = 0.001434f;
        memory_list[7] = 0.002f;
    }


    memory_list[4] += memory_list[6];
    memory_list[5] += memory_list[7];

    if (memory_list[4] > 31.0f) {
        memory_list[4] = 31.0f;
        memory_list[6] *= -1.0f;
    }

    if (memory_list[4] < -33.0f) {
        memory_list[4] = -33.0f;
        memory_list[6] *= -1.0f;
    }

    if (memory_list[5] > 31.0f + 64.0f) {
        memory_list[5] = 31.0f + 64.0f;
        memory_list[7] *= -1.0f;
    }

    if (memory_list[5] < 31.0f) {
        memory_list[5] = 31.0f;
        memory_list[7] *= -1.0f;
    }

    iteration++;

}
