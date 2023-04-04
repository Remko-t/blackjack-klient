#define WIN32_LEAN_AND_MEAN

#include <ws2tcpip.h>
#include <winsock2.h>
#include <windows.h>
#include <iostream>

using namespace std;

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")
#pragma comment(lib, "advapi32.lib")

class Klient {
private:
	SOCKET socket;
public:
	Klient(SOCKET x) : socket(x) {}

	void wyslstr(string wiadomosc) {
		send(socket, wiadomosc.c_str(), size(wiadomosc), 0);
	}
	string odbstr() {
		char odbior[5000];
		recv(socket, odbior, 5000, 0);
		return string(odbior);
	}
	void wyslint(int wartosc) {
		send(socket, (char*)wartosc, sizeof(wartosc), 0);
	}
	int odbint() {
		int wartosc;
		recv(socket, (char*)&wartosc, sizeof(wartosc), 0);
		return wartosc;
	}
};
void gra(Klient* kl) {



	
}

int main(int argc, char** argv) {

	int Wynik;
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	char recivebuf[DEFAULT_BUFLEN];
	int recivebuflen = DEFAULT_BUFLEN;
	const char* sendbuf = "Test";
	string dane;

	//inicjalizacja winsock

	Wynik = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (Wynik != 0) {
		cout << "Startup error";
	}
	else
		cout << "startup ok" << endl;

	struct addrinfo* wyn = NULL, * wsk = NULL, hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	//adress i port servera

	Wynik = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &wyn);
	if (Wynik != 0) {
		cout << "getaddrinfo error";
		WSACleanup();
	}
	else
		cout << "getaddrinfo ok"<<endl;

	// próby po³¹czenia a¿ siê uda

	for (wsk = wyn; wsk != NULL; wsk->ai_next) {

		//Tworzenie socketu do po³¹czenia z serwerem

		ConnectSocket = socket(wsk->ai_family, wsk->ai_socktype, wsk->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			cout << "socket error" << WSAGetLastError();
			WSACleanup();
		}

		// Po³¹czenie

		Wynik = connect(ConnectSocket, wsk->ai_addr, (int)wsk->ai_addrlen);
		if (Wynik == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}
	freeaddrinfo(wyn);
	if (ConnectSocket == INVALID_SOCKET) {
		cout << "Nieudalo sie polaczyc";
		WSACleanup();
	}
	else
		cout << "connect ok"<<endl;

	//Wysy³anie inicjalizera

	Klient klient(ConnectSocket);
	gra(&klient);

	//Zamykanie

	Wynik = shutdown(ConnectSocket, SD_SEND);
	if (Wynik == SOCKET_ERROR) {
		cout << "Shutdown error" << WSAGetLastError();
		closesocket(ConnectSocket);
		WSACleanup();
	}
	

	closesocket(ConnectSocket);
	WSACleanup();
}
