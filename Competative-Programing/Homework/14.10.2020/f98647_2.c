#include <stdio.h>
#include <stdlib.h>

int main() {

	int n = 0;

	while(scanf("%d", &n) != EOF) {
		int max = 0;
		int arr[n];
		for (int i = 0; i < n; i++) 
			scanf("%d", &arr[i]);

		max = arr[1] - arr[0];
		for (int i = 2; i < n; i++)
			if (max < (arr[i] - arr [i - 1]))
				max = arr[i] - arr[i - 1];
		
		printf("%d\n", max);	
	}

}
