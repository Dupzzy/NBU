#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

#define STEP 10000
#define MAXSIZE 100000

void merge(int* arr, int li_a, int ri_b, int mid) {
	int size = ri_b + 1;
	int hold[size];
	int pos = li_a;
	int li = li_a;
	int ri_a = mid;
	int li_b = mid + 1;
	
	/* starting sort untill one of the sides is empty */
	while(li_a <= ri_a && li_b <= ri_b) 
		if (arr[li_a] < arr[li_b]) {
			hold[pos++] = arr[li_a++];
		} else hold[pos++] = arr[li_b++]; 
		
	/* if left side isn't finished */
	while (li_a <= ri_a) {
		hold[pos++] = arr[li_a++];
	}
	
	/* if right side isn't finished */
	while (li_b <= ri_b) {
		hold[pos++] = arr[li_b++];
	}
	
	/* filling the results in passed arr */
	for (int i = li; i <= ri_b; i++)
		arr[i] = hold[i];
		
	return;	
}

void merge_sort(int* arr, int li, int ri) {
	int mid = 0;
	
	if (li < ri) {
		mid = (ri + li) / 2;
		merge_sort(arr, li, mid);
		merge_sort(arr, mid + 1, ri);
		merge(arr, li, ri, mid);
	}
	
	return;
}

void q_sort(int *arr, int li, int ri) {
	int i, j;
	int ethalon;
	int holder;

	i = li;
	j = ri;

	ethalon = arr[(i + j) / 2];

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


	if (li < j) 
		q_sort(arr, li, j);
	

	if (ri > i) 
		q_sort(arr, i, ri);
	
	return;
}

int main () {
	int i,j,size;
	clock_t c0, c1;
	clock_t c2, c3;
	srand(time(NULL));
	
	for (size = 10000; size <= MAXSIZE; size += STEP) {
		float cpuavg = 0;
		float cpuavg2 = 0;
		
		for (j = 0; j < 3; j++) {
			int arr[size], arr2[size];

			for(i = 0; i < size; i++) {
				arr[i] = rand() % size;
				arr2[i] = arr[i];
			}
			
			c0 = clock();
			merge_sort(arr, 0, size - 1);
			c1 = clock();
			cpuavg += (float)(c1 - c0) / CLOCKS_PER_SEC;
			
			c2 = clock();
			q_sort(arr, 0, size - 1);
			c3 = clock();
			cpuavg2 += (float)(c3 - c2) / CLOCKS_PER_SEC;
		}
		
		//printf("Total time for 3 sorts in int and float variable - int:%ld float:%f\n", avg, cpuavg);		
		printf("%f, ", cpuavg / 3);
		printf("%f\n", cpuavg2 / 3);
		
	}	

	
	
	return 0;
}
