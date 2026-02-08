#ifndef TFTP_CLIENT_H
#define TFTP_CLIENT_H

/* Header file for TFTP client */
typedef struct
{
    int sockfd;                     // Socket file descriptor for communication with the server
    struct sockaddr_in server_addr; // To store server address information
    socklen_t server_len;           // To store the length of the server address structure
    char server_ip[27];             // To store server IP address (IPv6 can be up to 45 characters, but we are using 27 for simplicity)
    int port;                       // To store server port number
} tftp_client_t;

/* Function prototypes for TFTP client operations */
void connect_to_server(tftp_client_t *client); // directly send the structure

/* Function prototypes for file transfer operations */
void put_file(tftp_client_t *client, tftp_packet *pkt, tftp_packet *ack);

/* Function prototype for receiving file from server */
void get_file(tftp_client_t *client, tftp_packet *pkt, tftp_packet *ack);

/* Function prototype for mode selection */
void mode(tftp_client_t *client, tftp_packet *pkt, tftp_packet *ack);

/* Function prototype for converting Netascii data from Windows format to Linux format */
void netascii_windows(int fd, tftp_packet *pkt, int size);

/* Function prototype for converting Netascii data from Linux format to Windows format */
void netascii_linux(int fd, tftp_packet *pkt, int size);

#endif