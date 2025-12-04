/* transfer_client.c */
#include "transfer.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

void transfer_prog_1(char *host, char *filename) {
    CLIENT *clnt;
    int  *result_1;
    file_data  upload_file_1_arg;
    FILE *fp;
    long file_size;
    char *file_buffer;

    // 1. Tạo kết nối RPC
    clnt = clnt_create(host, TRANSFER_PROG, TRANSFER_VERS, "udp");
    if (clnt == NULL) {
        clnt_pcreateerror(host);
        exit(1);
    }

    // 2. Đọc file local
    fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("Error: File %s not found.\n", filename);
        return;
    }

    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    rewind(fp);

    file_buffer = (char *)malloc(file_size);
    fread(file_buffer, 1, file_size, fp);
    fclose(fp);

    // 3. Đóng gói dữ liệu
    upload_file_1_arg.filename = filename;
    upload_file_1_arg.bytes_sent = file_size;
    upload_file_1_arg.content.content_val = file_buffer;
    upload_file_1_arg.content.content_len = file_size;

    printf("[CLIENT] Sending %s (%ld bytes) to %s...\n", filename, file_size, host);

    // 4. Gọi hàm RPC
    result_1 = upload_file_1(&upload_file_1_arg, clnt);
    if (result_1 == NULL) {
        clnt_perror(clnt, "call failed");
    } else if (*result_1 == 1) {
        printf("[SUCCESS] File transferred successfully.\n");
    } else {
        printf("[FAIL] Server reported error.\n");
    }

    free(file_buffer);
    clnt_destroy(clnt);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("usage: %s <server_host> <filename>\n", argv[0]);
        exit(1);
    }
    transfer_prog_1(argv[1], argv[2]);
    exit(0);
}