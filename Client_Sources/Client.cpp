#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int	main() {
	int sock;
	char *PORT;
	struct sockaddr_in	addr;

	/* Получаем порт сервера из переменной окружения */
	PORT = getenv("SERVER_PORT");

	/* Создаем сокет клиента */
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		std::cerr << "Error creating socket" << std::endl;
		return (1);
	}
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(PORT));
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	/* Создаем соединение с сервером и в случае успеха отправляем тестовую последовательность строк */
	if (connect(sock, (struct sockaddr *) &addr, sizeof(addr)) == 0) {
		send(sock, "START", strlen("START") + 1, 0);
		sleep(1);
		send(sock, "I'm student", strlen("I'm student") + 1, 0);
		sleep(1);
		send(sock, "Testing server", strlen("Testing server") + 1, 0);
		sleep(1);
		send(sock, "STOP", strlen("STOP") + 1, 0);
	}
	else {
		std::cerr << "Error connecting to server" << std::endl;
		return (2);
	}
	return 0;
}