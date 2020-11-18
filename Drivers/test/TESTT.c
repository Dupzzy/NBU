#include <stdio.h>

int main() {
	char buffer[10];
	snprintf(buffer, 10, "%d.%04d", 24, 2530);

	buffer[8] = '5';
	printf("%s\n", buffer);
	printf("%c\n", buffer[8]);
}
