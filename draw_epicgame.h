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
} RemotePlayerIn2;



int DrawEpicgame(RemotePlayerIn2 players[10], int type_in, int state_in, float information_in, float memory_list[10], Sound coin_sound_in) {

    //Color squareColor = RED;

    int return_value = -1;
    int anyone_in = 0;

    for (int i=0;i < 10; i++) {
        float x=players[i].position.x;
        float z=players[i].position.z;
        float dist = pow(pow(x - (-1 * 29.0f), 2) + pow(z - 29.6f, 2), 0.5f);
        if (dist < 2.0f){
            //squareColor = GREEN;
            anyone_in = 1;
            return_value = 1;

        } else {
            return_value = 1;
            memory_list[1] = -0.1f;
        }

    }

    if (anyone_in == 1) {
        memory_list[1] = -0.5f;
    }


    float y = 0;
    if (information_in >= 0.0f && type_in == 1) {
        y = information_in;
    }


    //Vector3 position = {
    //   19.0f,
    //   0.9f + y,
    //   19.0f
    //   };

    // Draw the square as a cube
    //DrawCube(position, 0.2f, 0.2f + 1.0f + 1.0f * sin(3.14f * sin(GetTime())), 0.2f, squareColor); // Thin height for a flat square
    //DrawCubeWires(position, 0.2f, 0.2f + 1.0f + 1.0f * sin(3.14f * sin(GetTime())), 0.2f, BLACK);  // Outline




    static Model model5 = { 0 };
    static bool isModel5Loaded = false;

    if (!isModel5Loaded) {
        // Load the model
        model5 = LoadModel("models/prestoncoin.obj"); // Ensure the model file is in the correct directory

        // Check if the model has normals
        if (model5.meshes[0].normals == NULL) {
            printf("NO NORMALS\n");
            // Optionally generate normals if needed
            // GenMeshTangents(&model4.meshes[0]); // Not necessary for flat shading
        }

        isModel5Loaded = true;
    }

    float rotationAngle5;

    // Define model transformation parameters
    Vector3 modelPosition5 = { -29.0f, 1.5f, 29.6f };
    Vector3 modelScale5 = { 0.015f, 0.015f, 0.015f };
    Vector3 rotationAxis5 = { 0.0f, 1.0f, 0.0f };

    if (anyone_in == 1 || ( type_in == 1 && information_in >= 0)) {
        rotationAngle5 = 90.0f * sin(GetTime() * 7.0f);
        int r = rand() % 100;
        if (r == 0) {
            PlaySound(coin_sound_in);
        }
    } else {
        rotationAngle5 = -90.0f;
    }


    // Draw the model with rotation and scaling
    DrawModelEx(model5, modelPosition5, rotationAxis5, rotationAngle5, modelScale5, (Color) {255, 215, 0, 205});

    // Draw the model wireframe over it
    DrawModelWiresEx(model5, modelPosition5, rotationAxis5, rotationAngle5, modelScale5, BLACK);





    return return_value;


}
