/* Common file for server & client*/

#ifndef TFTP_H
#define TFTP_H

/* Header files for socket programming, file handling, etc. */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>

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

/* Define TFTP packet structure */
typedef struct
{
    uint16_t opcode; // Operation code (RRQ, WRQ, DATA, ACK, ERROR)
    int Client_OS;   // To store client OS information
    int Server_OS;   // To store server OS information
    int mode;        // To store mode of transfer (DEFAULT, OCTET, NETASCII)
    union            /* Packet body for different TFTP operations */
    {
        struct
        { // WRQ / RRQ
            char filename[256];
        } request;

        struct
        {
            uint16_t block;     // Block number
            uint16_t size;      // Size of the data being sent
            char data[DEFAULT]; // Data buffer (size depends on mode)
            int data_size;      // To store the actual size of data read from file
        } data;

        struct
        {
            uint16_t block; // Block number being acknowledged
            int data_size;  // To store the size of data being acknowledged
        } ack;

    } body;
} tftp_packet;
#pragma pack(pop)

#endif // TFTP_H
