# SIMPLE SERVER
- `simple_srvr` is a implementation of a tcp-based server software that accepts commands from clients and sends out to lower-ranking clients for execution

| File | Description |
| ----- | ---------- |
| [main](./src/srvr.c) | Driver Function for the server|
| [comms](./src/comms.c) | Communications Module |
| [conns](./src/conn.c) | Connections Module |
| [err](./src/err.c) | Module to handle errors
 
## Description

### General
- TCP-based
- First Come First Serve ranking of clients
- Command Execution

The simple server is a simple implementation of a server based on the [tcp](https://en.wikipedia.org/wiki/Transmission_Control_Protocol) protocol.  

This particular implementation leverages on the [sytem calls](https://www.geeksforgeeks.org/introduction-of-system-call/) to build a [unix domain socket](https://en.wikipedia.org/wiki/Unix_domain_socket).  

The IPC is facilitated via a [file](./src/srvr.h/#22) on the file system

### Core Functionality
The server's [connection](./src/conn.c) module handles connections and requisite structures, connection lists and tables.  
The default structure is a linked list of connections which serves to order the the connections in an linear fashion with felexibity of removal and addition 

The [communications module](./src/comms.c) holds communication functionality and requisite strucures that map connected clients and pair sockets used to reach them from the server

The [errors modules](./src/err.c) provides a simple interface for reporiting error

## Use
### Requirements
`gcc` compiler  
## Compile
compile at the `src` folder  
`gcc -Wall -Wexta -Werror -pedantic -std=c89 comms.c conn.c err.c srvr.c -o srvr`
