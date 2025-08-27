# Computer Networks Projects

Hands-on implementations of core networking concepts with **TCP/IP sockets** in C.  
This repository contains simple client-server applications that demonstrate how data is transmitted reliably over networks at the OS level.

---

## 📌 Projects

### 1. TCP Client-Server Chat Application
- Implemented a **two-way communication system** using Berkeley sockets.
- Supports multiple message exchanges between client and server.
- Demonstrates use of `socket()`, `bind()`, `listen()`, `accept()`, `connect()`, `read()`, and `write()` system calls.

### 2. File Transfer (Text File) over TCP
- Built a **file transfer system** where:
  - Client parses and transmits file contents word-by-word to the server.
  - Server receives and reconstructs the file contents.
- Includes error handling for socket operations.
- Highlights concepts like **buffer management, message framing, and connection reliability**.

---

## 🛠️ Tech Stack
- **Language**: C  
- **APIs**: Linux Sockets (Berkeley sockets)  
- **Protocols**: TCP/IP  

---

## 🚀 Getting Started

### Prerequisites
- Linux/Unix environment
- GCC compiler

### Compilation & Execution

#### Compile
```bash
gcc server.c -o server
gcc client.c -o client
````

#### Run

Start the server:

```bash
./server <port>
```

Run the client (in another terminal):

```bash
./client <server_ip> <port>
```

> Example:

```bash
./server 8080
./client 127.0.0.1 8080
```

---

## 📂 Project Structure

```
├── client.c        # Client-side code
├── server.c        # Server-side code
├── glad.txt        # Sample text file for file transfer demo
└── README.md
```

---

## 📖 Learning Outcomes

* Gained practical experience with **low-level system calls** and **socket APIs**.
* Understood **TCP connection establishment** (three-way handshake) and **reliable data transfer**.
* Learned about **buffering, error handling, and client-server models** in C.
