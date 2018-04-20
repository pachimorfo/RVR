#include <time.h>
#include <iostream>

int main {

	struct tm t;
	time_t time;
	time = time(NULL);
	t = localtime(&t);


	char st[100];
	char ano[100] = "Estamos en el año ";
	strftime(st, 100,'%w', &t);
	
	
	std::cout << ano << st << std::endl;

	return 1;
}
