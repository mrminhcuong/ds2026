#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void write_file(int sockfd) {
    int n;
    char buffer[BUFFER_SIZE];
    char filename[100];
    long filesize = 0;
    long total_received = 0;

    bzero(buffer, BUFFER_SIZE);
    n = recv(sockfd, buffer, BUFFER_SIZE, 0);
    if (n <= 0) return;

    sscanf(buffer, "%[^|]|%ld", filename, &filesize);
    printf("[INFO] Receiving file: %s (%ld bytes)\n", filename, filesize);

    char new_filename[150];
    sprintf(new_filename, "recv_%s", filename);
    
    FILE *fp = fopen(new_filename, "wb");
    if (fp == NULL) {
        perror("[-] Error creating file");
        return;
    }

    send(sockfd, "OK", 2, 0);

    while (total_received < filesize) {
        bzero(buffer, BUFFER_SIZE);
        n = recv(sockfd, buffer, BUFFER_SIZE, 0);
        if (n <= 0) break;
        
        fwrite(buffer, 1, n, fp);
        total_received += n;
    }
    
    printf("[SUCCESS] File received successfully.\n");
    fclose(fp);
}

int main() {
    int server_fd, new_sock; 
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int opt = 1; 

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("[START] Server listening on port %d...\n", PORT);

    while(1) {
        // Accept connection
        if ((new_sock = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        printf("[INFO] Client connected.\n");
        
        write_file(new_sock);
        close(new_sock);
        printf("[INFO] Client disconnected. Waiting for new connection...\n");
    }
    return 0;
}