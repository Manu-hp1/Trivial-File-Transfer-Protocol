#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h> /* For waitpid() */
#include <fcntl.h>
#include <errno.h>
#include "tftp.h"

int main()
{
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t len = sizeof(client_addr);
    int queue_number = 0; /* Track how many clients have been served */

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
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("[\033[1;91mERROR\033[1;97m] Bind failed on port %d: %s\033[1;97m\n", PORT, strerror(errno));
        exit(1);
    }
    printf("[\033[1;33mBIND\033[1;97m] Socket bound to port %d\n", PORT);
    printf("[\033[38;5;51mTFTP\033[1;97m] [\033[38;2;0;255;255mSERVER\033[1;97m] ...\n");

    /*
     * QUEUE DESIGN — First Come First Served:
     *
     *  Client 1 arrives → server handles it FULLY → done
     *  Client 2 arrives → server handles it FULLY → done
     *  Client 3 arrives → server handles it FULLY → done
     *
     *  While Client 1 is being served:
     *  - Client 2's initial packet lands in the kernel UDP buffer
     *  - Kernel holds it automatically (UDP socket buffer)
     *  - When server calls recvfrom() after Client 1 is done,
     *    it picks up Client 2's buffered packet
     *  = Natural FIFO queue — no extra code needed!
     *
     *  Why this works:
     *  UDP sockets have a kernel receive buffer (default ~212KB)
     *  Any packets sent while server is busy are held in this buffer
     *  Server picks them up one by one after each client is done
     */

    while (1)
    {
        printf("\033[1;97m◄■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■►\n");

        tftp_packet pkt, ack;
        memset(&pkt, 0, sizeof(pkt));
        memset(&ack, 0, sizeof(ack));

        /* Step 1: Block here — wait for next client packet from queue */
        printf("[\033[38;5;51mQUEUE\033[1;97m] Waiting for client... (served %d so far)\n", queue_number);

        recvfrom(sockfd, &pkt, sizeof(pkt), 0, (struct sockaddr *)&client_addr, &len);
        pkt.opcode = ntohs(pkt.opcode);

        queue_number++;
        printf("[\033[1;36mCLIENT\033[1;97m] Client #%d → %s\n", queue_number, inet_ntoa(client_addr.sin_addr));

        /* Step 2: Handle this client FULLY before accepting next one */
        if (pkt.opcode == WRQ)
        {
            printf("[\033[1;35mSERVING\033[1;97m] Client #%d — PUT request\n", queue_number);
            receive_file(sockfd, client_addr, len, &pkt, &ack);
            printf("[\033[1;92mDONE\033[1;97m] Client #%d PUT completed — next client will be served\n", queue_number);
        }
        else if (pkt.opcode == RRQ)
        {
            printf("[\033[1;35mSERVING\033[1;97m] Client #%d — GET request\n", queue_number);
            send_file(sockfd, client_addr, len, &pkt, &ack);
            printf("[\033[1;92mDONE\033[1;97m] Client #%d GET completed — next client will be served\n", queue_number);
        }
        else
        {
            printf("[\033[1;91mERROR\033[1;97m] Unknown opcode %d from client #%d\n",
                   pkt.opcode, queue_number);
        }

        /* Step 3: Loop back — picks up next waiting client from UDP buffer */
    }

    close(sockfd);
    return 0;
}
