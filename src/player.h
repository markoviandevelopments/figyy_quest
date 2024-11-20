#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

#define GRAVITY -9.8f       // Gravity force
#define MOVE_SPEED 4.0f     // Player movement speed
#define JUMP_FORCE 5.0f     // Jump force
#define TURN_SPEED 90.0f    // Turning speed in degrees per second
#define PLAYER_CAMERA_HEIGHT 1.8f  // Player view height

typedef struct {
    Vector3 position;  // Player position
    float velocityY;   // Vertical velocity
    bool isGrounded;   // Is the player on the ground?
    float yaw;         // Horizontal angle
    float pitch;       // Vertical angle
} Player;

void init_player(Player *player);
void update_player(Player *player, float deltaTime);
void handle_input(Player *player, float deltaTime);

#endif
