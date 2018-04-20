#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <iostream>
#include <string.h>

int main(){
	
	int error = setuid(0);
	if (error == -1){
		/*Ejercicio 1*/	
		perror("Error: ");
		/*Ejercicio 2*/
		char errStr[100];
		char* errNum;
		errNum = strerror_r(errno,errStr, 100);
		//printf(&errStr);
		std::cout << errStr << errNum << std::endl;
	}

	return 1;

}
