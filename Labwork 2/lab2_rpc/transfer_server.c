/* transfer_server.c */
#include "transfer.h"
#include <stdio.h>

int * upload_file_1_svc(file_data *argp, struct svc_req *rqstp) {
    static int result;
    FILE *fp;
    char new_filename[300];

    sprintf(new_filename, "rpc_recv_%s", argp->filename);
    printf("[SERVER] Receiving file: %s (%d bytes)\n", argp->filename, argp->bytes_sent);

    fp = fopen(new_filename, "wb");
    if (fp == NULL) {
        perror("Error opening file");
        result = 0;
        return &result;
    }

    fwrite(argp->content.content_val, 1, argp->content.content_len, fp);
    fclose(fp);
    
    printf("[SERVER] Saved as %s\n", new_filename);
    result = 1;
    return &result;
}