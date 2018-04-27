/*
 * Ejercicio_2.cc
 *
 *  Created on: 20/4/2018
 *      Author: usuario_vms
 */



#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <iostream>
#include <errno.h>
#include <unistd.h>

using namespace std;

bool connected = true;
void printError();


int main(int argc, char* argv[]){

	struct addrinfo hints;
	memset((void*) &hints,'\0', sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	struct addrinfo* res;


	int eGetAddr = getaddrinfo(argv[1], argv[2], &hints, &res);
	if(eGetAddr != 0){

		cout << "Error getaddrinfo() la dirección no es correcta" << endl;
		return -1;
	}
	
	char host[NI_MAXHOST];
	char server[NI_MAXSERV];

	int eGetName = getnameinfo(res->ai_addr,res->ai_addrlen ,host, NI_MAXHOST,server, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
	if(eGetName != 0){
	   cout << "Error getnameinfo() no se ha podido resolver el nombre" << endl;
			return -1;
	}
	
	cout << "Conexion establecida: " << host << ":" << server << endl;


	int tcp_socket = socket(res->ai_family, res->ai_socktype, 0);

	if(tcp_socket == -1){
		printError();
		return -1;
	}


	
    int bindError = bind(tcp_socket, res->ai_addr,res->ai_addrlen);

    if(bindError == -1){
    	printError();
    	return -1;
    }

	int listenError = listen(tcp_socket, 15);
	
	if(listenError == -1){
		printError();
	}
	
	
	freeaddrinfo(res)
	while(true){
		struct sockaddr src;
		socklen_t size = sizeof(src);


		int idSrc = accept(tcp_socket, &src, &size);
		
		if(idSrc == -1){
			printError();
		}
		;

		while(connected){



			char buf[256];        

			//Receive
			ssize_t bytesRec =  recv(idSrc, &buf, 255, 0);	
			send(idSrc, &buf, bytesRec, 0);
			
			

		}

		//cout << "Conexión finalizada" << endl;

	}

	return 0;

}


void printError(){
	char errStr[100];
	char* errNum;
	errNum = strerror_r(errno,errStr, 100);

	std::cout << errStr << errNum << std::endl;


}





