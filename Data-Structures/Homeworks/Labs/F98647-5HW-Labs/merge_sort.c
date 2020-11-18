#include <stdio.h>
#include <time.h>
#include <stdlib.h>

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
		printf("Потъване наляво, лява граница: %d дясно граница: %d \n", li, mid);
		merge_sort(arr, li, mid);
		printf("Потъване надясно, лява граница: %d дясно граница: %d \n", mid + 1, ri);
		merge_sort(arr, mid + 1, ri);
		printf("Сливам в Ц и прехвърлям в А\n");
		merge(arr, li, ri, mid);
		printf("Слях ги и са наредени от индекс %d до индекс %d \n", li, ri);
	}
	
	return;
}

int main () {
/*
	int arr[6] = { 1, 5, 7, 2, 3, 6 };
	
	 Testing if the merge works 
	
	for (int i = 0; i < 6; i++)
		printf("%d ", arr[i]);
	printf("\n");
		
	merge(arr, 0, 5, 2);
	
	for (int i = 0; i < 6; i++)
		printf("%d ", arr[i]);
	printf("\n");
	
	 Testing if merge_sort works 
	
	int arr2[6] = { 1, 9, 3, 4, 1, 5 };
	
	for (int i = 0; i < 6; i++)
		printf("%d ", arr2[i]);
	printf("\n");
		
	merge_sort(arr2, 0, 5);
	
	for (int i = 0; i < 6; i++) 
		printf("%d ", arr2[i]);
	printf("\n");
*/
	srand(clock());
	
	int arr[20];
	
	for (int i = 0; i < 20; i++) 
		arr[i] = rand() % 50;
		
	merge_sort(arr, 0, 19);
	
	for (int i = 0; i < 20; i++) 
		printf("%d ", arr[i]);
	
	


	
	return 0;
}
