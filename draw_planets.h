#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>


void DrawPlanets(float server_time) {



    Vector3 spherePosition = { 15.0f, 300.0f * sin(0.1f * server_time), 300.0f * cos(0.1f * server_time)};
    DrawSphere(spherePosition, 150.0f, PINK);

    Vector3 spherePosition2 = { 15.0f, 300.0f * sin(0.1f * server_time) + 150.0f * sin(1.12f * server_time),
                               300.0f * cos(0.1f * server_time) + 150.0f * cos(2.534f * server_time)};
    DrawSphere(spherePosition2, 15.0f, ORANGE);

    Vector3 spherePosition3 = { 15.0f  + 2.5f * cos(0.127f * server_time)
                                , 0.5f + 0.5f * sin(0.1f * server_time), 40.0f + 2.5f * cos(0.1f * server_time)};

    Vector3 spherePosition4 = { 15.0f + 2.5f * cos(0.127f * server_time) + 1.0f * cos(4.236f * server_time)
                                , 0.5f + 0.5f * sin(0.1f * server_time) + 1.00f * sin(1.12f * server_time),
                               40.0f + 2.5f * cos(0.1f * server_time) + 1.0f * cos(2.534f * server_time)};
    DrawSphere(spherePosition4, 0.1f, ORANGE);
    DrawSphere(spherePosition3, 1.0f, (Color) {0, 0, 255, 100});


}
