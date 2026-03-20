/* Common file for server & client */

#include "tftp.h"

/* Function to send file to client (RRQ handler) */
void send_file(int sockfd, struct sockaddr_in client_addr, socklen_t client_len, tftp_packet *pkt, tftp_packet *ack)
{
    if (pkt->mode == DEFAULT)
        printf("[\033[1;35mMODE\033[1;97m] : DEFAULT\n");
    else if (pkt->mode == OCTET)
        printf("[\033[1;35mMODE\033[1;97m] : OCTET\n");
    else if (pkt->mode == NETASCII)
        printf("[\033[1;35mMODE\033[1;97m] : NETASCII\n");

    int fd = open(pkt->body.request.filename, O_RDONLY);
    if (fd < 0)
    {
        printf("[\033[1;91mERROR\033[1;97m] Failed to open file '%s': %s\n", pkt->body.request.filename, strerror(errno));
        ack->opcode = htons(ERROR);
        sendto(sockfd, ack, sizeof(*ack), 0, (struct sockaddr *)&client_addr, client_len);
        return;
    }

    off_t size2 = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    uint16_t block = 1;
    ssize_t n;

    /* FIX 1: Default data_size to 512 not 1
       Original defaulted to 1 — caused 1 byte/block even in DEFAULT mode */
    int data_size = 512;

    if (pkt->mode == DEFAULT)
        data_size = 512;
    else if (pkt->mode == OCTET)
        data_size = 1;
    else if (pkt->mode == NETASCII)
        data_size = 512;

    /* Store client address to verify ACKs come from correct client */
    struct sockaddr_in ack_addr;
    socklen_t ack_len = sizeof(ack_addr);

    while ((n = read(fd, pkt->body.data.data, data_size)) > 0)
    {
        pkt->opcode = htons(DATA);
        pkt->body.data.block = htons(block);
        pkt->body.data.size = htons(n);
        pkt->body.data.data_size = htons((int)size2);

        sendto(sockfd, pkt, sizeof(*pkt), 0, (struct sockaddr *)&client_addr, client_len);
        printf("[\033[1;94mDATA\033[1;97m] DATA sent | Block = %u | Size = %lu bytes\n", ntohs(pkt->body.data.block), n);

        /* FIX 2: Use recvfrom with ack_addr — ONLY accept ACK from OUR client
           Original used &client_addr which could receive from any client
           causing packet mixing when multiple clients connect simultaneously */
        do
        {
            recvfrom(sockfd, ack, sizeof(*ack), 0, (struct sockaddr *)&ack_addr, &ack_len);
            /* Discard packets from other clients — only accept from our client */
        } while (ack_addr.sin_addr.s_addr != client_addr.sin_addr.s_addr || ack_addr.sin_port != client_addr.sin_port);

        printf("[\033[1;92mACK\033[1;97m] ACK received for block %u\n", ntohs(ack->body.ack.block));

        block++;
    }

    /* FIX 3: Send EOF for ALL modes including OCTET
       Original only sent EOF for DEFAULT and NETASCII
       OCTET mode never got EOF signal = client looped forever! */
    pkt->opcode = htons(DATA);
    pkt->body.data.block = htons(block);
    pkt->body.data.size = htons(0); /* 0 bytes = EOF signal */
    sendto(sockfd, pkt, sizeof(*pkt), 0, (struct sockaddr *)&client_addr, client_len);
    printf("[\033[1;92mACK\033[1;97m] EOF signal sent for block %u\n", ntohs(pkt->body.data.block));

    printf("[\033[38;5;208mFILE\033[1;97m] SENDING FILE COMPLETED\n");
    close(fd);
}

/* Function to receive file from client (WRQ handler) */
void receive_file(int sockfd, struct sockaddr_in client_addr, socklen_t client_len, tftp_packet *pkt, tftp_packet *ack)
{
    if (pkt->mode == DEFAULT)
        printf("[\033[1;35mMODE\033[1;97m] : DEFAULT\n");
    else if (pkt->mode == OCTET)
        printf("[\033[1;35mMODE\033[1;97m] : OCTET\n");
    else if (pkt->mode == NETASCII)
        printf("[\033[1;35mMODE\033[1;97m] : NETASCII\n");

    int fd = open(pkt->body.request.filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);

    ack->opcode = htons(ACK);
    ack->body.ack.block = htons(0);
    ack->body.ack.data_size = 0;
    sendto(sockfd, ack, sizeof(*ack), 0, (struct sockaddr *)&client_addr, client_len);
    printf("[\033[1;92mACK\033[1;97m] : ACK 0 sent — ready to receive\n");

    int count = 1;

    /* FIX 4: Use separate addr for recvfrom — only accept from OUR client
       Same fix as send_file — discard packets from other clients */
    struct sockaddr_in data_addr;
    socklen_t data_len = sizeof(data_addr);

    while (1)
    {
        do
        {
            recvfrom(sockfd, pkt, sizeof(*pkt), 0, (struct sockaddr *)&data_addr, &data_len);
            /* Discard packets not from our client */
        } while (data_addr.sin_addr.s_addr != client_addr.sin_addr.s_addr || data_addr.sin_port != client_addr.sin_port);

        /* Check for EOF signal */
        if (ntohs(pkt->opcode) == ERROR || (ntohs(pkt->opcode) == DATA && ntohs(pkt->body.data.size) == 0))
        {
            printf("[\033[38;2;255;105;180mEOF\033[1;97m] : End of file received\n");
            break;
        }

        uint16_t size = ntohs(pkt->body.data.size);
        printf("[\033[1;94mDATA\033[1;97m] : DATA received | Block = %u | Size = %u bytes\n", ntohs(pkt->body.data.block), size);

        if (pkt->mode == NETASCII)
        {
            if (pkt->Client_OS == WINDOWS && pkt->Server_OS == LINUX)
                netascii_linux(fd, pkt, size);
            else if (pkt->Client_OS == LINUX && pkt->Server_OS == WINDOWS)
                netascii_windows(fd, pkt, size);
            else
                write(fd, pkt->body.data.data, size);
        }
        else
        {
            write(fd, pkt->body.data.data, size);
        }

        ack->opcode = htons(ACK);
        ack->body.ack.block = pkt->body.data.block;
        ack->body.ack.data_size = size;
        count++;
        sendto(sockfd, ack, sizeof(*ack), 0, (struct sockaddr *)&client_addr, client_len);
        printf("[\033[1;92mACK\033[1;97m] : ACK sent for block %u\n", ntohs(ack->body.ack.block));

        if (count > ntohs(pkt->body.data.data_size))
        {
            printf("[\033[1;91mDATA\033[1;97m] : SIZE is less that DATA_SIZE\n");
            break;
        }
    }
    printf("[\033[38;5;208mFILE\033[1;97m] : SENDING FILE COMPLETED\n");
    close(fd);
}

/* Netascii — remove CR (Windows to Linux) */
void netascii_linux(int fd, tftp_packet *pkt, int size)
{
    int i = 0, j = 0;
    char buf[512];
    memset(buf, 0, sizeof(buf));
    while (pkt->body.data.data[i] && i < size)
    {
        if (pkt->body.data.data[i] != '\r')
        {
            buf[j] = pkt->body.data.data[i];
            j++;
        }
        i++;
    }
    write(fd, buf, j);
}

/* Netascii — add CR before LF (Linux to Windows) */
void netascii_windows(int fd, tftp_packet *pkt, int size)
{
    int i = 0, j = 0;
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    while (pkt->body.data.data[i] && i < size)
    {
        if (pkt->body.data.data[i] != '\n')
        {
            buf[j] = pkt->body.data.data[i];
        }
        else
        {
            buf[j] = '\r';
            buf[++j] = '\n';
        }
        j++;
        i++;
    }
    write(fd, buf, j);
}
