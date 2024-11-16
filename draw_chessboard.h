#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>


void DrawChessboard(int BOARD_SIZE, float SQUARE_SIZE) {
    for (int z = 0; z < BOARD_SIZE; z++) {
            for (int x = 0; x < BOARD_SIZE; x++) {
                // Determine square color (alternating pattern)
                Color squareColor = ((x + z) % 2 == 0) ? DARKGRAY : LIGHTGRAY;

                // Position the square
                Vector3 position = {
                    (x - BOARD_SIZE / 2) * SQUARE_SIZE,
                    0.0f,
                    (z - BOARD_SIZE / 2) * SQUARE_SIZE
                };

                // Draw the square as a cube
                DrawCube(position, SQUARE_SIZE, 0.2f, SQUARE_SIZE, squareColor); // Thin height for a flat square
                DrawCubeWires(position, SQUARE_SIZE, 0.2f, SQUARE_SIZE, BLACK);  // Outline
            }
        }



}
