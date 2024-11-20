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
    Vector3 headPosition = { tree.position.x, tree.position.y + tree.stickHeight + tree.headRadius, tree.position.z };
    DrawSphere(headPosition, tree.headRadius, tree.headColor);
    DrawSphereWires(headPosition, tree.headRadius, 16, 16, GRAY);

    // Draw the lollipop ring
    float ringRadius = tree.headRadius * 1.1f;
    float ringHeight = 0.2f;
    Vector3 ringPosition = { tree.position.x, tree.position.y + tree.stickHeight + tree.headRadius, tree.position.z };
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
static const Color LOLLIPOP_COLOR_LIST[] = {
    // Primary fruity flavors
    (Color){255, 0, 0, 150},     // Red - Cherry
    (Color){255, 69, 0, 150},    // Orange - Orange
    (Color){255, 165, 0, 150},   // Bright Orange - Peach Mango
    (Color){255, 255, 0, 150},   // Yellow - Lemon
    (Color){0, 255, 0, 150},     // Green - Lime
    (Color){0, 128, 0, 150},     // Dark Green - Green Apple
    (Color){0, 255, 255, 150},   // Cyan - Blue Raspberry
    (Color){0, 0, 255, 150},     // Blue - Grape
    (Color){75, 0, 130, 150},    // Indigo - Blackberry
    (Color){238, 130, 238, 150}, // Violet - Mixed Berry
    (Color){255, 20, 147, 150},  // Deep Pink - Watermelon
    (Color){255, 105, 180, 150}, // Hot Pink - Strawberry Shortcake

    // Creamy and dessert flavors
    (Color){255, 222, 173, 150}, // Navajo White - Caramel
    (Color){210, 105, 30, 150},  // Chocolate - Root Beer
    (Color){255, 239, 213, 150}, // Papaya Whip - Cotton Candy
    (Color){255, 248, 220, 150}, // Cornsilk - Cream Soda
    (Color){240, 230, 140, 150}, // Khaki - Banana Split

    // Exotic and fun mixes
    (Color){173, 216, 230, 150}, // Light Blue - Bubble Gum
    (Color){255, 182, 193, 150}, // Light Pink - Pink Lemonade
    (Color){255, 99, 71, 150},   // Tomato - Tropical Punch
    (Color){255, 215, 0, 150},   // Gold - Pineapple
    (Color){160, 82, 45, 150},   // Sienna - Chocolate Fudge
    (Color){255, 240, 245, 150}, // Lavender Blush - Vanilla Cream

    // Seasonal and limited edition
    (Color){0, 128, 128, 150},   // Teal - Wintergreen
    (Color){199, 21, 133, 150},  // Medium Violet Red - Raspberry Lemonade
    (Color){255, 228, 225, 150}, // Misty Rose - Birthday Cake
    (Color){255, 192, 203, 150}, // Pink - Peppermint Swirl
    (Color){220, 20, 60, 150},   // Crimson - Pomegranate
};


static const int LOLLIPOP_COLOR_COUNT = sizeof(LOLLIPOP_COLOR_LIST) / sizeof(LOLLIPOP_COLOR_LIST[0]);

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
