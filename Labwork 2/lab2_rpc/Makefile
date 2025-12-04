CC = gcc
CFLAGS = -I/usr/include/tirpc
LDLIBS = -ltirpc

all: client server

transfer_clnt.c transfer_svc.c transfer.h: transfer.x
	rpcgen -C transfer.x

client: transfer_client.c transfer_clnt.c transfer_xdr.c
	$(CC) $(CFLAGS) -o client transfer_client.c transfer_clnt.c transfer_xdr.c $(LDLIBS)

server: transfer_server.c transfer_svc.c transfer_xdr.c
	$(CC) $(CFLAGS) -o server transfer_server.c transfer_svc.c transfer_xdr.c $(LDLIBS)

clean:
	rm -f client server transfer_clnt.c transfer_svc.c transfer_xdr.c transfer.h