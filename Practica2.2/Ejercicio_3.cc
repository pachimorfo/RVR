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
	hints.ai_socktype = SOCK_DGRAM;

	struct addrinfo* res;


	int eGetAddr = getaddrinfo(argv[1], argv[2], &hints, &res);
	if(eGetAddr != 0){

		cout << "Error getaddrinfo() la dirección no es correcta" << endl;
		return -1;
	}

	
	int udp_socket = socket(res->ai_family, res->ai_socktype, 0);
	
	if(udp_socket == -1){
		printError();
		return -1;
	}
	

	
		
	struct sockaddr src;
    socklen_t size = sizeof(src);
    char buf[256];
    memset((void*) buf,'\0', 256);
    buf[0] = argv[3][0];
    
   
    
	ssize_t bytesSent = sendto(udp_socket,buf,256,0, res->ai_addr, res->ai_addrlen);
	
	if(buf[0] = 'q'){
		cout << "Finalizando conexion" << endl;	
		return -1;	
	}
		
	if(bytesSent == -1){
		printError();		
	}
	
	
	ssize_t bytesRec =  recvfrom(udp_socket, &buf, 255, 0, &src, &size);
	
	char host[NI_MAXHOST];
	char server[NI_MAXSERV];

	int eGetName = getnameinfo(&src, size,host, NI_MAXHOST,server, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
	if(eGetName != 0){
	   cout << "Error getnameinfo() no se ha podido resolver el nombre" << endl;
			return -1;
	}
	freeaddrinfo(res);
	
	cout << bytesRec << " bytes de " << host << ":" << server << endl;

	
	cout << buf << endl;   


	return 0;

}


void printError(){
	char errStr[100];
	char* errNum;
	errNum = strerror_r(errno,errStr, 100);

	std::cout << errStr << errNum << std::endl;


}





