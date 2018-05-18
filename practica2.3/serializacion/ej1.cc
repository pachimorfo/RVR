#include "Serializable.h"

#include <iostream>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>



void printError(){
	char errStr[100];
	char* errNum;
	errNum = strerror_r(errno,errStr, 100);

	std::cout << errStr << errNum << std::endl;


}

class Jugador: public Serializable
{
public:

   

    Jugador(const char * _n, int16_t _x, int16_t _y):x(_x),y(_y)
    {
        strncpy(name, _n, 80);
    };

    virtual ~Jugador(){};

    void to_bin()
    {
	int32_t total = (80 * sizeof(char)) + (2 * sizeof(int16_t) + sizeof(int32_t));

	alloc_data(total + sizeof(int32_t));
	char* pointer = _data;
	memcpy(pointer,&total, sizeof(int32_t));	
	pointer = _data + sizeof(int32_t);

	memcpy(pointer,(void*)name, 80);
	pointer += 80;

	memcpy(pointer,&x,sizeof(int16_t));	
	pointer+= sizeof(int16_t);

	memcpy(pointer,&y,sizeof(int16_t));	
	pointer+= sizeof(int16_t);

	

    }

    int from_bin(char * data)
    {

	char *pointer = data + sizeof(int32_t);

	memcpy(name,pointer,80);
	pointer += 80;

	memcpy(&x,pointer,sizeof(int16_t));
	pointer += sizeof(int16_t);

	memcpy(&y,pointer,sizeof(int16_t));
	pointer += sizeof(int16_t);
	
	return 0;
		
    }

public:
    char name[80];

    int16_t x;
    int16_t y;
};

int main(int argc, char **argv)
{

	Jugador j1("Pachi", 10, 125);
	

	if(argv[1][0] == 'c'){
		int fd = open("Players.rvr", O_RDWR | O_CREAT );
		if(fd == -1){
			printError();
			return -1;
		}
	
		j1.to_bin();
		char* j1Data = j1.data();
	
		int wError = write(fd,(void*) j1Data, j1.size());
		if(wError == -1){
			printError();
			return -1;	
		}
	
		close(fd);
	}
	else if(argv[1][0] == 'l'){

		int fd = open("Players.rvr", O_RDWR);
		if(fd == -1){
			printError();
			return -1;		
		}
		char* jData;

		int32_t size;
		
		int32_t sizeReturn = read(fd, &size, sizeof(int32_t));
		
 		if(sizeReturn == -1){
			printError();
			return -1;
		
		}

		lseek(fd, 0, SEEK_SET);

		jData = (char*)malloc(size);
		sizeReturn = read(fd, jData, size);
	
		if(sizeReturn == -1){
			printError();
			return -1;
		
		}
		close(fd);

		Jugador j2("Phi", 150, 12);
		
		j2.from_bin(jData);

		//std::cout << j2.data() << std::endl;
		
		std::cout << j2.name << " " << j2.x << " " << j2.y << std::endl;

	}

	
	return 0;



}
