# 📘 Trivial File Transfer Protocol (TFTP) Implementation in C

This project is a **client-server based implementation** of the **Trivial File Transfer Protocol (TFTP)** developed in **C** using **UDP sockets**.
The main purpose of this project is to understand how simple network protocols work and how file transfer can be achieved using **Linux system calls**.

The project focuses on practical learning rather than theory and helps in building a strong foundation in Linux Internals, networking, and system-level programming.

---

## 🚀 Project Description

**TFTP** is a lightweight file transfer protocol commonly used in embedded systems and network environments because of its simplicity and small footprint.
In this project, a **TFTP client** and **** are implemented to transfer files within a local network using **UDP communication**.

This work was carried out as part of the **Linux Internals module** in the **Embedded Systems Professional Course** at Emertxe Information Technologies.
---

## 🎯 Objective

- To understand client-server communication using UDP
- To implement a simple file transfer protocol in C
- To gain hands-on experience with Linux system calls
- To strengthen fundamentals in network programming and debugging
---

## ✨ Features

- **Client-server based file transfer**
- **Support for file upload (PUT) and file download (GET)**
- **Handles text (netascii) and binary (octet) transfer modes**
- **Structured DATA and ACK packet handling**
- **Simple and readable logging for packet flow and errors**
- **Designed for Linux environments**  

---

## 🛠️ Technologies & Concepts Used

- **C Programming**
- **UDP Socket Programming**
- **Linux System Calls (socket, bind, sendto, recvfrom, fork, read, write)**
- **Client-Server Architecture**
- **File I/O Operations**
- **Basic Networking Concepts**
- **Structured Logging**

---

## 🧠 How the Application Works

- The **client** initiates file transfer requests (**GET / PUT**)

- The **server** listens for incoming requests and processes them accordingly

- Files are transferred in **fixed-size data blocks** with acknowledgment handling

- Transfer mode determines how data is written to or read from files

- Clear log messages help trace packet flow during execution

---

## 📂 Project Structure

```text
TFTP/
├── tftp_client.c
├── tftp_client.h
├── tftp.h {client-side}
├── tftp_server.c
├── tftp.h {client-side}
├── tftp.c
└── README.md

```

---

## ▶️ Compilation and Execution

### Compile and run:

#### COMPILATION:
```bash
gcc tftp.c tftp_server.c -o tftp_server
gcc tftp.c tftp_client.c -o tftp_client

```
#### RUN:
```bash
./tftp_server
./tftp_client
```

## Learning Outcome and Impact

Working on this project helped me understand how UDP-based communication works at a low level.
It strengthened my knowledge of Linux system calls, networking fundamentals, and protocol-based application design.
This project also improved my debugging approach and confidence in implementing system-level applications in C.

---

## Acknowledgment

I would like to sincerely thank Emertxe Information Technologies and the Linux Internals faculty for their guidance and structured learning approach, which helped me complete this project with a clear understanding of the concepts involved.

## GitHub Repository

[https://github.com/Manu-hp1/Trivial-File-Transfer-Protocol]

---

## About Me

I am an **Embedded Systems Trainee** at **Emertxe Information Technologies** with a strong interest in **C programming, Linux Internals, and system-level development**.
I enjoy building **low-level projects** that help me understand how **operating systems and network protocols** work internally.

---

## License

This project is developed for learning and educational purposes.

