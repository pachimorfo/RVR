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

	
	int tcp_socket = socket(res->ai_family, res->ai_socktype, 0);
	
	if(tcp_socket == -1){
		printError();
		return -1;
	}
	

	
		
	struct sockaddr src;
    	socklen_t size = sizeof(src);
    	char buf[256];
    	memset((void*) buf,'\0', 256);
    	
    
   	int connectError =  connect(tcp_socket, res->ai_addr,res->ai_addrlen);
	if(connectError != 0)
	{
		printError();
		return -1;
	}
    
	while(connected){
		cin >> buf;
	
		if(buf[0] == 'Q'){
		
			int closeError = close(tcp_socket);	
			if(closeError != 0){
				printError();
				return -1;
			}
			cout << "Finalizando conexion" << endl;
			connected = false;
			return 0;	
		}
		else{
			ssize_t bytesSent = send(tcp_socket, buf, 256, 0);
		
			if(bytesSent == -1){
				printError();
				return -1;		
			}
	
		}
		ssize_t bytesRec = recv(tcp_socket,buf, 256 , 0);
		cout << buf << endl;   
	}
	freeaddrinfo(res);

	return 0;

}


void printError(){
	char errStr[100];
	char* errNum;
	errNum = strerror_r(errno,errStr, 100);

	std::cout << errStr << errNum << std::endl;


}





