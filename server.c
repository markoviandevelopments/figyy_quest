// server.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>



#include "alter_prestongame.h"
#include "alter_epicgame.h"
#include "alter_bouncer.h"



#define PORT 12345
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024
#define BROADCAST_INTERVAL 10000000L // nanoseconds (10 ms)
#define PLAYER_HEIGHT 0.2f

typedef struct {
    int id;           // Player ID
    float x, y, z;    // Player position
} Player;

typedef struct {
    int socket;
    Player player;
} Client;


float get_current_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts); // Get current time in seconds and nanoseconds

    // Convert seconds and nanoseconds to a float
    return (long double)ts.tv_sec + (long double)ts.tv_nsec / 1e9L;
}


long double time_ref;
long double elapsed_time;



// To client
int type_tclient = -1;
int state_tclient = -1;
float information_tclient = -1.00f;


// From client (Willoh shoutout)
int type;
int state;
float information;


int iteration2 = 0;
int r;

// Memory List
float memory_list[10] = {-1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f};

// Change clients array to store pointers
Client *clients[MAX_CLIENTS];
int client_count = 0;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;


// Function to broadcast positions to all clients
void broadcast_info() {
    char buffer[BUFFER_SIZE];
    int offset = 0;

    pthread_mutex_lock(&clients_mutex);

    elapsed_time = get_current_time() - time_ref;



    // SECTION FOR HANDLING THE STATE OF OBJECTS AND THE BROADCASTING OF STATE INFORMATION TO CLIENTS

    srand(iteration2);
    r = rand() % 4;
    iteration2++;

    if (r == 0) {
        if (memory_list[0] <= -2.0f && memory_list[0] > -202.0f) {  // TO HANDLE alter_prestongame.h
            memory_list[0] -= 1.0f;
            type_tclient = 0;
            state_tclient = 0;
            information_tclient = (rand() % 1000 ) / 1000.0f * 5;
        } else {
            type_tclient = 0;
            state_tclient = 0;
            information_tclient = -1.0f;

        }
    }



    if (r == 1) {
        if (memory_list[1] <= -2.0f && memory_list[1] > -202.0f) {  // For Preston's first two-way game
            memory_list[1] -= 1.0f;
            type_tclient = 1;
            state_tclient = 0;
            information_tclient = (rand() % 1000 ) / 1000.0f * 5;
        } else {
            type_tclient = 1;
            state_tclient = 0;
            information_tclient = -1.0f;

        }
    }

    if (r == 2) {
        if (memory_list[2] <= -2.0f && memory_list[2] > -202.0f) {  // For Willoh's first two-way game
            memory_list[2] -= 1.0f;
            type_tclient = 2;
            state_tclient = 0;
            information_tclient = (rand() % 1000 ) / 1000.0f * 5;
        } else {
            type_tclient = 2;
            state_tclient = 0;
            information_tclient = -1.0f;

        }
    }



    AlterBouncer(memory_list);

    if (r == 3) {
        int r2 = rand() % 4;

        if (r2 == 0) {
            if (memory_list[4] > -1) {  // For Bouncer
                type_tclient = 3;
                state_tclient = 0;
                information_tclient = memory_list[4];
            } else {
                type_tclient = 3;
                state_tclient = 0;
                information_tclient = -1.0f;
            }
        }

        if (r2 == 1) {
            if (memory_list[5] > -1) {
                type_tclient = 3;
                state_tclient = 1;
                information_tclient = memory_list[5];
            } else {
                type_tclient = 3;
                state_tclient = 1;
                information_tclient = -1.0f;
            }
        }

        if (r2 == 2) {
            if (memory_list[6] > -1) {
                type_tclient = 3;
                state_tclient = 2;
                information_tclient = memory_list[6];
            } else {
                type_tclient = 3;
                state_tclient = 2;
                information_tclient = -1.0f;
            }
        }

        if (r2 == 3) {
            if (memory_list[7] > -1) {
                type_tclient = 3;
                state_tclient = 3;
                information_tclient = memory_list[7];
            } else {
                type_tclient = 3;
                state_tclient = 3;
                information_tclient = -1.0f;
            }
        }



    }





    // Serialize all information
    for (int i = 0; i < client_count; i++) {
        offset += snprintf(buffer + offset, BUFFER_SIZE - offset, "%d %f %f %f %Lf %d %d %f\n",
                           clients[i]->player.id, clients[i]->player.x, clients[i]->player.y, clients[i]->player.z, elapsed_time,
                           type_tclient, state_tclient, information_tclient);
    }

    // Debugging: Print the data being sent to clients
    //printf("Broadcasting to clients:  %s\n", buffer);

    // Send the serialized positions to all connected clients
    for (int i = 0; i < client_count; i++) {
        send(clients[i]->socket, buffer, strlen(buffer), 0);
    }

    pthread_mutex_unlock(&clients_mutex);
}

// Thread function to periodically broadcast positions
void *broadcast_thread(void *arg) {
    while (1) {
        struct timespec req;
        req.tv_sec = 0;           // 0 seconds
        req.tv_nsec = BROADCAST_INTERVAL; // In nanoseconds

        nanosleep(&req, NULL);
        broadcast_info();
    }
    return NULL;
}

void *handle_client(void *arg) {
    Client *client = (Client *)arg;
    char buffer[BUFFER_SIZE];
    char recv_buffer[BUFFER_SIZE];
    int recv_buffer_len = 0;
    ssize_t bytes_read;


    // Send the assigned ID to the client
    char id_buffer[16];
    sprintf(id_buffer, "%d\n", client->player.id);
    send(client->socket, id_buffer, strlen(id_buffer), 0);

    while ((bytes_read = recv(client->socket, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        // Append received data to recv_buffer
        memcpy(recv_buffer + recv_buffer_len, buffer, bytes_read);
        recv_buffer_len += bytes_read;
        recv_buffer[recv_buffer_len] = '\0';


        //printf("Recieved from client:  %s\n", buffer);

        // Process complete lines
        char *line_start = recv_buffer;
        char *newline_ptr = NULL;
        while ((newline_ptr = strchr(line_start, '\n')) != NULL) {
            *newline_ptr = '\0';

            // Process the line
            int id;
            float x, y, z;
            if (sscanf(line_start, "%d %f %f %f %d %d %f", &id, &x, &y, &z, &type, &state, &information) == 7) {
                pthread_mutex_lock(&clients_mutex);
                client->player.x = x;
                client->player.y = y;
                client->player.z = z;

                if (type == 0) {  // Handle alter_prestongame.h (uses memory slot 0)
                    AlterPrestongame(type, state, information, memory_list);
                }

                if (type == 1) {  // Handle Preston's first two (uses memory slot 1)
                    AlterEpicgame(type, state, information, memory_list);
                }

                if (type == 2) {  // Handle WIll
                    AlterPrestongame(type, state, information, memory_list);
                }



                //printf("Memory:  %f  %f  %f  %f\n", memory_list[0], memory_list[1], memory_list[2], memory_list[3]);

                pthread_mutex_unlock(&clients_mutex);
            }

            // Move to next line
            line_start = newline_ptr + 1;
        }

        // Move remaining data to the beginning of recv_buffer
        recv_buffer_len -= (line_start - recv_buffer);
        memmove(recv_buffer, line_start, recv_buffer_len);
        recv_buffer[recv_buffer_len] = '\0';
    }

    // Client disconnected
    close(client->socket);

    // Remove client from clients array
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (clients[i]->player.id == client->player.id) {
            clients[i] = clients[client_count - 1];
            client_count--;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    printf("Client %d disconnected.\n", client->player.id);
    free(client);
    pthread_exit(NULL);
}

int main() {
    int server_socket, new_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t addr_len = sizeof(client_address);
    time_ref = get_current_time();
    printf("Server started at epoch time: %.12Lf\n", time_ref);
    elapsed_time = 0;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    int optval = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY; // Listen on all interfaces

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // Start the broadcast thread
    pthread_t broadcast_tid;
    pthread_create(&broadcast_tid, NULL, broadcast_thread, NULL);

    while ((new_socket = accept(server_socket, (struct sockaddr *)&client_address, &addr_len))) {
        if (client_count >= MAX_CLIENTS) {
            printf("Max clients reached. Connection rejected.\n");
            close(new_socket);
            continue;
        }

        Client *new_client = malloc(sizeof(Client));
        new_client->socket = new_socket;
        new_client->player.id = new_socket; // Use socket fd as unique ID
        new_client->player.x = 0.0f;
        new_client->player.y = PLAYER_HEIGHT; // Default height for players
        new_client->player.z = 10.0f;

        pthread_mutex_lock(&clients_mutex);
        clients[client_count++] = new_client; // Store pointer in clients array
        pthread_mutex_unlock(&clients_mutex);

        pthread_t thread;
        pthread_create(&thread, NULL, handle_client, (void *)new_client);
        pthread_detach(thread);

        printf("Client %d connected.\n", new_client->player.id);
    }

    close(server_socket);
    return 0;
}
