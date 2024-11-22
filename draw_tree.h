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

extern const Color LOLLIPOP_COLOR_LIST[];
extern const int LOLLIPOP_COLOR_COUNT;


// Function to draw a single lollipop tree
static inline void DrawLollipopTree(LollipopTree tree) {

    // Draw the lollipop stick
    Vector3 stickPosition = { tree.position.x, tree.position.y + tree.stickHeight / 2, tree.position.z };
    DrawCube(stickPosition, tree.stickRadius, tree.stickHeight, tree.stickRadius, tree.stickColor); // White stick
    DrawCubeWires(stickPosition, tree.stickRadius, tree.stickHeight, tree.stickRadius, GRAY);       // Gray edges

    // Draw the lollipop head
    Vector3 headPosition = { tree.position.x, tree.position.y + tree.stickHeight + tree.headRadius - 0.1f, tree.position.z };
    DrawSphere(headPosition, tree.headRadius, tree.headColor);
    DrawSphereWires(headPosition, tree.headRadius, 8, 16, GRAY);

    // Draw the lollipop ring
    float ringRadius = tree.headRadius * 1.1f;
    float ringHeight = 0.2f;
    Vector3 ringPosition = { tree.position.x, tree.position.y + tree.stickHeight + tree.headRadius - 0.2f, tree.position.z };
    DrawCylinder(ringPosition, ringRadius, ringRadius, ringHeight, 16, tree.headColor);
    DrawCylinderWires(ringPosition, ringRadius, ringRadius, ringHeight, 16, GRAY);
}

// Function to draw multiple lollipop trees
static inline void DrawLollipopTrees(LollipopTree *trees, int treeCount) {
    for (int i = 0; i < treeCount; i++) {
        DrawLollipopTree(trees[i]);
    }
}

// Custom list of lollipop colors with transparency (preconverted to raylib's Color format)
const Color LOLLIPOP_COLOR_LIST[] = {
    // Updated primary fruity flavors with brighter colors and varied transparency
    (Color){255, 0, 0, 180},      // Bright Red - Cherry
    (Color){255, 140, 0, 160},    // Brighter Orange - Tangerine
    (Color){255, 215, 0, 170},    // Gold - Pineapple
    (Color){255, 255, 0, 200},    // Lemon Yellow
    (Color){0, 255, 0, 150},      // Lime Green
    (Color){64, 224, 208, 190},   // Turquoise - Blue Raspberry
    (Color){65, 105, 225, 160},   // Royal Blue - Blueberry
    (Color){138, 43, 226, 185},   // Blue Violet - Grape
    (Color){199, 21, 255, 175},   // Electric Purple - Wild Berry
    (Color){255, 20, 147, 190},   // Vivid Pink - Watermelon
    (Color){255, 105, 180, 195},  // Light Pink - Cotton Candy

    // More exotic and fun mixes
    (Color){255, 165, 0, 190},    // Orange - Mango Tango
    (Color){173, 255, 47, 200},   // Green Yellow - Kiwi Lime

    // Expanded seasonal and limited edition
    (Color){255, 69, 0, 160},     // Orange Red - Spiced Pumpkin
    (Color){255, 215, 0, 175},    // Goldenrod - Honey Gold
    (Color){218, 112, 214, 130},  // Orchid - Spring Lavender
    (Color){219, 112, 147, 140},  // Pale Violet Red - Cherry Blossom
};



const int LOLLIPOP_COLOR_COUNT = sizeof(LOLLIPOP_COLOR_LIST) / sizeof(LOLLIPOP_COLOR_LIST[0]);

// Function to generate random lollipop trees on the board
static inline void GenerateRandomLollipopTrees(LollipopTree *trees, int treeCount, float boardSize) {
    for (int i = 0; i < treeCount; i++) {
        trees[i].position.x = (rand() % (int)(boardSize)) - (boardSize / 2);
        trees[i].position.y = 0.2f; // Minimum height for stick base
        trees[i].position.z = (rand() % (int)(boardSize)) - (boardSize / 2);
        trees[i].stickHeight = 1.5f + (rand() % 10) * 0.1f; // Randomize stick height
        trees[i].stickRadius = 0.1f;                       // Thin stick
        trees[i].headRadius = 0.5f + (rand() % 5) * 0.1f;  // Randomize head size
        trees[i].stickColor = WHITE; // White stick, fully opaque

        // Select a random color from the lollipop list
        trees[i].headColor = LOLLIPOP_COLOR_LIST[rand() % LOLLIPOP_COLOR_COUNT];
    }
}

#endif // DRAW_TREE_H
