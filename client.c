// client.c

#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>



#include "draw_tree.h"
#include "draw_chessboard.h"
#include "draw_pyramid.h"
#include "draw_prestonhouse.h"
#include "draw_prestongame.h"
#include "draw_epicgame.h"

#include "draw_willohgame.h"


#define BOARD_SIZE 32       // Number of squares per row and column
#define SQUARE_SIZE 2.0f    // Size of each square
#define PLAYER_HEIGHT 0.4f  // Player height
#define PLAYER_CAMERA_HEIGHT 1.8f  // Player view height
#define GRAVITY -9.8f       // Gravity force
#define MOVE_SPEED 4.0f     // Player movement speed
#define JUMP_FORCE 5.0f     // Jump force
#define TURN_SPEED 90.0f    // Turning speed in degrees per second
#define SERVER_PORT 12345
#define BUFFER_SIZE 1024
#define MAX_PLAYERS 10
#define TREE_COUNT 142

// Set the server IP address here
#define SERVER_IP "50.188.120.138"

typedef struct {
    Vector3 position;  // Player position
    float velocityY;   // Vertical velocity
    bool isGrounded;   // Is the player on the ground?
    float yaw;         // Horizontal angle
    float pitch;       // Vertical angle
} Player;


typedef struct {
    int id;
    Vector3 position;
} RemotePlayer;

RemotePlayer remotePlayers[MAX_PLAYERS];
int remotePlayerCount = 0;
pthread_mutex_t players_mutex = PTHREAD_MUTEX_INITIALIZER;

LollipopTree lollipopTrees[TREE_COUNT];
// Networking variables
int client_socket;
int my_id; // Store our assigned player ID


// Client to server
int type = -1;
int state = -1;
float information = -1.00f;

// From server to client
int type_fserv = -1;
int state_fserv = -1;
float information_fserv = -1.00f;

// Memory List
float memory_list[10] = {-1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f};


long double server_time;

void *receive_updates(void *args) {
    char buffer[BUFFER_SIZE];
    char recv_buffer[BUFFER_SIZE];
    int recv_buffer_len = 0;
    while (1) {
        ssize_t bytes_read = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_read <= 0) {
            printf("Disconnected from server.\n");
            break;
        }

        // Append received data to recv_buffer
        memcpy(recv_buffer + recv_buffer_len, buffer, bytes_read);
        recv_buffer_len += bytes_read;
        recv_buffer[recv_buffer_len] = '\0';

        // Process complete lines
        char *line_start = recv_buffer;
        char *newline_ptr = NULL;
        while ((newline_ptr = strchr(line_start, '\n')) != NULL) {
            *newline_ptr = '\0';

            // Process the line
            int id;
            float x, y, z;
            long double t;
            printf("Recieved from Server:  %s\n", line_start);
            if (sscanf(line_start, "%d %f %f %f %Lf %d %d %f", &id, &x, &y, &z, &t, &type_fserv, &state_fserv, &information_fserv) == 8) {
                pthread_mutex_lock(&players_mutex);
                // Update the remotePlayers array
                int found = 0;
                server_time = t;
                for (int i = 0; i < remotePlayerCount; i++) {
                    if (remotePlayers[i].id == id) {
                        remotePlayers[i].position.x = x;
                        remotePlayers[i].position.y = y;
                        remotePlayers[i].position.z = z;
                        found = 1;
                        break;
                    }
                }
                if (!found && remotePlayerCount < MAX_PLAYERS) {
                    remotePlayers[remotePlayerCount].id = id;
                    remotePlayers[remotePlayerCount].position.x = x;
                    remotePlayers[remotePlayerCount].position.y = y;
                    remotePlayers[remotePlayerCount].position.z = z;
                    remotePlayerCount++;
                }

                if (type_fserv == 0) {
                    memory_list[0] = 0.0f;
                }

                if (type_fserv == 1) {
                    memory_list[1] = 0.0f;
                }

                if (type_fserv == 2) {
                    memory_list[2] = 0.0f;
                }



                pthread_mutex_unlock(&players_mutex);
            }

            // Move to next line
            line_start = newline_ptr + 1;
        }

        // Move remaining data to the beginning of recv_buffer
        recv_buffer_len -= (line_start - recv_buffer);
        memmove(recv_buffer, line_start, recv_buffer_len);
        recv_buffer[recv_buffer_len] = '\0';
    }

    close(client_socket);
    pthread_exit(NULL);
}

int main(void) {

    float total_time = 0;
    server_time = 0.0L;
    Cat cat;
    cat.x = 0.0f;
    cat.z = 0.0f;
    // Initialize the player
    Player player = { .position = { 0.0f, PLAYER_CAMERA_HEIGHT, 10.0f }, .velocityY = 0.0f, .isGrounded = false, .yaw = 0.0f, .pitch = 0.0f };

    // Connect to the server
    struct sockaddr_in server_address;
    pthread_t thread;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);

    // Use the defined SERVER_IP constant
    if (inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr) <= 0) {
        perror("Invalid server IP address");
        exit(EXIT_FAILURE);
    }

    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Connection to server failed");
        exit(EXIT_FAILURE);
    }

    // Receive our assigned ID from the server
    char id_buffer[16];
    ssize_t id_bytes = recv(client_socket, id_buffer, sizeof(id_buffer) - 1, 0);
    if (id_bytes <= 0) {
        printf("Failed to receive ID from server.\n");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
    id_buffer[id_bytes] = '\0';
    my_id = atoi(id_buffer);
    printf("Assigned player ID: %d\n", my_id);

    printf("Connected to server.\n");

    pthread_create(&thread, NULL, receive_updates, NULL);

    // Initialize the window
    InitWindow(1200, 800, "3D Chessboard with Angled Camera Control");

    // Initialize the audio device
    InitAudioDevice();

    // Load the meow sound
    Sound meow = LoadSound("sounds/meow.wav");
    Sound meow2 = LoadSound("sounds/meow2.wav");
    Sound meow3 = LoadSound("sounds/meow3.wav");
    Sound sound1 = LoadSound("sounds/sound1.wav");
    Sound sound2 = LoadSound("sounds/sound2.wav");



    // Initialize the camera
    Camera3D camera = { 0 };
    camera.position = player.position; // Camera position
    camera.target = (Vector3){ player.position.x, player.position.y, player.position.z - 1.0f }; // Camera target
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };                // Camera up vector
    camera.fovy = 60.0f;                                      // Field of view
    camera.projection = CAMERA_PERSPECTIVE;                   // Perspective camera

    SetTargetFPS(60);

    GenerateRandomLollipopTrees(lollipopTrees, TREE_COUNT, BOARD_SIZE * SQUARE_SIZE);

    while (!WindowShouldClose()) {
        // Get frame time
        float deltaTime = GetFrameTime();
        total_time += deltaTime;

        // Apply gravity if the player is not grounded
        if (!player.isGrounded) {
            player.velocityY += GRAVITY * deltaTime;
        }

        // Update player position based on velocity
        player.position.y += player.velocityY * deltaTime;

        // Check collisions with the chessboard (ground)
        if (player.position.y <= PLAYER_CAMERA_HEIGHT) {
            player.position.y = PLAYER_CAMERA_HEIGHT;
            player.velocityY = 0.0f;
            player.isGrounded = true;
        }

        // Handle movement input
        Vector3 moveDirection = { 0.0f, 0.0f, 0.0f };
        if (IsKeyDown(KEY_W)) {
            moveDirection.x += cos(DEG2RAD * player.yaw) * MOVE_SPEED * deltaTime;
            moveDirection.z += sin(DEG2RAD * player.yaw) * MOVE_SPEED * deltaTime;
        }
        if (IsKeyDown(KEY_E)) {
            moveDirection.x += cos(DEG2RAD * player.yaw) * MOVE_SPEED * 3.0f * deltaTime;
            moveDirection.z += sin(DEG2RAD * player.yaw) * MOVE_SPEED * 3.0f * deltaTime;
        }
        if (IsKeyDown(KEY_S)) {
            moveDirection.x -= cos(DEG2RAD * player.yaw) * MOVE_SPEED * deltaTime;
            moveDirection.z -= sin(DEG2RAD * player.yaw) * MOVE_SPEED * deltaTime;
        }
        if (IsKeyDown(KEY_A)) {
            moveDirection.x += cos(DEG2RAD * (player.yaw - 90)) * MOVE_SPEED * deltaTime;
            moveDirection.z += sin(DEG2RAD * (player.yaw - 90)) * MOVE_SPEED * deltaTime;
        }
        if (IsKeyDown(KEY_D)) {
            moveDirection.x += cos(DEG2RAD * (player.yaw + 90)) * MOVE_SPEED * deltaTime;
            moveDirection.z += sin(DEG2RAD * (player.yaw + 90)) * MOVE_SPEED * deltaTime;
        }

        // Move player horizontally
        player.position.x += moveDirection.x;
        player.position.z += moveDirection.z;

        // Jumping
        if ((IsKeyPressed(KEY_SPACE) && player.isGrounded) || player.position.x > 31) {
            player.velocityY = JUMP_FORCE;
            player.isGrounded = false;
        }

        // Adjust yaw and pitch with arrow keys
        if (IsKeyDown(KEY_LEFT)) player.yaw -= TURN_SPEED * deltaTime;
        if (IsKeyDown(KEY_RIGHT)) player.yaw += TURN_SPEED * deltaTime;
        if (IsKeyDown(KEY_UP)) player.pitch += TURN_SPEED * deltaTime;
        if (IsKeyDown(KEY_DOWN)) player.pitch -= TURN_SPEED * deltaTime;

        // Limit pitch to prevent flipping
        if (player.pitch > 89.0f) player.pitch = 89.0f;
        if (player.pitch < -89.0f) player.pitch = -89.0f;

        // Update camera target based on yaw and pitch
        Vector3 forward = {
            cos(DEG2RAD * player.pitch) * cos(DEG2RAD * player.yaw),
            sin(DEG2RAD * player.pitch),
            cos(DEG2RAD * player.pitch) * sin(DEG2RAD * player.yaw)
        };

        camera.position = player.position;
        camera.target.x = player.position.x + forward.x;
        camera.target.y = player.position.y + forward.y;
        camera.target.z = player.position.z + forward.z;

        // Play the sound when the m key is pressed
        float dist_to_cat = pow(pow(cat.x - player.position.x, 2) + pow(cat.z - player.position.z, 2), 0.5);
        if (IsKeyPressed(KEY_M) || (dist_to_cat < 5.0f && (rand() % 100) == 0)) {
            int r = rand() % 3;
            if (r == 0) {
                PlaySound(meow);
            } else if (r == 1) {
                PlaySound(meow2);
            } else {
                PlaySound(meow3);
            }
        }

        if (IsKeyPressed(KEY_N)) {
            PlaySound(sound1);
        }
        if (IsKeyPressed(KEY_B)) {
            PlaySound(sound2);
        }

        if (IsKeyPressed(KEY_P)) {
            time_t current_time = time(NULL);
            char time_str[30];
            snprintf(time_str, sizeof(time_str), "%ld", current_time - 1466112000);
            char filename_string[40];
            strcpy(filename_string, "screenshots/screenshot_");
            strcat(filename_string, time_str);
            strcat(filename_string, ".png");
            TakeScreenshot(filename_string);
        }

        // Send position to server
        char buffer[BUFFER_SIZE];
        snprintf(buffer, BUFFER_SIZE, "%d %f %f %f %d %d %f\n", my_id, player.position.x, player.position.y, player.position.z, type, state, information);
        printf("Broadcasting to Server:  %s\n", buffer);
        send(client_socket, buffer, strlen(buffer), 0);

        // Start drawing
        BeginDrawing();
        ClearBackground(SKYBLUE);

        BeginMode3D(camera);



        int type_temp = -1;    // Allows each function to have a shot to return its type
        int r_send = rand() % 3;  // Decides which function will pass its message

        // Call all the external functions to draw shapes
        DrawChessboard(BOARD_SIZE, SQUARE_SIZE);
        DrawPyramid();
        // Draw the lollipop trees
        cat = DrawPrestonhouse(server_time);
        DrawLollipopTrees(lollipopTrees, TREE_COUNT);
        type_temp = DrawPrestongame(remotePlayers, type_fserv, state_fserv, information_fserv, memory_list);
        if (type_temp >= 0 && r_send == 0) {
            type = type_temp;
        }

        type_temp = DrawEpicgame(remotePlayers, type_fserv, state_fserv, information_fserv, memory_list); //Replace with Preston's first two-way game
        if (type_temp >= 0 && r_send == 1) {
            type = type_temp;
        }



        type_temp = DrawPrestongame(remotePlayers, type_fserv, state_fserv, information_fserv, memory_list); //Replace with WIlloh's first two-way game
        if (type_temp >= 0 && r_send == 2) {
            type = type_temp;
        }



        printf("Memory:  %f  %f  %f  %f\n", memory_list[0], memory_list[1], memory_list[2], memory_list[3]);









        if (type == 0) {
            if (memory_list[0] < -0.4f && memory_list[0] > -0.6f) { //player close
                state = 0;
                information = 1.0f;
            } else if (memory_list[0] == -0.1f) {  // Player far
                state = 0;
                information = -1.0f;
            }
        }



        if (type == 1) {
            if (memory_list[1] < -0.4f && memory_list[1] > -0.6f) { //player close
                state = 0;
                information = 1.0f;
            } else if (memory_list[1] == -0.1f) {  // Player far
                state = 0;
                information = -1.0f;
            }
        }


        if (type == 2) {
            if (memory_list[2] < -0.4f && memory_list[2] > -0.6f) { //player close
                state = 0;
                information = 1.0f;
            } else if (memory_list[2] == -0.1f) {  // Player far
                state = 0;
                information = -1.0f;
            }
        }





        DrawWillohgame();

        // Draw other players
        pthread_mutex_lock(&players_mutex);
        Color playercolor;
        for (int i = 0; i < remotePlayerCount; i++) {
            /*
            // Skip rendering ourselves
            if (remotePlayers[i].id == my_id) {
                continue;
            }
            */

            // Draw other players as short rectangular prisms
            switch (remotePlayers[i].id % 5) {
                case 0:
                    playercolor = RED;
                    break;
                case 1:
                    playercolor = BLUE;
                    break;
                case 2:
                    playercolor = LIME;
                    break;
                case 3:
                    playercolor = MAGENTA;
                    break;
                case 4:
                    playercolor = (Color) {255, 255, 0, 100};
                    break;
            }
            Vector3 otherPlayerPosition = remotePlayers[i].position;
            DrawCube((Vector3){otherPlayerPosition.x, otherPlayerPosition.y - 1.6f, otherPlayerPosition.z}, 0.9f, PLAYER_HEIGHT, 0.9f, playercolor);
        }
        pthread_mutex_unlock(&players_mutex);

        EndMode3D();

        // Draw instructions
        DrawText("Move with [WASD], look with [Arrow Keys], jump with [SPACE]", 10, 10, 20, RED);
        DrawText("Press [ESC] to exit", 10, 40, 20, RED);
        char buffer_text[500];
        sprintf(buffer_text, "ID: %d   X: %f   Y: %f   Z: %f   Time: %f", my_id, player.position.x, player.position.y, player.position.z, total_time);
        DrawText(buffer_text, 10, 70, 20, RED);
        sprintf(buffer_text, "Server Time: %Lf", server_time);
        DrawText(buffer_text, 10, 100, 20, RED);

        EndDrawing();
    }

    // Unload the sound and close the audio device
    UnloadSound(meow);
    UnloadSound(meow2);
    UnloadSound(meow3);
    UnloadSound(sound1);
    UnloadSound(sound2);
    CloseAudioDevice();

    CloseWindow();
    close(client_socket);
    return 0;
}
