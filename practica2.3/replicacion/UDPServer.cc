#include "UDPServer.h"
#include "Serializable.h"

UDPServer::UDPServer(const char * serv, const char * port)
    :socket(serv, port)
{
    pthread_mutex_init(&mutex, 0);

    socket.bind();



};

UDPServer::~UDPServer()
{
    pthread_mutex_destroy(&mutex);
};

// ----------------------------------------------------------------------------

extern "C" void * _server_thread(void *arg)
{
    UDPServer * server = static_cast<UDPServer *>(arg);

    server->server_thread();

    return 0;
}

// ----------------------------------------------------------------------------

int UDPServer::start()
{
	pthread_t idThreads[THREAD_POOL_SIZE];

	for(int i = 0; i < THREAD_POOL_SIZE; i++){

		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

		int threadError = pthread_create(&idThreads[i], &attr, _server_thread, static_cast<void*>(this));

		if(threadError != 0){
			std::cout << "Error al crear thread" << std::endl;
			return -1;
		}

	}

	return 0;

}

// ----------------------------------------------------------------------------

void UDPServer::server_thread()
{

	while(true){

			char* buf;
			struct sockaddr src;
			socklen_t size = sizeof(src);

			Socket * newSocket;

			socket.recv(buf,&newSocket);

			add_connection(newSocket);

			do_message(buf);
	}

}

// ----------------------------------------------------------------------------

void UDPServer::add_connection (Socket * s)
{
	pthread_mutex_lock(&mutex);

	bool alreadyIn = false;

	std::vector<Socket*>::iterator it = connections.begin();
	std::vector<Socket*>::iterator itEnd = connections.end();

	while(it != itEnd){

		if((*it) == s){
			it = connections.erase(it);
			alreadyIn = true;

		}else
			it++;
	}


	if(!alreadyIn && connections.size() < THREAD_POOL_SIZE){
		connections.push_back(s);
	}

	pthread_mutex_unlock(&mutex);

}


// ----------------------------------------------------------------------------

void UDPServer::del_connection (Socket * s)
{

	pthread_mutex_lock(&mutex);

	bool found = false;

	std::vector<Socket *>::iterator it = connections.begin();

	while (it != connections.end() && !found){
		if((*it) == s){
			connections.erase(it);
			found = true;
		}
	}

	if(!found)
		delete s;

	pthread_mutex_unlock(&mutex);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

