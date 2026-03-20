#ifndef TFTP_CLIENT_H
#define TFTP_CLIENT_H

/* Note: tftp_client_t struct is defined in tftp.h (common header)
   This file only contains client-specific function prototypes */

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
