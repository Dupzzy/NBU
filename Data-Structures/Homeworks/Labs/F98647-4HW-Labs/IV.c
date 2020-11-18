#include <stdio.h>
#include <stdlib.h>

int* merge(int* arra, int* arrb, int sizea, int sizeb) {
	int sizec = sizea + sizeb;
	int *arrc = malloc(sizeof(int) * sizec);
	int i = 0, j = 0, k = 0;
	
	while(i < sizea && j < sizeb) 
		if (arra[i] < arrb[j]) {
			printf("В момента сравняваме масив А и Б и записваме\n");
			arrc[k++] = arra[i++];
		} else { arrc[k++] = arrb[j++]; printf("В момента сравняваме масив А и Б и записваме\n"); }
		
	/* if left side isn't finished */
	while (i < sizea) {
		printf("В момента допълваме остатъка от масив А\n");
		arrc[k++] = arra[i++];
	}
	
	/* if right side isn't finished */
	while (j < sizeb) {
		printf("В момента допълваме остатъка от масив Б\n");
		arrc[k++] = arrb[j++]; ;
	}
		
	return arrc;	
}

int main() {
	int a[3] = {3, 6, 7};
	int b[5] = {1, 2, 3, 4, 5};
	
	int *arr = merge(a, b, 3, 5);
	
	for (int i = 0; i < 8; i++) 
		printf("%d ", arr[i]);
}
