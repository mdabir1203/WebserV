#ifndef ERRORPAGES_HPP
#define ERRORPAGES_HPP

#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main() {
    const char* hostname = "www.example.com"; // Replace with the client's hostname
    struct addrinfo hints, *res;
    int status;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // use AF_INET6 to force IPv6
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(hostname, NULL, &hints, &res)) != 0) {
        std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
        return 1;
    }

    for (struct addrinfo* p = res; p != NULL; p = p->ai_next) {
        char ipstr[INET6_ADDRSTRLEN];
        void* addr;
        if (p->ai_family == AF_INET) {
            struct sockaddr_in* ipv4 = (struct sockaddr_in*)p->ai_addr;
            addr = &(ipv4->sin_addr);
        } else {
            struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)p->ai_addr;
            addr = &(ipv6->sin6_addr);
        }
        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
        std::cout << "IP address: " << ipstr << std::endl;
    }

    freeaddrinfo(res); // free the linked list
    return 0;
}


#endif /* ERRORPAGES_HPP */
