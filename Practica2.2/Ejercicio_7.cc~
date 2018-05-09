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

class ServerThread{
public:
	ServerThread(int sock):socket(sock), connected(true){};
	~ServerThread(){};

	void doMsg(){

		 while(connected){

			char buf[256];        

			//Receive
			ssize_t bytesRec =  recv(socket, &buf, 255, 0);
			if(bytesRec == 0){
				connected = false;
			}
			else {	
				send(socket, &buf, bytesRec, 0);
			}

			std::cout << pthread_self() << std::endl;	


    		}


	};

private:
	int socket;
	bool connected;


};

#ifdef __cplusplus

extern "C" {
#endif
void* startRoutine(void* st)	{
	ServerThread* _st = static_cast<ServerThread*>(st);	
	_st->doMsg();
	//void* ret;
	//pthread_exit(ret);//¿?cerrar hilo
	delete _st;
	return 0;
}
#ifdef __cplusplus
}
#endif



bool server = true;
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
	
	
	freeaddrinfo(res);

	while(server){
		struct sockaddr src;
		socklen_t size = sizeof(src);


		int idSrc = accept(tcp_socket, &src, &size);
		if(idSrc == -1){
			printError();
		}

		pthread_t idThread;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

		ServerThread* st = new ServerThread(idSrc);

		int threadError = pthread_create(&idThread, &attr, startRoutine, static_cast<void*>(st));

		if(threadError != 0){
			std::cout << "Error al crear thread" << std::endl;
			return -1;
		}		

		


	}
	cout << "Conexión finalizada" << endl;

	return 0;

}


void printError(){
	char errStr[100];
	char* errNum;
	errNum = strerror_r(errno,errStr, 100);

	std::cout << errStr << errNum << std::endl;


}





