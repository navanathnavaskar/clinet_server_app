# clinet_server_app
Application where server starts on specific port and then client connect to it

## To compile server program
gcc swap_server.c -o swap_server

## To compile client program
gcc swap_client.c -o swap_client

## Steps to run applications
1. Open 2 terminal on same server
2. On one terminal start server application using below command


    ./swap_server 8090
3. Now on terminal 2, start client application

   
    ./swap_client 127.0.0.1 8090
4. Send some message from client, server should receive it and send ACK.
5. Client will get ACK and then client can send next message.
6. Client terminates application by entering "exit" on terminal
