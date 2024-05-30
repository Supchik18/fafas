#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

using namespace std;

int main()
{
	WSADATA wsaData; /*Это переменная типа WSADATA, которая используется для хранения информации о версии Winsock, реализованной в операционной системе.*/
	ADDRINFO hints; /*Это переменная типа ADDRINFO, которая используется для хранения информации о параметрах подключения, таких как семейство адресов, тип сокета, протокол и прочее.*/
	ADDRINFO* addrResult;/*Это указатель на переменную типа ADDRINFO, который будет использоваться для хранения результата поиска адресной информации.*/
	SOCKET ClientSocket = INVALID_SOCKET;/*Это переменная типа SOCKET, которая будет использоваться для хранения идентификатора клиентского сокета. Она инициализируется значением INVALID_SOCKET.*/
	SOCKET ListenSocket = INVALID_SOCKET;/*Это переменная типа SOCKET, которая будет использоваться для хранения идентификатора серверного (прослушивающего) сокета. Она также инициализируется значением INVALID_SOCKET*/
	SetConsoleCP(1251);
	setlocale(LC_ALL, "Russian");/*Перевод на русский язык*/


	const char* sendBuffer = "Hello from Server";/*Сообщение*/
	char recvBuffer[512]; /*Размер сообщения в байтах*/

	int result = WSAStartup(MAKEWORD(2, 2), &wsaData); /*Эта строка вызывает функцию WSAStartup(), которая инициализирует использование Winsock (Windows Sockets) в программе.*/ /*MAKEWORD(2, 2) создает двухбайтовое значение, указывающее версию Winsock, которую мы хотим использовать (в данном случае версию 2.2).
   - &wsaData - это указатель на переменную WSADATA, которая будет заполнена информацией о версии Winsock, доступной в системе.
   - Результат выполнения функции сохраняется в переменной result.*/
	if (result != 0) {
		cout << "WSAStartup failed" << endl;
		return 1;
	}/*Эта строка проверяет, был ли результат вызова WSAStartup() успешным (возвращает 0) или нет.
   - Если result не равен 0, то это означает, что инициализация Winsock не удалась, и выводится сообщение об ошибке.
   - В случае ошибки, программа возвращает 1, указывая на неудачный запуск*/
	ZeroMemory(&hints, sizeof(hints));/* Эта строка очищает содержимое структуры hints (обнуляет ее) с помощью функции ZeroMemory().
   - Это важно, чтобы убедиться, что в структуре нет мусорных значений, которые могут повлиять на дальнейшую работу программы.*/
	hints.ai_family = AF_INET;/*Здесь мы устанавливаем поле ai_family структуры hints в значение AF_INET, что указывает на использование IPv4-адресов.*/
	hints.ai_socktype = SOCK_STREAM;/*Здесь мы устанавливаем поле ai_socktype структуры hints в значение SOCK_STREAM, что указывает на использование потокового сокета (TCP).*/
	hints.ai_protocol = IPPROTO_TCP;/*Здесь мы устанавливаем поле ai_protocol структуры hints в значение IPPROTO_TCP, что указывает на использование протокола TCP.
*/
	hints.ai_flags = AI_PASSIVE;/*Здесь мы устанавливаем поле ai_flags структуры hints в значение AI_PASSIVE, что означает, что сокет будет использоваться в качестве серверного (прослушивающего) сокета.*/

	result = getaddrinfo(NULL, "788", &hints, &addrResult);
	if (result != 0) {
		cout << "getaddrinfo failed" << endl;
		return 1;
	}/*Основная идея этого фрагмента - найти адресную информацию для сетевого сокета, который будет прослушивать порт 788.*/



	ListenSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		cout << "socket creation with error" << endl;
		freeaddrinfo(addrResult);
		return 1;
	}/*Основная идея - создать сокет для прослушивания подключений, используя данные, полученные ранее. Если создание сокета не удалось, программа завершается с ошибкой.
*/
	result = bind(ListenSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen);
	if (result == SOCKET_ERROR) {
		cout << "biging connect failed" << endl;
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}/*привязать сокет к адресу, полученному ранее. Если привязка не удалась, программа завершается с ошибкой.*/

	result = listen(ListenSocket, SOMAXCONN);
	if (result == SOCKET_ERROR) {
		cout << "Listening Failed" << endl;
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}/*перевести сокет в режим прослушивания входящих подключений. Если прослушивание не удалось, программа завершается с ошибкой.*/


	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (result == SOCKET_ERROR) {
		cout << "Accepting Failed" << endl;
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}
	closesocket(ListenSocket);/*принять входящее подключение на сокете ListenSocket, создав новый сокет ClientSocket для обработки этого подключения. Если принятие подключения не удалось, программа завершается с ошибкой. Затем сокет ListenSocket закрывается.*/
	
	
	
	/*result = send(ClientSocket, sendBuffer, (int)strlen(sendBuffer), MSG_DONTROUTE);
	if (result == SOCKET_ERROR) {
	 cout << "send Failed" << endl;
	 freeaddrinfo(addrResult);
	 WSACleanup();
	 return 1;
	}
	cout << "Sent " << result << " bytes";

	result = shutdown(ClientSocket, SD_SEND);
	if (result == SOCKET_ERROR) {
	 cout << "shutdown error" << endl;
	 freeaddrinfo(addrResult);
	 WSACleanup();
	 return 1;
	}*/


	do {
		ZeroMemory(recvBuffer, 512);
		result = recv(ClientSocket, recvBuffer, 512, 0);
		if (result > 0) {
			cout << "Получено" << result << "байт" << endl;
			cout << "Получено" << recvBuffer << endl;

			result = send(ClientSocket, sendBuffer, (int)strlen(sendBuffer), MSG_DONTROUTE);
			if (result == SOCKET_ERROR) {
				cout << "send Failed" << endl;
				freeaddrinfo(addrResult);
				WSACleanup();
				return 1;
			}
		}/**/
		else if (result == 0) {
			cout << "Connection closed" << endl;
		}
		else {
			cout << "Сообщение не получено" << endl;
		}/*циклически принимать данные от клиента и отправлять ответ. При ошибке отправки или приема данных программа завершается с ошибкой.
*/
	} while (result > 0);
	result = shutdown(ClientSocket, SD_SEND);
	if (result == SOCKET_ERROR) {
		cout << "shutdown error" << endl;
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}/*после завершения цикла приема и отправки данных, выполнить частичное закрытие сокета, закрыть сокет полностью, освободить память и завершить работу с библиотекой Windows Sockets. Возвращается 0, если все прошло успешно, иначе 1 (в случае ошибки).
*/
	closesocket(ClientSocket);/*Закрывает сокет ClientSocket.*/
	freeaddrinfo(addrResult);/* Освобождает память, занятую структурой addrResult.*/
	WSACleanup();/* Очищает библиотеку Windows Sockets.*/
	return 0;/* Возвращает 0, указывая на успешное завершение.*/
}
/*1. shotdown - частичное или полное закрытие соединения
2. accept - принятие входящего соединения со стороны клиент сокет
3. bind - привязка адреса и порта к конкретному сокету
4. listen - системный вызов, который прослушивает подключения
5. Вызвать системный вызов WSACleanup
6. Вызвать системный вызов freeaddrinfo()
7. Функция getaddrinfo разрешает адрес и заполняет структуру addrinfo, которая будет использоваться при создании сокета
8. Асинхронные операции выполняются одновременно с другими операциями. Программа не ждет выполнения операции, а переходит к другой
9. Синхронные операции выполняются последовательно, одна за другой. Это означает, что программа ожидает завершения текущей операции
10. Потому что клиент подключается к серверу и передает на него сообщения, а сервер действует только в ответ на действия клиента (принимает подключение, принимает сообщения, отправляет сообщения только в ответ)
11. Мы не используем флаги потому, что нам достаточно стандартных функций и настроек
12. SOMAXCONN - это константа, которая представляет максимально допустимое значение для данной платформы.
13. Закрытие отправления данных с сокета
14. TCP - отправка данных с подтверждением, UDP - отправка без подтверждения
15. При помощи системного вызова listen
16. Сначала инициализируем WinSock, далее задаем ему необходимые параметры(тип сокета, проток передачи, тип IP), разрешаем адрес и порт и инициализируем сокет.
17. Буфер для приема используется для временного хранения данных, что позволяет сохранять целостность данных так как данные могут приходить в любом порядке или разбиваться.
18. IP-адрес и порт идентифицируют узлы и приложения в сети. IP-адрес указывает на конкретное устройство в сети, а порт определяет конкретное приложение на этом устройстве.
19. Сокеты – это абстракция, предоставляемая операционной системой для взаимодействия с сетью.
20. Connect - это системный вызов для установления соединения с ServerSocket
21. bind
22. getaddrinfo*/