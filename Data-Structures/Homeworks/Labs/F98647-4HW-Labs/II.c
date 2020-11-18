#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define STEP 3000
#define INIT_ELEM 1000
#define MAX_ELEM 31000

void selection_sort(int* arr, int size);
void q_sort(int *arr, int li, int ri);
void print_arr(int * arr, int size);

int main() {
	int i = 0, j = 0;
	clock_t c0, c1, c2, c3;
	float cpuavg0 = 0., cpuavg1 = 0.;
	srand(time(NULL));
	
	printf("Elements, Selection_sort, Quick_sort\n");
		
	for(i = INIT_ELEM; i <= MAX_ELEM; i += STEP) {
		cpuavg0 = 0.;
		cpuavg1 = 0.;
		for (int k = 0; k < 3; k++) {		
			int arr[i];
	
			for (j = 0; j < i; j++) 
				arr[j] = j;
			
			c0 = clock();
			selection_sort(arr, i);
			c1 = clock();
			cpuavg0 += (float)(c1 - c0) / CLOCKS_PER_SEC;

			c2 = clock();
			q_sort(arr, 0, i - 1);
			c3 = clock();
			cpuavg1 += (float)(c3 - c2) / CLOCKS_PER_SEC;
		}

		printf("%d, %f, %f\n", i, cpuavg0 / 3, cpuavg1 / 3);
	}
		
}


void selection_sort(int* arr, int size) {
	int i, j, min, pos, flag;
	min = arr[0];

	for (i = 0, flag = 0; i < size; i++) {
		min = arr[i];
		for (j = i; j < size; j++) {
			if (arr[j] < min) {
				min = arr[j];
				pos = j;
				flag = 1;
			}

		}
		if (flag) {
			arr[i] ^= arr[pos];
			arr[pos] ^= arr[i];
			arr[i] ^= arr[pos];
			flag = 0;
		}
	}
	
	return;
}

void q_sort(int *arr, int li, int ri) {
	int i, j;
	int ethalon;
	int holder;

	i = li;
	j = ri;

	ethalon = arr[i];

	do {
		while (arr[i] < ethalon) {
			i++;
		}

		while (arr[j] > ethalon) {
			j--;
		}

		if (j >= i) {
			holder = arr[i];
			arr[i] = arr[j];
			arr[j] = holder;
			i++; 
			j--;
		}
	} while (j >= i);


	if (li < j) {
		q_sort(arr, li, j);
	}

	if (ri > i) {
		q_sort(arr, i, ri);
	}

	return;
}

void print_arr(int * arr, int size) {
	for(int i = 0; i < size; i++) {
		printf("%d ", arr[i]);
	}
}

