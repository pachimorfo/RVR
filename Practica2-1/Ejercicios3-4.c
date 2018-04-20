#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <iostream>

int main(){
	
	time_t currentTime = time(NULL);
	char* fecha = ctime(&currentTime);

	std::cout << fecha << std::endl;
	
	return 1;	
	
}


/*
1.El espacio lo reserva el sistema
2.No es necesario que nosotros liberemos el puntero
3.No, debería usarse la función ctime_r(..)



*/
