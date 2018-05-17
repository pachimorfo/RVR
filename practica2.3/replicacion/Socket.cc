#include "Socket.h"
#include "Serializable.h"
#include <errno.h>
#include <string.h>
#include <stdexcept>
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Implementación Socket
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void printError(){
	char errStr[100];
	char* errNum;
	errNum = strerror_r(errno,errStr, 100);

	std::cout << errStr << errNum << std::endl;

}



bool operator== (const Socket &s1, const Socket &s2)
{

	char host1[NI_MAXHOST];
	char server1[NI_MAXSERV];

	char host2[NI_MAXHOST];
	char server2[NI_MAXSERV];

	int gNumber = getnameinfo(&s1.sa,s1.sa_len,host1, NI_MAXHOST,
			server1, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

	gNumber = getnameinfo(&s2.sa,s1.sa_len,host2, NI_MAXHOST,
			server2, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);


	if(s1.sa.sa_family != s2.sa.sa_family)
		return false;

	if(host1 != host2)
		return false;

	if(server1 != server2)
		return false;

	return true;
}

std::ostream& operator<<(std::ostream& os, const Socket& s)
{
    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];

    getnameinfo((struct sockaddr *) &(s.sa), s.sa_len, host, NI_MAXHOST, serv,
                NI_MAXSERV, NI_NUMERICHOST);

    os << host << ":" << serv;
}

// ----------------------------------------------------------------------------




Socket::Socket(const char * address, const char * port):sd(-1)
{


	struct addrinfo hints;
	memset((void*) &hints,'\0', sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;

	struct addrinfo* res;

	int eGetAddr = getaddrinfo(address, port, &hints, &res);
	if(eGetAddr != 0){
		printError();
		throw new std::runtime_error("Error getaddrinfo");
	}

	sd = socket(res->ai_family, res->ai_socktype, 0);

	if(sd == -1){
		printError();
		throw new std::runtime_error("Error socket");
	}



}

// ----------------------------------------------------------------------------

int Socket::bind()
{
    return ::bind(sd, (const struct sockaddr *) &sa, sa_len);
}

// ----------------------------------------------------------------------------
/**
     *  Envía un mensaje de aplicación definido por un objeto Serializable.
     *  NOTA:La función espera que el objeto esté serializado.
     *
     *    @param obj en el que se enviará por la red
     *
     *    @param sock con la dirección y puerto destino
     *
     *    @return 0 en caso de éxito o -1 si error
     */
int Socket::send(Serializable * obj, Socket * sock)
{

	//llega serializado o lo serializamos aqui

	int s = sendto(sock->sd,(void*)obj->_data,obj->_size,0,&sock->sa,sock->sa_len);

	if(s == -1){
		printError();
		return -1;
	}

	return 0;
}

// ----------------------------------------------------------------------------

int Socket::recv(char * buffer, Socket * sock)
{


	struct sockaddr src;
	socklen_t size = sizeof(src);
	buffer = (char*)malloc(MAX_MESSAGE_SIZE);
	char host[NI_MAXHOST];
	char server[NI_MAXSERV];

	if(sock != 0){

		int eGetName = getnameinfo(&src, size,host, NI_MAXHOST,
				server, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
		if(eGetName != 0){
			printError();
			return -1;
		}
		sock = new Socket(host,server);
		ssize_t bytesRec =  recvfrom(sock->sd, (void* )&buffer,
		    			MAX_MESSAGE_SIZE, 0, &src, &size);
		if(bytesRec == -1){
			printError();
			return -1;
		}
	}else{

    	ssize_t bytesRec =  recvfrom(sock->sd, (void* )&buffer,
    			MAX_MESSAGE_SIZE, 0, &src, &size);

    	if(bytesRec == -1){
    		printError();
    		return -1;
    	}
    	//Serializable::from_bin(buffer); mas tarde si eso.

	}


	return 0;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

