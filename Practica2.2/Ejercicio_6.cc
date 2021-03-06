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
#include <pthread.h>


using namespace std;



class ServerThread{
public:
	ServerThread(int sock):socket(sock), connected(true){};
	~ServerThread(){};

	void doMsg(){

		 while(connected){

			char buf[256];
			struct sockaddr src;
			socklen_t size = sizeof(src);

		    	//Receive
		    	ssize_t bytesRec =  recvfrom(socket, &buf, 255, 0, &src, &size);

		    	char host[NI_MAXHOST];
			char server[NI_MAXSERV];

			int eGetName = getnameinfo(&src, size,host, NI_MAXHOST,server, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
			if(eGetName != 0){
			   cout << "Error getnameinfo() no se ha podido resolver el nombre" << endl;
					connected = false;
			}

			cout << bytesRec << " bytes de " << host << ":" << server << endl;


			struct tm* timeT;
			time_t t;
			char c = buf[0];
			memset((void*) buf,'\0', 256);
			switch(c){
				case 't':
					 cout << "Enviando hora" << endl;

					time(&t);
					timeT = localtime(&t);
					strftime(buf, 256, "%H-%M-%S\n", timeT);
					sendto(socket,buf,256,0, &src,size);

					break;
				case 'd':
					 cout << "Enviando fecha" << endl;

					time(&t);
					timeT = localtime(&t);
					strftime(buf, 256, "%Y-%m-%d\n", timeT);
					sendto(socket,buf,256,0, &src,size);

					break;				
				default:
					break;



				}
			std::cout << pthread_self() << std::endl;

			sleep(10);

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
	//pthread_exit(ret);
	delete _st;
	return 0;
}
#ifdef __cplusplus
}
#endif
















void printError();

const int N_THREADS = 5;

int main(int argc, char* argv[]){

	//Main thread

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



    int bindError = bind(udp_socket, res->ai_addr,res->ai_addrlen);

    if(bindError == -1){
    	printError();
    	return -1;
    }



	freeaddrinfo(res);



	//Threads

	pthread_t idThreads[N_THREADS];

	for(int i = 0; i < N_THREADS; i++){

		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

		ServerThread* st = new ServerThread(udp_socket);

		int threadError = pthread_create(&idThreads[i], &attr, startRoutine, static_cast<void*>(st));

		if(threadError != 0){
			std::cout << "Error al crear thread" << std::endl;
			return -1;
		}

	}
	char c = 'c';
	while(c != 'q'){
	   	
		std::cin >> c;
		
	}
	cout << "Conexión finalizada" << endl;
	/*for(int i = 0; i < N_THREADS; i++){


	}*///cerrar hilos
	close(udp_socket);





	return 0;

}


void printError(){
	char errStr[100];
	char* errNum;
	errNum = strerror_r(errno,errStr, 100);

	std::cout << errStr << errNum << std::endl;


}






