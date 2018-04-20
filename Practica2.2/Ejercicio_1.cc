/*
 * Ejercicio_1.cc
 *
 *  Created on: 20/4/2018
 *      Author: usuario_vms
 */


#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <iostream>

using namespace std;



int main(int argc, char* argv[]){



	struct addrinfo hints;
	memset((void*) &hints,'\0', sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;


	struct addrinfo* res;


	int eGetAddr = getaddrinfo(argv[1], NULL, &hints, &res);
	if(eGetAddr != 0){

		cout << "Error getaddrinfo() la dirección no es correcta" << endl;
		return -1;
	}


	char host[NI_MAXHOST];
	char server[NI_MAXSERV];

	while(res->ai_next != NULL){

	       int eGetName = getnameinfo(res->ai_addr, res->ai_addrlen,host, NI_MAXHOST,server, NI_MAXSERV, NI_NUMERICHOST);
	       if(eGetName != 0){
	    	   cout << "Error getnameinfo() no se ha podido resolver el nombre" << endl;
	    	   		return -1;
	       }

	       std::cout << host << " " << res->ai_family << " " << " " << res->ai_socktype << endl;
	       res = res->ai_next;
	}


	return 0;

}

