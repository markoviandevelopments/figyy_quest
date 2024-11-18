#ifndef DRAW_TREE_H
#define DRAW_TREE_H

#include "raylib.h"
#include <stdlib.h> // For rand()

// Tree properties structure
typedef struct {
    Vector3 position;
    float stickHeight;
    float stickRadius;
    float headRadius;
    Color stickColor; // Color for the lollipop stick
    Color headColor;  // Color for the lollipop head and ring
} LollipopTree;

// Function to draw a single lollipop tree
static inline void DrawLollipopTree(LollipopTree tree) {
    // Draw the lollipop stick
    Vector3 stickPosition = { tree.position.x, tree.position.y + tree.stickHeight / 2, tree.position.z };
    DrawCube(stickPosition, tree.stickRadius, tree.stickHeight, tree.stickRadius, tree.stickColor); // White stick
    DrawCubeWires(stickPosition, tree.stickRadius, tree.stickHeight, tree.stickRadius, GRAY);       // Gray edges

    // Draw the lollipop head
    Vector3 headPosition = { tree.position.x, tree.position.y - 0.5 * tree.headRadius + tree.stickHeight + tree.headRadius, tree.position.z };
    DrawSphere(headPosition, tree.headRadius, tree.headColor);

    // Draw the lollipop ring
    float ringRadius = tree.headRadius * 1.1f;
    float ringHeight = 0.2f;
    Vector3 ringPosition = { tree.position.x, tree.position.y - 0.5 * tree.headRadius + tree.stickHeight + tree.headRadius, tree.position.z };
    DrawCylinder(ringPosition, ringRadius, ringRadius, ringHeight, 16, tree.headColor);
}

// Function to draw multiple lollipop trees
static inline void DrawLollipopTrees(LollipopTree *trees, int treeCount) {
    for (int i = 0; i < treeCount; i++) {
        DrawLollipopTree(trees[i]);
    }
}

// Function to generate random lollipop trees on the board
static inline void GenerateRandomLollipopTrees(LollipopTree *trees, int treeCount, float boardSize, float squareSize) {
    unsigned int seed = 69;    // Default seed for consistency
    unsigned char alpha = 150;  // Default transparency value



    srand(seed); // Set the seed for reproducibility

    for (int i = 0; i < treeCount; i++) {
        trees[i].position.x = (rand() % (int)(boardSize)) - (boardSize / 2);
        trees[i].position.y = 0.2f; // Minimum height for stick base
        trees[i].position.z = (rand() % (int)(boardSize)) - (boardSize / 2);
        trees[i].stickHeight = 1.5f + (rand() % 10) * 0.1f; // Randomize stick height
        trees[i].stickRadius = 0.1f; // Thin stick
        trees[i].headRadius = 0.5f + (rand() % 5) * 0.1f; // Randomize head size
        trees[i].stickColor = WHITE;
        trees[i].headColor = (Color){
            rand() % 256, // Red
            rand() % 256, // Green
            rand() % 256, // Blue
            alpha          // Transparency
        };
    }
}

#endif // DRAW_TREE_H
