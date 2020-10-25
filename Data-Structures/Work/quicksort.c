#include <stdio.h>
#include <stdlib.h>

void print_arr(int * arr, int size) {
	for(int i = 0; i < size; i++) {
		printf("%d ", arr[i]);
	}
}

void q_sort(int *arr, int li, int ri) {
	int i, j;
	int ethalon;
	int holder;

	i = li;
	j = ri;

	printf("Partion borders: Left - %d\tright - %d\n", li, ri);
	ethalon = arr[(i + j) / 2];

	printf("Ethalon value: %d\n", ethalon);

	do {
		while (arr[i] < ethalon) {
			i++;
		}

		while (arr[j] > ethalon) {
			j--;
		}

		if (j >= i) {
			printf("Swaping %d with %d\n", arr[i], arr[j]);
			holder = arr[i];
			arr[i] = arr[j];
			arr[j] = holder;
			i++; 
			j--;
		}
	} while (j >= i);

	printf("End of swaping.\n");
	printf("--------------------------------------------\n");

	if (li < j) {
		printf("Left sink\n");
		q_sort(arr, li, j);
	}

	if (ri > i) {
		printf("Right sink\n");
		q_sort(arr, i, ri);
	}

	return;
}

int main() {
	int arr[11] = { 9,8,1,2,3,7,9,4,5,6,7 };
	printf("Array start state:\n");
	print_arr(arr, 11);
	printf("\n");
	q_sort(arr, 0, 10);
	printf("Array end state:\n");
	print_arr(arr, 11);
	printf("\n");
}
