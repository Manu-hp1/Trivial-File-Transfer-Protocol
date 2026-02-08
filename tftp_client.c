#include "tftp.h" // Include the common header file for TFTP
#include "tftp_client.h" // Include the client-specific header file for TFTP

int main()
{
    char command[256];
    tftp_client_t client;

    memset(&client, 0, sizeof(client)); // Initialize client structure
    tftp_packet pkt, ack;
    
    /* Initialize packet and ack structures to zero */
    memset(&pkt, 0, sizeof(pkt));
    memset(&ack, 0, sizeof(ack));
   
    pkt.mode = DEFAULT;

    int choice;

    // Main loop for command-line interface
    while (1)
    {
  
        usleep(1000010);
        system("clear");

        // Print the main menu
        printf("\033[1;97m\n▐▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▌\n");
        printf("▐ \033[1;7;93m%-12c\033[1;92m %s \033[0m\033[1;7;93m%-11c\033[0m\033[1;97m ▌\n", ' ', "WELCOME TO MANUSHARE", ' ');
        printf("\033[1;97m▐▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▌\n");
        printf("▐ \033[1;7;92m%-18c\033[1;92m %s \033[0m\033[1;7;92m%-16c\033[0m\033[1;97m ▌\n", ' ', "MAIN MENU", ' ');
        printf("\033[1;97m▐▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▌\n");
        printf("▐ \033[1;93m\033[1;7m 1. \033[1;92m %-4s\033[0m\033[1;97m\t\t\t\033[1;3m\033[0m▌\n", "\U0001F517 CONNECT TO SERVER "); // Print TITLE
        printf("\033[1;97m▐▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▌\n");
        printf("▐ \033[1;93m\033[1;7m 2. \033[1;92m %-4s\033[0m\033[1;97m\t\t\t\033[1;3m\033[0m▌\n", "\U0001F4E4 PUT TO SERVER "); // Print ARTIST
        printf("\033[1;97m▐▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▌\n");
        printf("▐ \033[1;93m\033[1;7m 3. \033[1;92m %-4s\033[0m\033[1;97m\t\t\t\033[1;3m\033[0m▌\n", "\U0001F4E5 GET FROM SERVER "); // Print ALBUM
        printf("\033[1;97m▐▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▌\n");
        printf("▐ \033[1;93m\033[1;7m 4. \033[1;92m %-4s\033[0m\033[1;97m\t\t\t\t\033[1;3m\033[0m▌\n", "\U0001F504 MODE CHANGE "); // Print YEAR
        printf("\033[1;97m▐▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▌\n");
        printf("▐ \033[1;93m\033[1;7m 5. \033[1;92m %-4s\033[0m\033[1;97m\t\t\t\033[1;3m\033[0m▌\n", "\U0001F6AA EXIT FROM MANUSHARE "); // Print GENRE
        printf("\033[1;97m▐▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▌\n");

        int choice;
        printf("\n\033[38;2;0;255;255mEnter your choice: \033[1;97m");
        scanf("%d", &choice); // Read the choice

        switch (choice) // Based on the choice perform the operation
        {
        case 1:
            connect_to_server(&client); // Pass the client structure to the connect_to_server function to establish connection with the server
            break;
        case 2:
            put_file(&client, &pkt, &ack); // Pass the client structure, packet structure and ack structure to the put_file function to send file to the server
            break;
        case 3:
            get_file(&client, &pkt, &ack); // Pass the client structure, packet structure and ack structure to the get_file function to receive file from the server
            break;
        case 4:
            mode(&client, &pkt, &ack); // Pass the client structure, packet structure and ack structure to the mode function to change the mode of transfer
            break;
        case 5:
            close(client.sockfd); // Close the socket before exiting
            return 0;
        default:
            printf("Enter a valid choice!!\n");
        }
    }
}

/* Function to connect to the server */
void connect_to_server(tftp_client_t *client)
{
    client->sockfd = socket(AF_INET, SOCK_DGRAM, 0); // Create a UDP socket
    if (client->sockfd < 0) // Check if socket creation was successful
    {
        printf("\033[1;97m[\033[1;91mERROR\033[1;97m] Socket creation failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    printf("\033[1;97m[\033[1;92mSOCKET\033[1;97m] Socket created successfully\n");
    
    /* Get server IP address and port number from user input */
    printf("Enter the server address: \033[1;92m");
    scanf("%s", client->server_ip);
    printf("\033[1;97mEnter the server port number: \033[1;92m");
    scanf("%d", &client->port);
    
    /* Validate the server IP address and port number */
    if (client->port >= 1024 && client->port <= 65535) // user ports range: 1024 ~ 49151
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

    /* Set up the server address structure */
    client->server_addr.sin_family = AF_INET;
    client->server_addr.sin_port = htons(client->port);

    client->server_addr.sin_addr.s_addr = inet_addr(client->server_ip);
    client->server_len = sizeof(client->server_addr);
    printf("\033[1;97m[\033[1;92mCONNECT\033[1;97m] CONNECTION SUCCESSFULL\n");
}

/* Function to handle PUT operation */
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
    scanf("%255s", filename); // Read the file name from the user

    int fd = open(filename, O_RDONLY); // Open the file for reading
    if (fd < 0) // Check if file opening was successful
    {
        printf("[\033[1;91mERROR\033[1;97m] Failed to open file '%s': %s\n", filename, strerror(errno));
        return;
    }

    pkt->opcode = htons(WRQ);
    strcpy(pkt->body.request.filename, filename);

    sendto(client->sockfd, pkt, sizeof(*pkt), 0, (struct sockaddr *)&client->server_addr, client->server_len);

    recvfrom(client->sockfd, ack, sizeof(*ack), 0, (struct sockaddr *)&client->server_addr, &client->server_len);

    uint16_t block = 1;
    ssize_t n;

    /* Use lseek to find size of file */
    off_t size1 = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    int data_size = 512; // Default data size for DEFAULT and NETASCII modes

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
        sendto(client->sockfd, ack, sizeof(*ack), 0, (struct sockaddr *)&client->server_addr, client->server_len);
        printf("[\033[1;92mACK\033[1;97m] ACK sent for block %u\n", ntohs(ack->body.ack.block));

        pkt->opcode = htons(DATA);
        pkt->body.data.block = htons(block);
        pkt->body.data.size = htons(n);
        pkt->body.data.data_size = htons((int)size1);

        sendto(client->sockfd, pkt, sizeof(*pkt), 0, (struct sockaddr *)&client->server_addr, client->server_len);
        printf("[\033[1;94mDATA\033[1;97m] DATA sent | Block = %u | Size = %lu bytes\n", ntohs(pkt->body.data.block), n);

        recvfrom(client->sockfd, ack, sizeof(*ack), 0, (struct sockaddr *)&client->server_addr, &client->server_len);
        printf("[\033[1;92mACK\033[1;97m] ACK received for block %u\n", ntohs(ack->body.data.block));

        block++;
    }
    printf("[\033[1;35mMODE\033[1;97m] : %dDEFAULT\n", pkt->mode);

    if (pkt->mode == DEFAULT)
        printf("[\033[1;35mMODE\033[1;97m] : DEFAULT\n");
    else if (pkt->mode == OCTET)
        printf("[\033[1;35mMODE\033[1;97m] : OCTET\n");
    else if (pkt->mode == NETASCII)
        printf("[\033[1;35mMODE\033[1;97m] : NETASCII\n");
    if (pkt->mode == DEFAULT) // Send an ACK packet to indicate end of file transfer
    {
        ack->opcode = htons(ERROR);
        ack->body.data.block = htons(block);
        sendto(client->sockfd, ack, sizeof(*ack), 0, (struct sockaddr *)&client->server_addr, client->server_len);
        printf("[\033[1;92mACK\033[1;97m] ACK sent for block %u\n", ntohs(ack->body.ack.block));
    }
    else if (pkt->mode == NETASCII) // Send an ACK packet to indicate end of file transfer
    {
        ack->opcode = htons(ERROR);
        ack->body.data.block = htons(block);
        sendto(client->sockfd, ack, sizeof(*ack), 0, (struct sockaddr *)&client->server_addr, client->server_len);
        printf("[\033[1;92mACK\033[1;97m] ACK sent for block %u\n", ntohs(ack->body.ack.block));
    }

    printf("[\033[38;2;0;255;255mPUT\033[1;97m] File sent successfully\n");
    close(fd);
}

/* Function to handle GET operation */
void get_file(tftp_client_t *client, tftp_packet *pkt, tftp_packet *ack)
{
    char filename[256];

    printf("Enter a valid filename: ");
    scanf("%255s", filename); // read the file name from the user

    pkt->opcode = htons(RRQ);
    strcpy(pkt->body.request.filename, filename);

    sendto(client->sockfd, pkt, sizeof(*pkt), 0, (struct sockaddr *)&client->server_addr, client->server_len);

    recvfrom(client->sockfd, ack, sizeof(*ack), 0, (struct sockaddr *)&client->server_addr, &client->server_len);

    if (ntohs(ack->opcode) == ERROR)
    {
        printf("[\033[1;91mERROR\033[1;97m] No such file in server side\n");
        return;
    }
    int fd = open(pkt->body.request.filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd < 0)
    {
        printf("[\033[1;91mERROR\033[1;97m] Failed to open file '%s': %s\n", filename, strerror(errno));
        return;
    }
    ack->opcode = htons(ACK);
    ack->body.ack.block = htons(0);
    int count = 1;
    /* Receive DATA packets and send ACKs */
    while (1)
    {
        recvfrom(client->sockfd, ack, sizeof(*ack), 0, (struct sockaddr *)&client->server_addr, &client->server_len);
        printf("[\033[1;92mACK\033[1;97m] ACK received for block %u\n", ntohs(ack->body.data.block));
        if (ntohs(ack->opcode) == ERROR) // Check if the received packet is an ERROR packet indicating end of file transfer for DEFAULT and NETASCII modes
        {
            printf("[\033[38;2;255;105;180mEOF\033[1;97m] End of file in server\n");
            break;
        }
        recvfrom(client->sockfd, pkt, sizeof(*pkt), 0, (struct sockaddr *)&client->server_addr, &client->server_len);
        uint16_t size = ntohs(pkt->body.data.size);
        printf("[\033[1;92mDATA\033[1;97m] DATA received | Block = %u | Size = %u bytes\n", ntohs(pkt->body.data.block), size);

        /* Write data to file based on mode */
        if (pkt->mode == NETASCII)
        {
            if (pkt->Client_OS == WINDOWS && pkt->Server_OS == LINUX)
            {
                netascii_windows(fd, pkt, size);
            }
            else if (pkt->Client_OS == LINUX && pkt->Server_OS == WINDOWS)
            {
                netascii_linux(fd, pkt, size);
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
        sendto(client->sockfd, ack, sizeof(*ack), 0, (struct sockaddr *)&client->server_addr, client->server_len);
        printf("[\033[1;92mACK\033[1;97m] ACK sent for block %u\n", ntohs(ack->body.ack.block));

        if (count > ntohs(pkt->body.data.data_size)) // Check if the count exceeds the total number of blocks expected based on the file size for OCTET mode
        {
            printf("[\033[1;92mFILE\033[1;97m] COMPLETED!\n");
            break;
        }
    }
    printf("[\033[38;2;0;255;255mGET\033[1;97m] File got successfully\n");
}

/* Function to handle mode change operation */
void mode(tftp_client_t *client, tftp_packet *pkt, tftp_packet *ack)
{

    printf("\033[1;36m███████████████████████████████████████\n");
    printf("▌\t%-6c\U0001F4C2\033[1;37m  %-20s\033[0m\033[1;36m▐\n", ' ', "MODE MENU");
    printf("███████████████████████████████████████\n");
    printf("\033[1;36m▌\t\t\033[1;37m1. %-19s\033[0m\033[1;36m▐\n", "DEFAULT"); // list contacts by sorting using names for choice 1
    printf("███████████████████████████████████████\n");
    printf("\033[1;36m▌\t\t\033[1;37m2. %-19s\033[0m\033[1;36m▐\n", "OCTET"); // list contacts by sorting using phone number for choice 2
    printf("███████████████████████████████████████\n");
    printf("\033[1;36m▌\t\t\033[1;37m3. %-19s\033[0m\033[1;36m▐\n", "NETASCII"); // list contacts by sorting using email for choice 3
    printf("███████████████████████████████████████\n");
    printf("\n\033[1;36mEnter the mode in which you wnat to transfer/receive data: \033[0m\033[1;37m");
    int choice;
    scanf("%d", &choice);
    printf("YOUR CHOICE: %d\n", choice); // Print the choice for debugging purposes
    switch (choice) // Based on the choice set the mode of transfer and print the selected mode
    {
    case 1:
        printf("[\033[1;35mMODE\033[1;97m] : DEFAULT\n");
        pkt->mode = DEFAULT; // Set the mode of transfer to DEFAULT
        break;
    case 2:
        printf("[\033[1;35mMODE\033[1;97m] : OCTET\n");
        pkt->mode = OCTET; // Set the mode of transfer to OCTET
        break;
    case 3:
        printf("[\033[1;35mMODE\033[1;97m] : NETASCII\n");
        printf("\033[1;92mPlease submit below mandatory information:-\n\033[1;97mChoose Client OS:  1] Linux   2] Windows\nEnter the choice: ");
        scanf("%d", &choice);
        pkt->Client_OS = (choice == 1) ? LINUX : WINDOWS;

        printf("Choose Server OS:-  1] Linux    2] Windows\nEnter the choice: ");
        scanf("%d", &choice);
        pkt->Server_OS = (choice == 1) ? LINUX : WINDOWS;
        pkt->mode = NETASCII; // Set the mode of transfer to NETASCII
        break;
    default:
        printf("Invalid Mode\n");
    }
}

/* Function to handle NETASCII mode for Linux client and Windows server */
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

    memcpy(pkt->body.data.data, buf, i);
    write(fd, pkt->body.data.data, size);
}

/* Function to handle NETASCII mode for Windows client and Linux server */
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
    
    write(fd, buf, i);
    // write(fd, pkt->body.data.data, size);
}
