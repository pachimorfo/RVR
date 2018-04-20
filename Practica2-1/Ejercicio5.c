#include <time.h>
#include <iostream>
#include <math.h>

int main(){
	int count = 0;
	struct timespec tIn, tFin;
	
	clock_gettime(CLOCK_REALTIME, &tIn);
	
	while(count < 10000){
	
		count++;
	}
	
	clock_gettime(CLOCK_REALTIME, &tFin);

	long dif = tFin.tv_nsec - tIn.tv_nsec;
	
	std::cout << dif << std::endl;
	return 1;
}
