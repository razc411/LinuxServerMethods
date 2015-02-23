# LinuxServerMethods
A project comparing select, edge triggering and simple threading as the core of a server.

Arguments
*       p <port>        - port for the server to be established on or the client to send to.     DEFAULT: 5001
*       t <num threads> - number of threads for the threaded servers to run on.                  DEFAULT: 4
*       h <hostname>    - host to send data to for the client.                                   DEFAULT: LOCALHOST
*       d <data size>   - the amount of data in bytes for each client to send.                   DEFAULT: 1000000
*       c <num clients> - the number of clients to create :                                      DEFAULT: 1
*       
*       1 of these arguments is required to run
*       e - runs the edge triggered threaded server.
*       w - runs the level triggered non threaded server.
*       l - runs the level triggered threaded server.
*       C - runs the client.

Compiling
cd src
g++ -pthread -std=c+=11 *.cpp -o cscomp

Running
./cscomp arguments

More detailed instructions available in the documentation.