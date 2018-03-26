#ifndef __UDP_LISTENER_H
#define __UDP_LISTENER_H

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <cstdio>
#include <cstring>
#include <string>

class UdpListenerException {
    private:
        std::string message;
        int errno;

        void init(std::string &message, bool syserr) {
            this->message = message;
            if (syserr) {
                errno = sys_nerr;
                this->message += ": ";
                this->message += strerror(errno);
            }
        }

    public:
        UdpListenerException(std::string &message, bool syserr) {
            init(message, syserr);
        }

        UdpListenerException(const char* msg, bool syserr) {
            std::string message(msg);
            init(message, syserr);
        }

        std::string &get_message() { return message; }
        int get_errno() { return errno; }
};

std::ostream& operator<< (std::ostream& os, UdpListenerException& x);
std::string operator+ (std::string& str, UdpListenerException& x);

class UdpListener {
    private:
        int listening_socket;
        sockaddr_in local_addr;

    public:
        UdpListener(int port);
        ~UdpListener();

        void listen();
        bool message_ready();
        std::string receive_message();
};

#endif //__UDP_LISTENER_H
