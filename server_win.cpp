#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <thread>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

constexpr int PORT = 8080;

void handle_client(SOCKET client_socket) {
    char buffer[1024];
    int bytes_received;

    while (true) {
        bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received == SOCKET_ERROR || bytes_received == 0) {
            std::cout << "Client disconnected\n";
            closesocket(client_socket);
            break;
        }

        std::string msg(buffer, bytes_received);
        std::cout << "Received: " << msg << "\n";

        send(client_socket, buffer, bytes_received, 0);
    }
}

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed\n";
        WSACleanup();
        return 1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed\n";
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed\n";
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server listening on port " << PORT << "\n";

    std::vector<std::thread> threads;

    while (true) {
        sockaddr_in client_addr{};
        int client_size = sizeof(client_addr);

        SOCKET client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_size);
        if (client_socket == INVALID_SOCKET) {
            std::cerr << "Accept failed\n";
            continue;
        }

        std::cout << "Client connected\n";
        threads.emplace_back(handle_client, client_socket);
    }

    for (auto& t : threads) {
        if (t.joinable()) t.join();
    }

    closesocket(server_socket);
    WSACleanup();

    return 0;
}
