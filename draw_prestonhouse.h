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


Cat DrawPrestonhouse(long double server_time) {
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
                    DrawCube(position, block_w, 0.05f, block_w, squareColor); // Thin height for a flat square
                    //DrawCubeWires(position, block_w, 0.05f, block_w, BLACK);  // Outline
                }
            }
        i++;
    }

    static Model model = { 0 };
    static bool isModelLoaded = false;
    if (!isModelLoaded) {
        model = LoadModel("models/cat.obj"); // Load the model
        Texture2D texture = LoadTexture("models/cat_texture.png"); // Load the texture

        // Attach the texture to the material
        model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

        // Set texture scaling using a shader value
        Shader shader = LoadShader(NULL, "resources/scaling.fs"); // Load a fragment shader for scaling
        model.materials[0].shader = shader;

        // Pass texture scaling factor to the shader
        float textureScale = 1.0f; // Scale down the texture
        SetShaderValue(shader, GetShaderLocation(shader, "textureScale"), &textureScale, SHADER_UNIFORM_FLOAT);
        isModelLoaded = true;
    }

    // Draw the loaded model
    cat.x = 10.4f - fabs(10.0f * sin(server_time * 0.06f));
    cat.z = 0.0f  + fabs(2.8f * sin(server_time * 0.06f));
    Vector3 modelPosition = { cat.x, 0.2f, cat.z}; // Adjust position as needed
    Vector3 modelScale = { 0.03f, 0.03f, 0.03f };    // Scale factor (1/5 = 0.2)

    // Combine the two rotations into one axis and angle
    Vector3 combinedAxis = { 0.0f, 0.1f, 0.1f};
    float combinedAngle =  -180.0f; // Rotate dynamically over time

    // Draw the model using the combined axis and angle
    DrawModelEx(model, modelPosition, combinedAxis, combinedAngle, modelScale, WHITE);



    static Model model2 = { 0 };
    static bool isModel2Loaded = false;
    if (!isModel2Loaded) {
        model2 = LoadModel("models/bed.obj"); // Load the model
        Texture2D texture2 = LoadTexture("models/bed_texture.png"); // Load the texture

        // Attach the texture to the material
        model2.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture2;

        // Set texture scaling using a shader value
        Shader shader = LoadShader(NULL, "resources/scaling2.fs"); // Load a fragment shader for scaling
        model2.materials[0].shader = shader;

        // Pass texture scaling factor to the shader
        float textureScale2 = 0.02f; // Scale down the texture
        SetShaderValue(shader, GetShaderLocation(shader, "textureScale"), &textureScale2, SHADER_UNIFORM_FLOAT);
        isModel2Loaded = true;
    }

    // Draw the loaded model
    Vector3 modelPosition2 = { 11.1f, 0.8f, 0.0f }; // Adjust position as needed
    Vector3 modelScale2 = { 0.01f, 0.01f, 0.01f };    // Scale factor (1/5 = 0.2)
    Vector3 rotationAxis2 = { 0.0f, 0.5f, 0.5f };   // Rotation around Y-axis
    float rotationAngle2 =  sin(GetTime() * 0.5f * ( sin(GetTime() * 0.1f) + 1)) * 10.0f + 180.0f;       // Rotate over time (30 degrees per second)

    // Draw the model with rotation and scaling
    DrawModelEx(model2, modelPosition2, rotationAxis2, rotationAngle2, modelScale2, WHITE);



    static Model model3 = { 0 };
    static bool isModel3Loaded = false;
    if (!isModel3Loaded) {
        model3 = LoadModel("models/penguin.obj"); // Load the model
        Texture2D texture3 = LoadTexture("models/penguin_texture.png"); // Load the texture

        // Attach the texture to the material
        model3.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture3;

        // Set texture scaling using a shader value
        Shader shader3 = LoadShader(NULL, "resources/scaling2.fs"); // Load a fragment shader for scaling
        model3.materials[0].shader = shader3;

        // Pass texture scaling factor to the shader
        float textureScale3 = 1.0f; // Scale down the texture
        SetShaderValue(shader3, GetShaderLocation(shader3, "textureScale"), &textureScale3, SHADER_UNIFORM_FLOAT);
        isModel3Loaded = true;
    }

    // Draw the loaded model
    Vector3 modelPosition3 = { 30.0f * sin(server_time * 0.003f), 0.2f, 30.0f * sin(server_time * 0.00124f) }; // Adjust position as needed
    Vector3 modelScale3 = { 0.01f, 0.01f, 0.01f };    // Scale factor (1/5 = 0.2)
    Vector3 rotationAxis3 = { 0.0f, 0.5f, 0.5f };   // Rotation around Y-axis
    float rotationAngle3 =  sin(GetTime() * 0.5f * ( sin(GetTime() * 0.1f) + 1)) * 10.0f + 180.0f;       // Rotate over>

    // Draw the model with rotation and scaling
    DrawModelEx(model3, modelPosition3, rotationAxis3, rotationAngle3, modelScale3, WHITE);







    static Model model4 = { 0 };
    static bool isModel4Loaded = false;

    if (!isModel4Loaded) {
        // Load the model
        model4 = LoadModel("models/water_bowl.obj"); // Ensure the model file is in the correct directory

        // Check if the model has normals
        if (model4.meshes[0].normals == NULL) {
            printf("NO NORMALS\n");
            // Optionally generate normals if needed
            // GenMeshTangents(&model4.meshes[0]); // Not necessary for flat shading
        }

        isModel4Loaded = true;
    }

    // Define model transformation parameters
    Vector3 modelPosition4 = { 9.0f, 0.3f, -1.6f };
    Vector3 modelScale4 = { 0.02f, 0.02f, 0.02f };
    Vector3 rotationAxis4 = { 1.0f, 0.0f, 0.0f };
    float rotationAngle4 = sin(GetTime() * 0.5f * (sin(GetTime() * 0.1238764287f) + 1)) * 5.0f - 90.0f;

    // Draw the model with rotation and scaling
    DrawModelEx(model4, modelPosition4, rotationAxis4, rotationAngle4, modelScale4, (Color) {50, 50, 50, 215});

    // Draw the model wireframe over it
    DrawModelWiresEx(model4, modelPosition4, rotationAxis4, rotationAngle4, modelScale4, BLACK);






    static Model model5 = { 0 };
    static bool isModel5Loaded = false;

    if (!isModel5Loaded) {
        // Load the model
        model5 = LoadModel("models/dome_house.obj"); // Ensure the model file is in the correct directory

        // Check if the model has normals
        if (model5.meshes[0].normals == NULL) {
            printf("NO NORMALS\n");
            // Optionally generate normals if needed
            // GenMeshTangents(&model4.meshes[0]); // Not necessary for flat shading
        }

        isModel5Loaded = true;
    }

    // Define model transformation parameters
    Vector3 modelPosition5 = { -10.0f, 1.5f, 20.6f };
    Vector3 modelScale5 = { 0.15f, 0.15f, 0.15f };
    Vector3 rotationAxis5 = { 1.0f, 0.0f, 0.0f };
    float rotationAngle5 = sin(GetTime() * 0.5f * (sin(GetTime() * 0.1238764287f) + 1)) * 0.01f + 90.0f;

    // Draw the model with rotation and scaling
    DrawModelEx(model5, modelPosition5, rotationAxis5, rotationAngle5, modelScale5, (Color) {20, 80, 150, 255});

    // Draw the model wireframe over it
    DrawModelWiresEx(model5, modelPosition5, rotationAxis5, rotationAngle5, modelScale5, BLACK);









    return cat;

}

