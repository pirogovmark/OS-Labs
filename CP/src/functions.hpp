#include <string>
#include <vector>


// Send a message to the server in a convenient form - login$recipient$message
void send_message_to_server(int fd, std::string curlogin, std::string user, std::string message) {
    std::string text = curlogin + "$" + user + "$" + message;
    int k = text.size();
    write(fd, &k, sizeof(k));
    char messagec[k];
    for (int i = 0; i < k; ++i) {
        messagec[i] = text[i];
    }
    write(fd, messagec, k);
}


// Send a message to a client
void send_message_to_client(int fd, std::string message) {
    std::string text = message;
    int k = text.size();
    write(fd, &k, sizeof(k));
    char messagec[k];
    for (int i = 0; i < k; ++i) {
        messagec[i] = text[i];
    }
    write(fd, messagec, k);
}


// Get the message in a client-friendly form
std::string recieve_message_client(int fd) {
    int size;
    read(fd, &size, sizeof(size));
    char messagec[size];
    read(fd, messagec, size);
    std::string recv;
    for (int i = 0; i < size; ++i) {
        if (messagec[i] != '$') {
            recv.push_back(messagec[i]);
        } else {
            recv = recv + ": ";
        }
    }
    return recv;
}


// Get the message in a server-friendly form
std::string recieve_message_server(int fd) {
    int size;
    read(fd, &size, sizeof(size));
    char messagec[size];
    read(fd, messagec, size);
    std::string recv;
    for (int i = 0; i < size; ++i) {
        recv.push_back(messagec[i]);
    }
    return recv;
}


// Get the login from the message for the server
std::string extract_login(std::string message) {
    std::string login;
    int i = 0;
    while (message[i] != '$') {
        login.push_back(message[i]);
        ++i;
    }
    return login;
}


// Get a message to the client
std::string extract_message(std::string message) {
    std::string text, text1, text2;
    int i = 0;
    while (message[i] != '$') {
        text1.push_back(message[i]);
        ++i;
    }
    ++i;
    while (message[i] != '$') {
        ++i;
    }
    while (i < message.size()) {
        text2.push_back(message[i]);
        ++i;
    }
    text = text1 + text2;
    return text;
}


// Get the message recipient
std::string extract_addressee(std::string message) {
    std::string text;
    int i = 0;
    while (message[i] != '$') {
        ++i;
    }
    ++i;
    while (message[i] != '$') {
        text.push_back(message[i]);
        ++i;
    }
    return text;
}


// Get message text
std::string extract_text(std::string message) {
    std::string text;
    int i = 0;
    while (message[i] != '$') ++i;
    ++i;
    while (message[i] != '$') ++i;
    ++i;
    ++i;
    while (i < message.size()) {
        text.push_back(message[i]);
        ++i;
    }
    return text;
}


// Searching for a substring in a string using the Knuth-Morris-Pratt algorithm
std::vector<int> prefix_function(std::string s) {
    int n = s.size();
    std::vector<int> pi(n);
    for (int i = 1; i < n; i++) {
        int j = pi[i - 1];
        while (j > 0 && s[i] != s[j]) {
            j = pi[j - 1];
        }
        if (s[i] == s[j]) pi[i] = j + 1;
    }
    return pi;
}


bool search(std::string t, std::string p) {
    std::vector<int> v = prefix_function(p);
    int l = 0;
    int k = 0;
    while (k < t.size()) {
        if (t[k] == p[l]) {
            k++;
            l++;
            if (l == p.size()) return true;
        } else if (l == 0) {
            k++;
        } else {
            l = v[l - 1];
        }
    }
    return false;
}
