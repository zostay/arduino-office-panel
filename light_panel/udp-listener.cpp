#include "udp-listener.h"

#include <sys/fcntl.h>
#include <unistd.h>
#include <iostream>

std::ostream& operator<< (std::ostream& os, UdpListenerException& x) {
    return os << x.get_message();
}

std::string operator+ (std::string& str, UdpListenerException& x) {
    std::string result = str + x.get_message();
    return result;
}

UdpListener::UdpListener(int port) {
    listening_socket = 0;

    memset(reinterpret_cast<char*>(&local_addr), 0, sizeof(local_addr));

    local_addr.sin_family      = AF_INET;
    local_addr.sin_port        = htons(port);
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
}

UdpListener::~UdpListener() {
    if (listening_socket > 0) close(listening_socket);
}

void UdpListener::listen() {
    if ((listening_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        throw UdpListenerException("unable to connect to socket", true);

    int flags = fcntl(listening_socket, 0);
    flags |= O_NONBLOCK;
    fcntl(listening_socket, F_SETFL, flags);

    if (bind(listening_socket, reinterpret_cast<sockaddr*>(&local_addr), sizeof(local_addr)) == -1)
        throw UdpListenerException("unable to bind to socket", true);
}

bool UdpListener::message_ready() {
    if (!listening_socket)
        throw UdpListenerException("socket is not open", false);

    timeval zero_wait = { .tv_sec = 0, .tv_usec = 1 };
    fd_set read_sockets;
    FD_ZERO(&read_sockets);
    FD_SET(listening_socket, &read_sockets);
    
    int max_fds = listening_socket + 1;
    int ready = select(max_fds, &read_sockets, 0, 0, &zero_wait);
    if (ready == -1) 
        throw UdpListenerException("failed while waiting for message", true);

    return ready > 0;
}

std::string UdpListener::receive_message() {
    struct sockaddr_in remote_addr;
    socklen_t remote_addr_len = sizeof(remote_addr);

    auto message_buffer = std::make_unique<char>(65535);

    int actual_size = recvfrom(
        listening_socket, 
        reinterpret_cast<void*>(message_buffer.get()), 
        sizeof(unsigned char)*65535, 
        0,
        reinterpret_cast<sockaddr*>(&remote_addr),
        &remote_addr_len
    );

    if (actual_size == -1)
        throw UdpListenerException("failed receiving message", true);

    std::string message(message_buffer.get(), actual_size);
    return message;
}
