#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <vector>
#include <fcntl.h>
#include "functions.hpp"
#include <thread>


// Message reception function (for the stream)
void func(int fd_recv, std::string login) {
    while (1) {
        std::string reply = recieve_message_client(fd_recv);
        std::cout << reply << "\n";
        std::cout.flush();
        std::cout << login << ">";
        std::cout.flush();
    }
}


int main() {
    // Connecting to the input FIFO server
    int fd_send = open("input", O_RDWR);
    if (fd_send == -1) {
        std::cout << "ERROR: MAIN FIFO WAS NOT OPENED\n";
        exit(1);
    }

    std::cout << "Welcome to chat.\n";
    std::cout << "Follow the instruction below and notice: to switch the user you need to leave this session and re-login via './client'\n";
    std::cout << "Insert your login: ";
    std::string login;

    // Connecting to a personal named pipe
    int fd_recv = -1;
    while (fd_recv == -1) {
        std::cin >> login;
        fd_recv = open(login.c_str(), O_RDWR);
        if (fd_recv == -1) {
            std::cout << "Wrong login!\nTry again: ";
        }
    };

    // Login successful, the stream of messages from the server is started
    std::string adressee, message;
    std::cout << "Congrats! You have signed in chat.\n";
    std::thread thr_recieve(func, fd_recv, login);

    // Start a cycle of messages to the server
    while (true) {
        std::cout << login << "> ";
        std::cin >> adressee;
        if (adressee == "history") {
            std::string pattern;
            std::getline(std::cin, pattern);
            send_message_to_server(fd_send, login, adressee, pattern);
        } else {
            if (adressee == "quit")
                break;
            std::getline(std::cin, message);
            send_message_to_server(fd_send, login, adressee, message);
        }
    }

    thr_recieve.detach();
}
