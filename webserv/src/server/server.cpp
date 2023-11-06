#include <iostream>
#include <string>


// TODO: I want a http server part where it can handle file upload

class Server {
public:
    Server() {
        std::cout << "Server constructor" << std::endl;
    }

    ~Server() {
        std::cout << "Server destructor" << std::endl;
    }

    void start() {
        std::cout << "Server start" << std::endl;
    }

    void stop() {
        std::cout << "Server stop" << std::endl;
    }

    void restart() {
        std::cout << "Server restart" << std::endl;
    }
// server 
    void uploadFile(std::string filename) {
        std::cout << "Server uploadFile: " << filename << std::endl;
    }

    void downloadFile(std::string filename) {
        std::cout << "Server downloadFile: " << filename << std::endl;
    }
};