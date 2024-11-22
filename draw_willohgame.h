#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

extern const Color LOLLIPOP_COLOR_LIST[];
extern const int LOLLIPOP_COLOR_COUNT;

extern LollipopTree lollipopTrees[];

int iteration = 0;

Color GetInverseColor(Color color) {
    Color inverseColor = {
        255 - color.r,
        255 - color.g,
        255 - color.b,
        color.a
    };
    return inverseColor;
}

void DrawWillohgame() {
    static Vector3 blockPosition_new;
    static Color blockColor;

    if (iteration % 100 == 0) {
        int lilTreeIndex = rand() % 142;
        LollipopTree lilTree = lollipopTrees[lilTreeIndex];

        blockPosition_new = (Vector3){
            lilTree.position.x,
            lilTree.position.y + lilTree.stickHeight - 0.3f, 
            lilTree.position.z
        };

        blockColor = GetInverseColor(lilTree.headColor);
    }

    printf("iteration %d blockPosition: %f %f %f\n",iteration, blockPosition_new.x, blockPosition_new.y, blockPosition_new.z);

    // Draw the square as a cube
    DrawCube(blockPosition_new, 0.4f, 0.4f, 0.4f, blockColor);
    DrawCubeWires(blockPosition_new, 0.4f, 0.4f, 0.4f, DARKGRAY);

    iteration++;
}
