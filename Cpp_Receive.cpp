#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <unistd.h>

int main(int argc, char* argv[]) {
    // Create a UDP socket for receiving
    int inSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (inSocket < 0) {
        perror("unable to create socket");
        return 1;
    }

    // Set up local address to bind to
    sockaddr_in address{};
    address.sin_family = AF_INET; // IPv4
    if (argc > 1) {
        address.sin_port = htons(atoi(argv[1]));
    } else {
        address.sin_port = htons(12345); // Bind to port 12345
    }
    address.sin_addr.s_addr = INADDR_ANY; // Listen on 127.0.0.1

    // Bind the socket to the specified port and address
    if (bind(inSocket, (sockaddr*)&address, sizeof(address)) < 0) {
        perror("unable to bind");
        return 1;
    }

    char buffer[10];
    int value = 0;

    // Loop t o receive incoming datagrams
    while (true) {
        // Receive a datagram (blocking)
        ssize_t len = recv(inSocket, buffer, sizeof(buffer) - 1, 0);
        if (len > 0) {
            buffer[len] = '\0'; // Null-terminate the string
            // Convert to int
            value = atoi(buffer);
            std::cout << value << std::endl; // Print received message
        }
    }

    // Cleanup (not reached in infinite loop)
    close(inSocket);
    return 0;
}

