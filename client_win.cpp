#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

constexpr int PORT = 8080;

void receive_messages(SOCKET sock) {
    char buffer[1024];
    int bytes_received;

    while (true) {
        bytes_received = recv(sock, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) break;
        std::cout << "Server: " << std::string(buffer, bytes_received) << "\n";
    }
}

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed\n";
        WSACleanup();
        return 1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    std::thread recv_thread(receive_messages, sock);

    std::string msg;
    while (true) {
        std::getline(std::cin, msg);
        if (msg == "exit") break;
        send(sock, msg.c_str(), msg.size(), 0);
    }

    closesocket(sock);
    recv_thread.join();
    WSACleanup();

    return 0;
}
