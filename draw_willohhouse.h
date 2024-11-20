#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include "raymath.h"


typedef struct {
    float x;
    float z;
} Cat;


Cat DrawWillohhouse(long double server_time, Camera3D *camera) {
    float dist;
    Cat cat;
    int x_c = 0;
    int z_c = 1;
    for (float x = -3; x < 3; x+= 0.1f) {
        x_c++;
        x_c++;
        for (float z = -3; z < 3; z+= 0.1f) {
            x_c++;
            Color squareColor = ((x_c + z_c) % 2 == 0) ? MAROON : RED;
            dist = pow(pow(x, 2) + pow(z, 2), 0.5f);
            if (dist < 2.95) {
                Vector3 position = {
                    10.0f + x,
                    0.2f,
                    z
                };
                DrawCube(position, 0.1f, 0.05f, 0.1f, squareColor); // Thin height for a flat square
            }

        }
    }


    int i = 0;
    float r_f;
    float r_f_2;
    float block_w = 0.05f;
    for (float y = 0.0f; y < 4.0f; y+= 0.04f) {
            for (float a = 0.0f; a < 6.29; a+= 0.01) {
                int rowIndex = (int)(y / 0.04f);
                int colIndex = (int)(a / 0.01f);
                Color squareColor = (rowIndex % 2 == 0) ? ((colIndex % 2 == 0) ? DARKBROWN : BROWN)
                                       : ((colIndex % 2 == 0) ? BROWN : BEIGE);

                // Position the square
                r_f = 1.0f;
                r_f_2 = 1.0f + 0.03f * sin(y * 15.0f);
                if (y > 3.0f) {
                    r_f = cos((y - 3.0f) * 3.141f / 2.0f);
                    block_w = 0.25f;
                    r_f_2 = 1.0f;
                }
                Vector3 position = {
                    10.0f + 3 * r_f * r_f_2 * sin(a),
                    y + 0.2f,
                    0.0f + 3 * r_f * r_f_2 * cos(a)
                };
                // Draw
                if (12 - 200 * pow((a - 5) * (a - 5), 1.2) - 5 * y < 0) {
                    DrawCube(position, block_w, 0.05f, block_w, squareColor);
                }
            }
        i++;
    }
}