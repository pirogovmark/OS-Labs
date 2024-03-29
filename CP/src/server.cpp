#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <vector>
#include <fcntl.h>
#include <map>
#include <vector>
#include "functions.hpp"


int in(std::vector<std::string> logins, std::string str) {
    for (int i = 0; i < logins.size(); ++i) {
        if (logins[i] == str) return i;
    }
    return -1;
}


int main() {
    std::vector<std::vector<std::string> > history; // Message history
    std::vector<std::string> logins;
    std::string command;
    std::string login;

    // Login input
    std::cout << "Enter all user's logins. Insert 'end' to stop:\n";
    while (login != "end") {
        std::cin >> login;
        std::vector<std::string> vec;
        vec.push_back(login);
        history.push_back(vec);
        if (in(logins, login) == -1)
            logins.push_back(login);
        else
            std::cout << "already exists!";
    }
    std::cout << "CHAT IS READY. LAUNCH CLIENTS\n";

    // Creating output FIFOs for all logins
    for (int i = 0; i < logins.size(); ++i) {
        if (mkfifo(logins[i].c_str(), 0777) == -1) {
            if (errno != EEXIST) {
                std::cout << "FIFO WAS NOT CREATED";
                exit(1);
            }
        }
    }

    // Creating an input FIFO
    if (mkfifo("input", 0777) == -1) {
        std::cout << "MAIN INPUT FIFO WAS NOT CREATED";
        exit(1);
    }
    int fd_recv = open("input", O_RDWR);
    if (fd_recv == -1) {
        std::cout << "INPUT FIFO WAS NOT OPENED";
        exit(1);
    }

    // Opening all FIFOs for writing
    int fd[logins.size()];
    for (int i = 0; i < logins.size(); ++i) {
        fd[i] = open(logins[i].c_str(), O_RDWR);
    }

    while (1) {

        std::string message;
        message = recieve_message_server(fd_recv);
        std::cout << message;
        std::string rcvd_usr = extract_login(message);          // From who
        std::string rcvd_adressee = extract_addressee(message); // To whom
        std::string rcvd_message = extract_message(message);    // What
        int fd_repl = in(logins, rcvd_adressee);                // Recipient id
        int fd_usr = in(logins, rcvd_usr);                      // Sender id
        int pos = -1;
        if (rcvd_adressee == "history") {
            std::string reply = "No matches found\n";
            for (int i = 0; i < history.size(); ++i) {
                if (i == fd_usr) {
                    for (int j = 0; j < history[i].size(); ++j) {
                        if (search(history[i][j], extract_text(message))) {
                            reply = history[i][j];
                            pos = i;
                        }
                    }
                }
            }
            if (reply != "No matches found\n") {
                for (int i = 0; i < history.size(); ++i) {
                    if (i != pos) {
                        for (int j = 0; j < history[i].size(); ++j) {
                            if (search(history[i][j], extract_text(message))) {
                                reply = "[To " + logins[i] + "] " + history[i][j];
                            }
                        }
                    }
                }
            }
            send_message_to_client(fd[fd_usr], reply);
        } else {
            for (int i = 0; i < history.size(); ++i) {
                if (logins[i] == rcvd_usr)
                    history[i].push_back(extract_text(message));
                if (logins[i] == rcvd_adressee && rcvd_usr != rcvd_adressee)
                    history[i].push_back(extract_text(message));
            }
            if (in(logins, rcvd_adressee) == -1) {
                send_message_to_client(fd[fd_usr], "Login does not exists!\n");
            } else {
                send_message_to_client(fd[fd_repl], rcvd_message);
            }
        }
    }
}
