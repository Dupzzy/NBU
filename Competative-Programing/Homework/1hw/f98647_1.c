#include <stdio.h>
#include <stdlib.h>

void convert(int system, int num) {
	int arrsize = 0;
	int numh = num;
	int holder;
	
	while(numh) {
		arrsize++;
		numh /= system;
	}
	
	char arr[arrsize];
	int pos = arrsize - 1;	
	
	while(num) {
		holder = num % system;
		
		if (holder >= 0 && holder <= 9) {
			arr[pos] = holder + '0';
		} else {
			arr[pos] = 'A' + (holder - 10);
		}

		num /= system;
		pos--;
	}
	
	for (int i = 0; i < arrsize; i++) 
		printf("%c", arr[i]);
		
	printf(" ");
		
	return;
}

int main() {
	int num;
	int holder;
	int system;
	int flag = 1;
	int state;
	
	while(scanf("%d", &holder)) {
		if (flag == 1) {
			system = holder;
			flag = 0;
			continue;
		}
		
		state = getchar();
		
		if (state == '\n') {
			state = 0;
			num = holder;
			convert(system, num);
			printf("\n");
			flag = 1;
			continue;
		} 
		
		else if (state == EOF) {
			num = holder;
			convert(system, num);
			break;
		}
		
		ungetc(state, stdin);
		num = holder;
		convert(system, num);
			
	}
	
}
