#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>




void AlterPrestongame(int type_in, int state_in, float information_in, float memory_list[10]) {

    if (state_in == 0 && information_in == -1.0f) {  // player far
        memory_list[0] = -10000.0f;
    } else if (state_in == 0 && information_in == 1.0f) { // player near
        memory_list[0] = -2.0f;
    }


}
