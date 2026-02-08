/* Common file for server & client */

#include "tftp.h" // Include the common header file for TFTP

/* Function to receive file from client and save it on server */
void send_file(int sockfd, struct sockaddr_in client_addr, socklen_t client_len, tftp_packet *pkt, tftp_packet *ack)
{
    if (pkt->mode == DEFAULT)
        printf("[\033[1;35mMODE\033[1;97m] : DEFAULT\n");
    else if (pkt->mode == OCTET)
        printf("[\033[1;35mMODE\033[1;97m] : OCTET\n");
    else if (pkt->mode == NETASCII)
        printf("[\033[1;35mMODE\033[1;97m] : NETASCII\n");

    int fd = open(pkt->body.request.filename, O_RDONLY); // Open the file for reading, the file should exist in server directory
    if (fd < 0)                                          // Check if file opening was successful
    {
        printf("[\033[1;91mERROR\033[1;97m] Failed to open file '%s': %s\n", pkt->body.request.filename, strerror(errno));
        ack->opcode = htons(ERROR);
        sendto(sockfd, ack, sizeof(*ack), 0, (struct sockaddr *)&client_addr, client_len);
        return;
    }
    /* Use lseek to find size of file*/
    off_t size2 = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    ack->opcode = htons(ACK);
    sendto(sockfd, ack, sizeof(*ack), 0, (struct sockaddr *)&client_addr, client_len);

    uint16_t block = 1; // To keep track of block number
    ssize_t n;          // To store number of bytes read from file

    int data_size = 1; // To store size of data to be sent based on mode

    /* Set data_size based on mode */
    if (pkt->mode == DEFAULT)
    {
        data_size = 512;
    }
    else if (pkt->mode == OCTET)
    {
        data_size = 1;
    }
    else if (pkt->mode == NETASCII)
    {
        data_size = 512;
    }

    /* Read file in chunks and send DATA packets */
    while ((n = read(fd, pkt->body.data.data, data_size)) > 0)
    {
        ack->opcode = htons(ACK);
        ack->body.ack.block = htons(block);
        sendto(sockfd, ack, sizeof(*ack), 0, (struct sockaddr *)&client_addr, client_len);
        printf("[\033[1;92mACK\033[1;97m] ACK sent for block %u\n", ntohs(ack->body.ack.block));

        pkt->opcode = htons(DATA);
        pkt->body.data.block = htons(block);
        pkt->body.data.size = htons(n);
        pkt->body.data.data_size = htons((int)size2);

        sendto(sockfd, pkt, sizeof(*pkt), 0, (struct sockaddr *)&client_addr, client_len);
        printf("[\033[1;94mDATA\033[1;97m] DATA sent | Block = %u | Size = %lu bytes\n", ntohs(pkt->body.data.block), n);

        ack->body.ack.block = pkt->body.data.block;
        recvfrom(sockfd, ack, sizeof(*ack), 0, (struct sockaddr *)&client_addr, &client_len);
        printf("[\033[1;92mACK\033[1;97m] ACK received for block %u\n", ntohs(ack->body.data.block));

        block++;
    }
    if (pkt->mode == DEFAULT) // Check if the last packet sent was less than 512 bytes for DEFAULT mode to indicate end of file transfer
    {
        ack->opcode = htons(ERROR);
        ack->body.data.block = htons(block);
        sendto(sockfd, ack, sizeof(*ack), 0, (struct sockaddr *)&client_addr, client_len);
        printf("[\033[1;92mACK\033[1;97m] ACK sent for block %u\n", ntohs(ack->body.ack.block));
    }
    else if (pkt->mode == NETASCII) // Check if the last packet sent was less than 512 bytes for NETASCII mode to indicate end of file transfer
    {
        ack->opcode = htons(ERROR);
        ack->body.data.block = htons(block);
        sendto(sockfd, ack, sizeof(*ack), 0, (struct sockaddr *)&client_addr, client_len);
        printf("[\033[1;92mACK\033[1;97m] ACK sent for block %u\n", ntohs(ack->body.ack.block));
    }
    printf("[\033[38;5;208mFILE\033[1;97m] SENDING FILE COMPLETED\n");
    close(fd);
}

/* Function to receive file from client */
void receive_file(int sockfd, struct sockaddr_in client_addr, socklen_t client_len, tftp_packet *pkt, tftp_packet *ack)
{
    if (pkt->mode == DEFAULT)
        printf("[\033[1;35mMODE\033[1;97m] : DEFAULT\n");
    else if (pkt->mode == OCTET)
        printf("[\033[1;35mMODE\033[1;97m] : OCTET\n");
    else if (pkt->mode == NETASCII)
        printf("[\033[1;35mMODE\033[1;97m] : NETASCII\n");

    int fd = open(pkt->body.request.filename, O_WRONLY | O_CREAT | O_TRUNC, 0666); // Open file for writing, create if it doesn't exist, truncate if it does

    ack->opcode = htons(ACK);
    ack->body.ack.block = htons(0);
    ack->body.ack.data_size = 0;

    sendto(sockfd, ack, sizeof(*ack), 0, (struct sockaddr *)&client_addr, client_len);
    int count = 1;

    while (1) // Loop to receive DATA packets and send ACKs until end of file transfer is indicated by an ERROR packet from the client for DEFAULT and NETASCII modes
    {
        recvfrom(sockfd, ack, sizeof(*ack), 0, (struct sockaddr *)&client_addr, &client_len);
        printf("[\033[1;92mACK\033[1;97m] : ACK received for block %u\n", ntohs(ack->body.data.block));
        if (ntohs(ack->opcode) == ERROR)
        {
            printf("[\033[38;2;255;105;180mEOF\033[1;97m] : End of file in server\n");
            break;
        }
        recvfrom(sockfd, pkt, sizeof(*pkt), 0, (struct sockaddr *)&client_addr, &client_len);
        printf("[\033[1;94mDATA\033[1;97m] : DATA received | Block = %u | Size = %u bytes\n", ntohs(pkt->body.data.block), ntohs(pkt->body.data.size));

        uint16_t size = ntohs(pkt->body.data.size);

        if (pkt->mode == NETASCII)
        {
            if (pkt->Client_OS == WINDOWS && pkt->Server_OS == LINUX)
            {
                netascii_linux(fd, pkt, size);
            }
            else if (pkt->Client_OS == LINUX && pkt->Server_OS == WINDOWS)
            {
                netascii_windows(fd, pkt, size);
            }
            else
            {
                write(fd, pkt->body.data.data, size);
            }
        }
        else
        {
            write(fd, pkt->body.data.data, size);
        }

        ack->body.ack.block = pkt->body.data.block;
        ack->body.ack.data_size = size;
        count++;
        sendto(sockfd, ack, sizeof(*ack), 0, (struct sockaddr *)&client_addr, client_len);
        printf("[\033[1;92mACK\033[1;97m] : ACK sent for block %u\n", ntohs(ack->body.ack.block));
        if (count > ntohs(pkt->body.data.data_size)) // Check if the count of blocks received exceeds the total data size indicated by the client for DEFAULT and NETASCII modes to indicate end of file transfer
        {
            printf("[\033[1;91mDATA\033[1;97m] : SIZE is less that DATA_SIZE\n");
            break;
        }
    }
    printf("[\033[38;5;208mFILE\033[1;97m] : SENDING FILE COMPLETED\n");

    close(fd);
}

/* Function to convert Netascii data from Linux format to Windows format */
void netascii_linux(int fd, tftp_packet *pkt, int size)
{
    int i = 0;
    char buf[512];
    memset(buf, 0, sizeof(buf));
    while (pkt->body.data.data[i] && i < size)
    {
        if (pkt->body.data.data[i] != '\r')
        {
            buf[i] = pkt->body.data.data[i];
        }
        i++;
    }

    memcpy(pkt->body.data.data, buf, i); // Copy the converted data back to the packet's data buffer
    write(fd, pkt->body.data.data, size); // Write the converted data to the file
}

/* Function to convert Netascii data from Windows format to Linux format */
void netascii_windows(int fd, tftp_packet *pkt, int size)
{
    int i = 0, j = 0;
    char buf[1024];
    memset(buf, 0, sizeof(buf)); // Buffer to hold the converted data, size is doubled to accommodate potential increase in size due to conversion of \n to \r\n
    while (pkt->body.data.data[i] && i < size)
    {
        if (pkt->body.data.data[i] != '\n') // If it's not a newline, copy it to the buffer
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

    write(fd, buf, i); // Write the converted data to the file, using 'i' as the size since it represents the number of bytes processed from the original data buffer
    // write(fd, pkt->body.data.data, size);
}