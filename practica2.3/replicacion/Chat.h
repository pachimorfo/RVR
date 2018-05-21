#include <string>
#include <unistd.h>
#include <string.h>
#include "Serializable.h"
#include "Socket.h"
#include "UDPServer.h"

class ChatMessage: public Serializable
{
public:
    ChatMessage(){};

    ChatMessage(const char * n, const std::string m)
    {

    	strncpy(nick, n, 8);
    	strncpy(nick, n, 8);

    	//m.c_str();
    	m.copy(message,80);

    };

    void to_bin()
    {

    	int32_t total = (80* sizeof(char)) + (8 * sizeof(char)) + sizeof(int32_t);
    	alloc_data(total);

    	char* pointer = _data;
		memcpy(pointer,&total, sizeof(int32_t));
		pointer = _data + sizeof(int32_t);

		memcpy(pointer,(void*)nick, 8);
		pointer += 8;

		memcpy(pointer,(void*)message,80);
		pointer+= 80;


    }

    virtual int from_bin(char * bobj)
    {
    	char *pointer = bobj + sizeof(int32_t);

		memcpy(nick,pointer,8);
		pointer += 8;

		memcpy(message,pointer,80);
		pointer += 80;

		return 0;
    }

    char message[80];

    char nick[8];
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class ChatServer : public UDPServer
{
public:
    ChatServer(const char * s, const char * p): UDPServer(s, p){};

    void do_message(char * buffer)
    {
    	ChatMessage msg;
    	msg.from_bin(buffer);



    	for(Socket* mySocket : connections){

    		socket.send(&msg,mySocket);

    	}



    }
};

class ChatClient
{
public:
    ChatClient(const char * s, const char * p, const char * n):socket(s, p)
    {

    	strncpy(nick, n, 8);

    };

    void input_thread()
    {
    	char c[80];
    	ChatMessage* msg;
    	while(true){

    		std::cin >> c;
    		msg = new ChatMessage(nick, c);
    		msg->to_bin();
    		socket.send(msg, &socket);
    	}

    }

    void net_thread()
    {
    	ChatMessage msg;
    	char c[80];
    	while(true){

    		socket.recv(c);
    		msg.from_bin(c);

    		if(msg.nick != nick){
				std::cout << msg.nick <<  ": "<< msg.message;
    		}
    	}
    }

private:
    char nick[8];

    Socket socket;
};

