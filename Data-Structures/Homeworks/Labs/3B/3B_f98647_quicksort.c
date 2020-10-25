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

	printf("Граници: лява - %d\tдясна - %d\n", li, ri);
	ethalon = arr[(i + j) / 2];

	printf("Еталон: %d\n", ethalon);

	do {
		while (arr[i] < ethalon) {
			i++;
		}

		while (arr[j] > ethalon) {
			j--;
		}

		if (j >= i) {
			printf("Размяна на %d с %d\n", arr[i], arr[j]);
			holder = arr[i];
			arr[i] = arr[j];
			arr[j] = holder;
			i++; 
			j--;
		}
	} while (j >= i);

	printf("Крайн на размените.\n");
	printf("--------------------------------------------\n");

	if (li < j) {
		printf("Ляво потъване\n");
		q_sort(arr, li, j);
	}

	if (ri > i) {
		printf("Дясно потъване\n");
		q_sort(arr, i, ri);
	}

	printf("Дъно.\nИзплуване.\n");
	return;
}

int main() {
	int arr[9] = { 9,8,6,4,7,9,9,0,4 };
	printf("Начално състояние на масива:\n");
	print_arr(arr, 9);
	printf("\n");
	q_sort(arr, 0, 8);
	printf("Крайно състояние на масива:\n");
	print_arr(arr, 9);
	printf("\n");
}
