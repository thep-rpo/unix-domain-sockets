# Unix Domain Sockets: Client-Server IPC

A Linux system programming project demonstrating Inter-Process Communication (IPC) using Unix Domain Sockets (`AF_UNIX`). This project features a multithreaded server capable of handling multiple client connections concurrently on the same machine.

## Features
* **Multithreaded Server:** The server listens on `/tmp/unix_socket` and spawns a detached POSIX thread for every incoming client connection.
* **Data Processing:** The server receives a dynamically sized sequence of integers, calculates their average, and returns a validation message ("Sequence OK" if the average is > 20, else "Check failed").
* **Interactive Client:** A client application that connects to the server's socket path, prompts the user for sequences of numbers, and displays the server's processed response.

## Build and Run Instructions

This project requires a POSIX-compliant operating system (Linux, macOS, or WSL) and a C compiler. 

### Compilation
Open your terminal and compile the client and server separately. **Note:** The server requires the `-pthread` flag.
```bash
gcc src/client.c -o client
gcc src/server.c -o server -pthread
