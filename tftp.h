/* Common file for server & client*/

#ifndef TFTP_H
#define TFTP_H

/* Header files for socket programming, file handling, etc. */
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define PORT 6969 // Default TFTP Server port

/* Define constants and structures for TFTP */
typedef enum
{
    RRQ = 1,
    WRQ = 2,
    DATA = 3,
    ACK = 4,
    ERROR = 5
} tftp_opcode;

typedef enum
{
    DEFAULT = 512,
    OCTET = 1,
    NETASCII = 3
} tftp_mode;

typedef enum
{
    LINUX = 11,
    WINDOWS = 22
} OS;

#pragma pack(push, 1)

typedef struct
{
    uint16_t opcode; // Operation code (RRQ, WRQ, DATA, ACK, ERROR)
    int Client_OS;   // To store client OS information
    int Server_OS;   // To store server OS information
    int mode;        // To store mode of transfer (DEFAULT, OCTET, NETASCII)
    union            // Packet body for different TFTP operations
    {
        struct
        { 
            char filename[256]; // To store filename for RRQ/WRQ
        } request;

        struct
        {
            uint16_t block;     // Block number
            uint16_t size;      // Size of the data being sent
            char data[DEFAULT]; // Data buffer (size depends on mode)
            int data_size;      // To store the actual size of data read from file
        } data;

        struct
        {                   // ACK
            uint16_t block; // Block number being acknowledged
            int data_size;  // To store the size of data being acknowledged
        } ack;

    } body;

} tftp_packet;

#pragma pack(pop)

/* Define client structure */
typedef struct
{
    int sockfd; // To store fd
    struct sockaddr_in client_addr; // To store client address
    socklen_t client_len; // To store client address length
    char client_ip[27]; // To store client IP address (IPv6 can be up to 45 characters, but we are using 27 for simplicity)
    int port; // To store client port number
} tftp_client_t; 

/* Function prototype for sending file to client */    
void send_file(int sockfd, struct sockaddr_in client_addr, socklen_t client_len, tftp_packet *pkt, tftp_packet *ack);

/* Function prototype for receiving file from client */
void receive_file(int sockfd, struct sockaddr_in client_addr, socklen_t client_len, tftp_packet *pkt, tftp_packet *ack);

/* Function to convert Netascii data from Linux format to Windows format */
void netascii_windows(int fd, tftp_packet *pkt, int size);

/* Function to convert Netascii data from Windows format to Linux format */
void netascii_linux(int fd, tftp_packet *pkt, int size);

#endif 
