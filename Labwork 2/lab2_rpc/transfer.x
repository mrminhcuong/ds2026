/* transfer.x */
struct file_data {
    string filename<256>;
    opaque content<>;      /* Dữ liệu file (binary) */
    int bytes_sent;
};

program TRANSFER_PROG {
    version TRANSFER_VERS {
        int UPLOAD_FILE(file_data) = 1;
    } = 1;
} = 0x31230000;