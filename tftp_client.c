#include "tftp.h"        // Include the common header file for TFTP
#include "tftp_client.h" // Include the client-specific header file for TFTP

int main()
{
    tftp_client_t client;
    memset(&client, 0, sizeof(client));
    tftp_packet pkt, ack;
    memset(&pkt, 0, sizeof(pkt));
    memset(&ack, 0, sizeof(ack));
    pkt.mode = DEFAULT;

    while (1)
    {
        usleep(1000010);
        system("clear");

        printf("\033[1;97m\n▐▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▌\n");
        printf("▐ \033[1;7;93m%-12c\033[1;92m %s \033[0m\033[1;7;93m%-11c\033[0m\033[1;97m ▌\n", ' ', "WELCOME TO MANUSHARE", ' ');
        printf("\033[1;97m▐▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▌\n");
        printf("▐ \033[1;7;92m%-18c\033[1;92m %s \033[0m\033[1;7;92m%-16c\033[0m\033[1;97m ▌\n", ' ', "MAIN MENU", ' ');
        printf("\033[1;97m▐▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▌\n");
        printf("▐ \033[1;93m\033[1;7m 1. \033[1;92m %-4s\033[0m\033[1;97m\t\t\t\033[1;3m\033[0m▌\n", "\U0001F517 CONNECT TO SERVER ");
        printf("\033[1;97m▐▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▌\n");
        printf("▐ \033[1;93m\033[1;7m 2. \033[1;92m %-4s\033[0m\033[1;97m\t\t\t\033[1;3m\033[0m▌\n", "\U0001F4E4 PUT TO SERVER ");
        printf("\033[1;97m▐▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▌\n");
        printf("▐ \033[1;93m\033[1;7m 3. \033[1;92m %-4s\033[0m\033[1;97m\t\t\t\033[1;3m\033[0m▌\n", "\U0001F4E5 GET FROM SERVER ");
        printf("\033[1;97m▐▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▌\n");
        printf("▐ \033[1;93m\033[1;7m 4. \033[1;92m %-4s\033[0m\033[1;97m\t\t\t\t\033[1;3m\033[0m▌\n", "\U0001F504 MODE CHANGE ");
        printf("\033[1;97m▐▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▌\n");
        printf("▐ \033[1;93m\033[1;7m 5. \033[1;92m %-4s\033[0m\033[1;97m\t\t\t\033[1;3m\033[0m▌\n", "\U0001F6AA EXIT FROM MANUSHARE ");
        printf("\033[1;97m▐▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▌\n");

        int choice;
        printf("\n\033[38;2;0;255;255mEnter your choice: \033[1;97m");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            connect_to_server(&client);
            break;
        case 2:
            if (client.sockfd <= 0)
            {
                printf("[\033[1;91mERROR\033[1;97m] Please connect to server first! (Choose option 1)\n");
                sleep(1);
                break;
            }
            put_file(&client, &pkt, &ack);
            break;
        case 3:
            if (client.sockfd <= 0)
            {
                printf("[\033[1;91mERROR\033[1;97m] Please connect to server first! (Choose option 1)\n");
                sleep(1);
                break;
            }
            get_file(&client, &pkt, &ack);
            break;
        case 4:
            mode(&client, &pkt, &ack);
            break;
        case 5:
            close(client.sockfd);
            return 0;
        default:
            printf("Enter a valid choice!!\n");
        }
    }
}

/* Function to connect to the server */
void connect_to_server(tftp_client_t *client)
{
    if (client->sockfd > 0)
    {
        close(client->sockfd);
        client->sockfd = 0;
    }

    client->sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (client->sockfd < 0)
    {
        printf("\033[1;97m[\033[1;91mERROR\033[1;97m] Socket creation failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    printf("\033[1;97m[\033[1;92mSOCKET\033[1;97m] Socket created successfully\n");

    printf("Enter the server address: \033[1;92m");
    scanf("%s", client->server_ip);
    printf("\033[1;97mEnter the server port number: \033[1;92m");
    scanf("%d", &client->port);

    if (client->port >= 1024 && client->port <= 65535)
    {
        if (inet_pton(AF_INET, client->server_ip, &(client->server_addr.sin_addr)) != 1)
        {
            printf("\033[1;97m[\033[1;91mERROR\033[1;97m] Invalid IP address format: %s\n", client->server_ip);
            exit(0);
        }
    }
    else
    {
        printf("\033[1;97m[\033[1;91mERROR\033[1;97m] Invalid port number: %d \033[1;91m(valid range: 1024 - 65535)\033[97m\n", client->port);
        exit(0);
    }

    client->server_addr.sin_family = AF_INET;
    client->server_addr.sin_port = htons(client->port);
    client->server_addr.sin_addr.s_addr = inet_addr(client->server_ip);
    client->server_len = sizeof(client->server_addr);
    printf("\033[1;97m[\033[1;92mCONNECT\033[1;97m] CONNECTION SUCCESSFULL\n");
}

/* Function to handle PUT operation
 * CORRECT WRQ flow — client side:
 * 1. Client sends WRQ
 * 2. Server sends ACK 0  ← wait for this
 * 3. Client sends DATA block 1
 * 4. Server sends ACK 1  ← wait for this
 * 5. Client sends DATA block 2
 * ... repeat until all data sent
 * 6. Client sends empty DATA (size=0) = EOF signal
 */
void put_file(tftp_client_t *client, tftp_packet *pkt, tftp_packet *ack)
{
    if (pkt->mode == DEFAULT)
        printf("[\033[1;35mMODE\033[1;97m] : DEFAULT\n");
    else if (pkt->mode == OCTET)
        printf("[\033[1;35mMODE\033[1;97m] : OCTET\n");
    else if (pkt->mode == NETASCII)
        printf("[\033[1;35mMODE\033[1;97m] : NETASCII\n");

    char filename[256];
    printf("Enter valid filename: ");
    scanf("%255s", filename);

    int fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        printf("[\033[1;91mERROR\033[1;97m] Failed to open file '%s': %s\n", filename, strerror(errno));
        return;
    }

    /* Step 1: Send WRQ to server */
    pkt->opcode = htons(WRQ);
    strcpy(pkt->body.request.filename, filename);
    sendto(client->sockfd, pkt, sizeof(*pkt), 0, (struct sockaddr *)&client->server_addr, client->server_len);

    /* Step 2: Wait for ACK 0 from server (server ready to receive) */
    recvfrom(client->sockfd, ack, sizeof(*ack), 0, (struct sockaddr *)&client->server_addr, &client->server_len);
    printf("[\033[1;92mACK\033[1;97m] ACK 0 received — server ready\n");

    uint16_t block = 1;
    ssize_t n;

    off_t size1 = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    int data_size = 512;

    if (pkt->mode == DEFAULT)
        data_size = 512;
    else if (pkt->mode == OCTET)
        data_size = 1;
    else if (pkt->mode == NETASCII)
        data_size = 512;

    while ((n = read(fd, pkt->body.data.data, data_size)) > 0)
    {
        /* Step 3: Send DATA to server */
        pkt->opcode = htons(DATA);
        pkt->body.data.block = htons(block);
        pkt->body.data.size = htons(n);
        pkt->body.data.data_size = htons((int)size1);
        sendto(client->sockfd, pkt, sizeof(*pkt), 0, (struct sockaddr *)&client->server_addr, client->server_len);
        printf("[\033[1;94mDATA\033[1;97m] DATA sent | Block = %u | Size = %lu bytes\n", ntohs(pkt->body.data.block), n);

        /* Step 4: Wait for ACK from server */
        recvfrom(client->sockfd, ack, sizeof(*ack), 0, (struct sockaddr *)&client->server_addr, &client->server_len);
        printf("[\033[1;92mACK\033[1;97m] ACK received for block %u\n", ntohs(ack->body.ack.block));

        block++;
    }

    /* Step 5: Send EOF signal — empty DATA packet */
    if (pkt->mode == DEFAULT || pkt->mode == NETASCII)
    {
        pkt->opcode = htons(DATA);
        pkt->body.data.block = htons(block);
        pkt->body.data.size = htons(0);
        sendto(client->sockfd, pkt, sizeof(*pkt), 0, (struct sockaddr *)&client->server_addr, client->server_len);
        printf("[\033[1;92mACK\033[1;97m] EOF signal sent for block %u\n", ntohs(pkt->body.data.block));
    }

    printf("[\033[38;2;0;255;255mPUT\033[1;97m] File sent successfully\n");
    close(fd);
}

/* Function to handle GET operation
 * CORRECT RRQ flow — client side:
 * 1. Client sends RRQ
 * 2. Server sends DATA block 1  ← receive this directly
 * 3. Client sends ACK block 1
 * 4. Server sends DATA block 2
 * 5. ... repeat until EOF
 * 6. Server sends empty DATA (size=0) = EOF
 */
void get_file(tftp_client_t *client, tftp_packet *pkt, tftp_packet *ack)
{
    char filename[256];
    printf("Enter a valid filename: ");
    scanf("%255s", filename);

    /* Step 1: Send RRQ to server */
    pkt->opcode = htons(RRQ);
    strcpy(pkt->body.request.filename, filename);
    sendto(client->sockfd, pkt, sizeof(*pkt), 0, (struct sockaddr *)&client->server_addr, client->server_len);

    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd < 0)
    {
        printf("[\033[1;91mERROR\033[1;97m] Failed to open file '%s': %s\n", filename, strerror(errno));
        return;
    }

    int count = 1;

    while (1)
    {
        /* Step 2: Receive DATA directly from server (no waiting for ACK first!) */
        recvfrom(client->sockfd, pkt, sizeof(*pkt), 0, (struct sockaddr *)&client->server_addr, &client->server_len);

        /* Check for ERROR from server (e.g. file not found) */
        if (ntohs(pkt->opcode) == ERROR)
        {
            printf("[\033[1;91mERROR\033[1;97m] No such file in server side\n");
            close(fd);
            return;
        }

        /* Check for EOF signal — empty DATA packet */
        if (ntohs(pkt->opcode) == DATA && ntohs(pkt->body.data.size) == 0)
        {
            printf("[\033[38;2;255;105;180mEOF\033[1;97m] End of file received\n");
            break;
        }

        uint16_t size = ntohs(pkt->body.data.size);
        printf("[\033[1;92mDATA\033[1;97m] DATA received | Block = %u | Size = %u bytes\n",
               ntohs(pkt->body.data.block), size);

        /* Write data to file based on mode */
        if (pkt->mode == NETASCII)
        {
            if (pkt->Client_OS == WINDOWS && pkt->Server_OS == LINUX)
                netascii_windows(fd, pkt, size);
            else if (pkt->Client_OS == LINUX && pkt->Server_OS == WINDOWS)
                netascii_linux(fd, pkt, size);
            else
                write(fd, pkt->body.data.data, size);
        }
        else
        {
            write(fd, pkt->body.data.data, size);
        }

        /* Step 3: Send ACK back to server */
        ack->opcode = htons(ACK);
        ack->body.ack.block = pkt->body.data.block;
        ack->body.ack.data_size = size;
        count++;
        sendto(client->sockfd, ack, sizeof(*ack), 0, (struct sockaddr *)&client->server_addr, client->server_len);
        printf("[\033[1;92mACK\033[1;97m] ACK sent for block %u\n", ntohs(ack->body.ack.block));

        if (count > ntohs(pkt->body.data.data_size))
        {
            printf("[\033[1;92mFILE\033[1;97m] COMPLETED!\n");
            break;
        }
    }
    printf("[\033[38;2;0;255;255mGET\033[1;97m] File got successfully\n");
    close(fd);
}

/* Function to handle mode change operation */
void mode(tftp_client_t *client, tftp_packet *pkt, tftp_packet *ack)
{
    (void)client;
    (void)ack;
    printf("\033[1;36m███████████████████████████████████████\n");
    printf("▌\t%-6c\U0001F4C2\033[1;37m  %-20s\033[0m\033[1;36m▐\n", ' ', "MODE MENU");
    printf("███████████████████████████████████████\n");
    printf("\033[1;36m▌\t\t\033[1;37m1. %-19s\033[0m\033[1;36m▐\n", "DEFAULT");
    printf("███████████████████████████████████████\n");
    printf("\033[1;36m▌\t\t\033[1;37m2. %-19s\033[0m\033[1;36m▐\n", "OCTET");
    printf("███████████████████████████████████████\n");
    printf("\033[1;36m▌\t\t\033[1;37m3. %-19s\033[0m\033[1;36m▐\n", "NETASCII");
    printf("███████████████████████████████████████\n");
    printf("\n\033[1;36mEnter the mode in which you want to transfer/receive data: \033[0m\033[1;37m");

    int choice;
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        printf("[\033[1;35mMODE\033[1;97m] : DEFAULT\n");
        pkt->mode = DEFAULT;
        break;
    case 2:
        printf("[\033[1;35mMODE\033[1;97m] : OCTET\n");
        pkt->mode = OCTET;
        break;
    case 3:
        printf("[\033[1;35mMODE\033[1;97m] : NETASCII\n");
        printf("\033[1;92mPlease submit below mandatory information:-\n\033[1;97mChoose Client OS:  1] Linux   2] Windows\nEnter the choice: ");
        scanf("%d", &choice);
        pkt->Client_OS = (choice == 1) ? LINUX : WINDOWS;
        printf("Choose Server OS:-  1] Linux    2] Windows\nEnter the choice: ");
        scanf("%d", &choice);
        pkt->Server_OS = (choice == 1) ? LINUX : WINDOWS;
        pkt->mode = NETASCII;
        break;
    default:
        printf("Invalid Mode\n");
    }
}

/* Function to handle NETASCII — removes CR (Windows to Linux) */
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

/* Function to handle NETASCII — adds CR before LF (Linux to Windows) */
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
