// Binary insert is a function that takes dynamically allocated array, size and integer to be inserted.
// The function actually creates new dynamic array with space for one more element and after it inserts 
// the element it deletes the old array and also increments the size (u need to pass it by &).
// It does binary search through the sorted array and finds the position to insert the number.
// The return is pointer to the new array

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define TEST_CASES 5

int *binary_insert(int *arr, int *size, int num) {
	int i, *newarr;		 // iterator for the for loops and the array that is gonna be returned
	int right_end, left_end; // The borders of the half that the num is gonna be positioned at
	int middle_index;	 // The new middle of the set/subset the num is gonna be inserted
	
	
	middle_index = *size / 2;
	left_end = 0;		// index of the first element of the array
	right_end = *size;	// index of the last element of the array + 1
	
	newarr = malloc((*size + 1) * sizeof(int)); // allocating space for new arr with 1 space for one more elem
	for (i = 0; i < *size; i++)		    // initializing the elements of the new arr with the elem from the old one	
		newarr[i] = arr[i];
	
	while(1) {
		if (newarr[middle_index] == num) {
			for (i = (*size - 1); i >= middle_index; i--)	
				newarr[i + 1] = newarr[i];
			newarr[middle_index] = num;
			goto done;
		} 
		
		else if ((right_end - left_end) == 1) {
			if (newarr[middle_index] > num) {
				for (i = (*size - 1); i >= middle_index; i--)
					newarr[i + 1] = newarr[i];
				newarr[middle_index] = num;
			} 

			else {
				for (i = (*size - 1); i >= middle_index + 1; i--)
					newarr[i + 1] = newarr[i];
				newarr[middle_index + 1] = num;
			}
				goto done;			
		}
			
		else if (newarr[middle_index] > num) {
			right_end = middle_index;
			middle_index = ((right_end - left_end) / 2) + left_end;
			
		}

		else if (newarr[middle_index] < num) {
			left_end = middle_index;
			middle_index = ((right_end - left_end) / 2) + left_end;
		}	
	}
done:		
	(*size)++;
	free(arr);
	return newarr;
}

int main() {
	srand(time(NULL));
	int *arr;
	int sz, i, j, ins;
	
	for (i = 0; i < TEST_CASES; i++) {
		sz = (rand() % 6) + 5;	// size of the array (from 5---10)
		arr = malloc(sz * sizeof(int));	
		
		for (j = 0; j < sz; j++) 
			arr[j] = j * 2;	
		
		printf("Before binary insert:");
		for (int j = 0; j < sz; j++)
			printf(" %d", arr[j]);
		printf("\n");

		ins = rand() % (sz * 2);
		printf("Num to be inserted: %d\n", ins);
		arr = binary_insert(arr, &sz, ins);

		printf("After binary insert:");
		for (int j = 0; j < sz; j++)
			printf(" %d", arr[j]);
		printf("\n");
		printf("\n");

		free(arr);
	}
	
	return 0;
}























