#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define SERVER_IP "127.0.0.1" // IP Localhost
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char filename[100];
    char buffer[BUFFER_SIZE];

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed. Is the server running?\n");
        return -1;
    }

    printf("Enter filename to send: ");
    scanf("%s", filename);

    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        perror("[-] File not found");
        return 1;
    }

    fseek(fp, 0L, SEEK_END);
    long filesize = ftell(fp);
    rewind(fp);
    
    sprintf(buffer, "%s|%ld", filename, filesize);
    send(sock, buffer, sizeof(buffer), 0);

    recv(sock, buffer, BUFFER_SIZE, 0); 

    printf("[INFO] Sending file data...\n");
    while(1) {
        int bytes_read = fread(buffer, 1, BUFFER_SIZE, fp);
        if (bytes_read <= 0) break;
        send(sock, buffer, bytes_read, 0);
    }
    printf("[SUCCESS] File sent.\n");

    fclose(fp);
    close(sock);
    return 0;
}