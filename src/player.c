#include "player.h"
#include <math.h>

void init_player(Player *player) {
    player->position = (Vector3){ 0.0f, PLAYER_CAMERA_HEIGHT, 10.0f };
    player->velocityY = 0.0f;
    player->isGrounded = false;
    player->yaw = 0.0f;
    player->pitch = 0.0f;
}

void update_player(Player *player, float deltaTime) {
    // Apply gravity if the player is not grounded
    if (!player->isGrounded) {
        player->velocityY += GRAVITY * deltaTime;
    }

    // Update player position based on velocity
    player->position.y += player->velocityY * deltaTime;

    // Check collisions with the ground
    if (player->position.y <= PLAYER_CAMERA_HEIGHT) {
        player->position.y = PLAYER_CAMERA_HEIGHT;
        player->velocityY = 0.0f;
        player->isGrounded = true;
    }
}

void handle_input(Player *player, float deltaTime) {
    Vector3 moveDirection = { 0.0f, 0.0f, 0.0f };

    // Movement keys
    if (IsKeyDown(KEY_W)) {
        moveDirection.x += cos(DEG2RAD * player->yaw) * MOVE_SPEED * deltaTime;
        moveDirection.z += sin(DEG2RAD * player->yaw) * MOVE_SPEED * deltaTime;
    }
    if (IsKeyDown(KEY_S)) {
        moveDirection.x -= cos(DEG2RAD * player->yaw) * MOVE_SPEED * deltaTime;
        moveDirection.z -= sin(DEG2RAD * player->yaw) * MOVE_SPEED * deltaTime;
    }
    if (IsKeyDown(KEY_A)) {
        moveDirection.x += cos(DEG2RAD * (player->yaw - 90)) * MOVE_SPEED * deltaTime;
        moveDirection.z += sin(DEG2RAD * (player->yaw - 90)) * MOVE_SPEED * deltaTime;
    }
    if (IsKeyDown(KEY_D)) {
        moveDirection.x += cos(DEG2RAD * (player->yaw + 90)) * MOVE_SPEED * deltaTime;
        moveDirection.z += sin(DEG2RAD * (player->yaw + 90)) * MOVE_SPEED * deltaTime;
    }

    // Jumping
    if (IsKeyPressed(KEY_SPACE) && player->isGrounded) {
        player->velocityY = JUMP_FORCE;
        player->isGrounded = false;
    }

    // Update horizontal position
    player->position.x += moveDirection.x;
    player->position.z += moveDirection.z;

    // Adjust yaw and pitch with arrow keys
    if (IsKeyDown(KEY_LEFT)) player->yaw -= TURN_SPEED * deltaTime;
    if (IsKeyDown(KEY_RIGHT)) player->yaw += TURN_SPEED * deltaTime;
    if (IsKeyDown(KEY_UP)) player->pitch += TURN_SPEED * deltaTime;
    if (IsKeyDown(KEY_DOWN)) player->pitch -= TURN_SPEED * deltaTime;

    // Limit pitch to prevent flipping
    if (player->pitch > 89.0f) player->pitch = 89.0f;
    if (player->pitch < -89.0f) player->pitch = -89.0f;
}
