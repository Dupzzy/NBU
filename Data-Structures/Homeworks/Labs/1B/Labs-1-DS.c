#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define STEP 10000
#define MAXSIZE 100000

void swap (int *a, int *b);
void selection_sort(int* arr, int size);
void bubble_sort(int * arr, int size);
void insertion_sort (int * arr, int size);
void sort_speed(void (*sort)(int *arr, int size)); 

int main() {
	printf("Avg speed for 3 sortings, selection sort:\n\n");
	sort_speed(selection_sort);
	printf("\nAvg speed for 3 sortings, bubble sort:\n\n");
	sort_speed(bubble_sort);
	printf("\nAvg speed for 3 sortings, insertion sort:\n\n");
	sort_speed(insertion_sort);
	
	return 0;
}

void sort_speed(void (*sort)(int *, int)) {
	int j, i;
	int size;
	float cpuavg;
	long avg;
	clock_t c0, c1;
	time_t t0, t1;
	srand(time(NULL));

	for (size = 10000; size <= MAXSIZE; size += STEP) {
		avg = 0;
		cpuavg = 0;
		for (j = 0; j < 3; j++) {
			int arr[size];

			for(i = 0; i < size; i++)
				arr[i] = rand() % size;
			//t0 = time(NULL);
			c0 = clock();
			sort(arr, size);
			//t1 = time(NULL);
			c1 = clock();
			cpuavg += (float)(c1 - c0) / CLOCKS_PER_SEC;
			//avg += t1 - t0;
		}
		
		//printf("Total time for 3 sorts in int and float variable - int:%ld float:%f\n", avg, cpuavg);		
		printf("Avg time for %d elements: %f\n", size, cpuavg / 3);
	}	

	return;
}

void swap (int *a, int *b) {
	*a ^= *b;
	*b ^= *a;
	*a ^= *b;
	return;
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

void bubble_sort(int * arr, int size) {
	int i, j;
	
	for (j = 0; j < size - 1; j++) 
		for (i = 0; i < size - 1 - j; i++) {
			if(arr[i] > arr[i + 1])
				swap (&arr[i], &arr[i + 1]);	
	}
	
	return;
}

void insertion_sort (int * arr, int size) {
	int i, j, holder = 0;

	for (i = 1; i < size; i++) {
		j = i;
		while (arr[j] < arr[j - 1] && j > 0) {
			arr[j] ^= arr[j - 1];
			arr[j - 1] ^= arr[j];
			arr[j] ^= arr[j - 1];
			//printf("%d\n",arr[j]);
		//	holder = arr[j];
		//	arr[j] = arr[j - 1];
		//	arr[j - 1] = holder;
			j--;
		}
	}

	return;
}	
