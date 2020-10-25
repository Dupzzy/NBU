#include <stdio.h>
#include <stdlib.h>

int main() {
	int n; // Size of the row
	int state = 0; // state holds the getc, and if EOF has not occured, ungetc to pput it back in the buffer
	int l, r; // l - left - a[index], r - right - a[index + 1]
	int check = 0; // Hold the result of substraction of r - l
	int max = 0; // If check is higher of current max, max is gonna be = check
	
	while(scanf("%d", &n)) {
		state = getchar();
		if (state == EOF) 
			break;
			
		ungetc(state, stdin);
		scanf("%d", &l);
		scanf("%d", &r);
		max = l + r;
		
		for (int i = 2; i < n; i++) {
			l = r;
			scanf("%d", &r);
			if (max < (check = r - l))
				max = check;
		}
		
		printf("%d\n", max);
		max = 0;
	}
}
