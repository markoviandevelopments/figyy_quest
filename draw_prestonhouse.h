#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>


void DrawPrestonhouse() {


    float dist;
    int x_c = 0;
    int z_c = 1;
    for (float x = -3; x < 3; x+= 0.1f) {
        x_c++;
        x_c++;
        for (float z = -3; z < 3; z+= 0.1f) {
            x_c++;
            Color squareColor = ((x_c + z_c) % 2 == 0) ? GREEN : BROWN;
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
                // Determine square color (alternating pattern)
                Color squareColor = (i % 2 == 0) ? PURPLE : ORANGE;
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
                if (12 - 200 * pow((a - 2) * (a - 2), 1.2) - 5 * y < 0) {
                    DrawCube(position, block_w, 0.05f, block_w, squareColor); // Thin height for a flat square
                    //DrawCubeWires(position, block_w, 0.05f, block_w, BLACK);  // Outline
                }
            }
        i++;
    }

    static Model model = { 0 };
    static bool isModelLoaded = false;
    if (!isModelLoaded) {
        model = LoadModel("model.obj"); // Replace with your model path
        Texture2D texture = LoadTexture("texture.png"); // Replace with your texture path
        model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
        isModelLoaded = true;
    }

    // Draw the loaded model
    Vector3 modelPosition = { 10.1f, 3.1f, 0.0f }; // Adjust position as needed
    Vector3 modelScale = { 0.06f, 0.06f, 0.06f };    // Scale factor (1/5 = 0.2)
    Vector3 rotationAxis = { 0.0f, 1.0f, 0.0f };   // Rotation around Y-axis
    float rotationAngle = GetTime() * 30.0f;       // Rotate over time (30 degrees per second)

    // Draw the model with rotation and scaling
    DrawModelEx(model, modelPosition, rotationAxis, rotationAngle, modelScale, WHITE);

    static Model model2 = { 0 };
    static bool isModel2Loaded = false;
    if (!isModel2Loaded) {
        model2 = LoadModel("model_2.obj"); // Load the model
        Texture2D texture2 = LoadTexture("texture_2.png"); // Load the texture

        // Attach the texture to the material
        model2.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture2;

        // Set texture scaling using a shader value
        Shader shader = LoadShader(NULL, "resources/scaling.fs"); // Load a fragment shader for scaling
        model2.materials[0].shader = shader;

        // Pass texture scaling factor to the shader
        float textureScale = 0.02f; // Scale down the texture
        SetShaderValue(shader, GetShaderLocation(shader, "textureScale"), &textureScale, SHADER_UNIFORM_FLOAT);
        isModel2Loaded = true;
    }

    // Draw the loaded model
    Vector3 modelPosition2 = { 8.7f, 0.8f, 0.0f }; // Adjust position as needed
    Vector3 modelScale2 = { 0.01f, 0.01f, 0.01f };    // Scale factor (1/5 = 0.2)
    Vector3 rotationAxis2 = { 0.0f, 0.5f, 0.5f };   // Rotation around Y-axis
    float rotationAngle2 =  sin(GetTime() * 0.5f * ( sin(GetTime() * 0.1f) + 1)) * 10.0f + 180.0f;       // Rotate over time (30 degrees per second)

    // Draw the model with rotation and scaling
    DrawModelEx(model2, modelPosition2, rotationAxis2, rotationAngle2, modelScale2, WHITE);


}
