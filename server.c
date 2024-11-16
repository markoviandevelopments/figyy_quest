// server.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 12345
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024
#define BROADCAST_INTERVAL 100000000L // nanoseconds
#define PLAYER_HEIGHT 1.8f

typedef struct {
    int id;           // Player ID
    float x, y, z;    // Player position
} Player;

typedef struct {
    int socket;
    Player player;
} Client;

// Change clients array to store pointers
Client *clients[MAX_CLIENTS];
int client_count = 0;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

// Function to broadcast positions to all clients
void broadcast_positions() {
    char buffer[BUFFER_SIZE];
    int offset = 0;

    pthread_mutex_lock(&clients_mutex);

    // Serialize all player positions
    for (int i = 0; i < client_count; i++) {
        offset += snprintf(buffer + offset, BUFFER_SIZE - offset, "%d %f %f %f\n",
                           clients[i]->player.id, clients[i]->player.x, clients[i]->player.y, clients[i]->player.z);
    }

    // Debugging: Print the data being sent to clients
    printf("Broadcasting to clients:\n%s\n", buffer);

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
        broadcast_positions();
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

        // Process complete lines
        char *line_start = recv_buffer;
        char *newline_ptr = NULL;
        while ((newline_ptr = strchr(line_start, '\n')) != NULL) {
            *newline_ptr = '\0';

            // Process the line
            float x, y, z;
            if (sscanf(line_start, "%f %f %f", &x, &y, &z) == 3) {
                pthread_mutex_lock(&clients_mutex);
                client->player.x = x;
                client->player.y = y;
                client->player.z = z;
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
