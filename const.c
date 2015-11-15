#include <stdio.h>


int main() {
	const int *data;
	const int var;
	int x;
	int y;
	data = &x;
	//*data = 100;
	data = &y;

	return 0;
}
