#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include "log4cpp/Category.hh"
#include "log4cpp/Appender.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/BasicLayout.hh"
#include "log4cpp/Priority.hh"

#define SERVER_PORT 2301
#define RECEIVE_SIZE 1024

int main() {
	int sock;
	int opt = 1;
	int	listener;
	int bytes_read;
	struct	sockaddr_in	addr;
	char	buf[RECEIVE_SIZE];

	/* В данном проекте дял логгирования информации, приходящей на сервер, используется
	 * библиотека log4cpp. Далее создается логгер
	*/
	log4cpp::Appender *appender1 = new log4cpp::FileAppender("root", "server.log");
	appender1->setLayout(new log4cpp::BasicLayout());
	log4cpp::Category& root = log4cpp::Category::getRoot();
	root.setPriority(log4cpp::Priority::INFO);
	root.addAppender(appender1);

	listener = socket(AF_INET, SOCK_STREAM, 0);
	if (listener < 0) {
		std::cerr << "Error creating socket" << std::endl;
		return (1);
	}
	addr.sin_family = AF_INET;
	addr.sin_port = htons(SERVER_PORT);
	addr.sin_addr.s_addr = htonl(0);

	/* Связываем сокет с адресом и портом прослушиваем их */
	if (bind(listener, (struct sockaddr *) &addr, sizeof(addr)) != 0) {
		std::cerr << "Error binding socket" << std::endl;
		return (2);
	}
	if (listen(listener, SOMAXCONN) != 0) {
		std::cerr << "Error listening socket" << std::endl;
		return (3);
	}
	setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	/* Принимаем вхдящие соединения до завершения программы */
	while (1) {
		sock = accept(listener, nullptr, nullptr);
		if (sock < 0) {
			std::cerr << "Accept error" << std::endl;
			exit(3);
		}
		bytes_read = recv(sock, buf, RECEIVE_SIZE, 0);
		if (bytes_read <= 0) {
			continue;
		}
		buf[bytes_read - 1] = '\0';
		if (strcmp(buf, "START") != 0) {
			root.error("Protocol Error");
			close(sock);
			continue;
		}
		root.info("START");
		while (1) {
			bytes_read = recv(sock, buf, RECEIVE_SIZE - 1, 0);
			buf[bytes_read - 1] = '\0';
			if (bytes_read <= 0) {
				break;
			}
			root.info(buf);
			if (strcmp(buf, "STOP") == 0) {
				break;
			}
		}
		close(sock);
	}
}
