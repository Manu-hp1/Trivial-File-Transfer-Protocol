
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
#include "tftp.h"

// Forward declaration
void handle_client(int sockfd, struct sockaddr_in *client_addr, socklen_t *client_len);

int main()
{
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t len = sizeof(client_addr);
    printf("\033[1;97m◄■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■►\n");

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        printf("\033[1;91m[ERROR] Socket creation failed: %s\033[1;97m\n", strerror(errno));
        exit(1);
    }
    printf("\033[1;97m[\033[1;92mSOCKET\033[1;97m] Socket created successfully\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("[\033[1;91mERROR\033[1;97m] Bind failed on port %d: %s\033[1;97m\n", PORT, strerror(errno));
        exit(1);
    }
    printf("[\033[1;33mBIND\033[1;97m] Socket bound to port %d\n", PORT);
    printf("[\033[38;5;51mTFTP\033[1;97m] [\033[38;2;0;255;255mSERVER\033[1;97m] ...\n");
    int i = 0;
    while (1)
    {
        printf("\033[1;97m◄■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■►\n");
        handle_client(sockfd, &client_addr, &len);
    }
    // printf("\033[1;97m**************************************************************************\n");
    printf("\033[1;97m◄■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■►\n");

    close(sockfd);
    return 0;
}

/* Function to handle client requests */
void handle_client(int sockfd, struct sockaddr_in *client_addr, socklen_t *client_len)
{
    tftp_packet pkt, ack;
    /* Initialize packet and ack structures to zero */
    memset(&ack, 0, sizeof(ack));
    memset(&pkt, 0, sizeof(pkt));

    recvfrom(sockfd, &pkt, sizeof(pkt), 0, (struct sockaddr *)client_addr, client_len);
    pkt.opcode = ntohs(pkt.opcode);

    /* Determine the type of request and call the appropriate function */
    if (pkt.opcode == WRQ)
    {
        receive_file(sockfd, *client_addr, *client_len, &pkt, &ack); // Pass the socket file descriptor, client address, client address length, packet structure and ack structure to the receive_file function to receive file from the client
    }
    else if (pkt.opcode == RRQ)
    {
        send_file(sockfd, *client_addr, *client_len, &pkt, &ack); // Pass the socket file descriptor, client address, client address length, packet structure and ack structure to the send_file function to send file to the client
    }
}
