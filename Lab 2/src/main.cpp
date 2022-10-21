#include <iostream>
#include <unistd.h>
#include <string>


std::string spaces(std::string s) {
	std::string out = "";
	char prev = 'a';
	for (int i = 0; i < s.size(); ++i) {
		if (!(prev == ' ' && s[i] == ' ')) {
			out += s[i];
			prev = s[i];
		}
	}

	return out;
}


std::string to_upper(std::string s) {
	for (int i = 0; i < s.size(); ++i) {
		if (s[i] >= 'a' && s[i] <= 'z') {
			s[i] = toupper(s[i]);
		}
	}

	return s;
}


int main() {
	int pipefd_p1[2]; // Pipe for communicating Parent and Child1
	pipe(pipefd_p1);

	int pipefd_12[2]; // Pipe for communicating Child1 and Child2
	pipe(pipefd_12);

	int pipefd_2p[2]; // Pipe for communicating Child2 and Parent
	pipe(pipefd_2p);

	int id_1 = fork();

	if (id_1 == -1) {
		return -1;
	} else if (id_1 == 0) {        // Child 1
		std::string s_1;
		read(pipefd_p1[0], &s_1, sizeof(std::string));
		std::cout << "Child 1 in: " << s_1 << "\n";
		close(pipefd_p1[0]);
        close(pipefd_p1[1]);

		s_1 = to_upper(s_1);
		std::cout << "Child 1 out: " << s_1 << "\n";
		write(pipefd_12[1], &s_1, sizeof(std::string));
		close(pipefd_12[0]);
        close(pipefd_12[1]);

        std::cout << '\n';
	} else {
		int id_2 = fork();

		if (id_2 == -1) {
			return -1;
		} else if (id_2 == 0) {    // Child 2
			std::string s_2;
			read(pipefd_12[0], &s_2, sizeof(std::string));
			std::cout << "Child 2 in: " << s_2 << "\n";
			close(pipefd_12[0]);
        	close(pipefd_12[1]);

        	s_2 = spaces(s_2);
        	std::cout << "Child 2 out: " << s_2 << "\n";
        	write(pipefd_2p[1], &s_2, sizeof(std::string));
        	close(pipefd_2p[0]);
        	close(pipefd_2p[1]);

        	std::cout << '\n';
		} else {                   // Parent
			std::string s;
			std::getline(std::cin, s);
			std::cout << "Parent in: " << s << "\n\n";
			write(pipefd_p1[1], &s, sizeof(std::string));
			close(pipefd_p1[0]);
        	close(pipefd_p1[1]);

        	read(pipefd_2p[0], &s, sizeof(std::string));
        	std::cout << "Parent out: " << s << "\n";

        	close(pipefd_2p[0]);
        	close(pipefd_2p[1]);

		}

	}

	return 0;
}
