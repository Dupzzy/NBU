#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int* gradingStudents(int grades_count, int* grades, int* result_count) {
    int i;
    *result_count = grades_count;
    int *newarr = malloc(grades_count * sizeof(int));
   
    for(i = 0; i < grades_count; i++) {
        if ( ((grades[i] % 5) >= 3) && grades[i] > 37) {
            newarr[i] = grades[i] + (5 - (grades[i] % 5));
        } else newarr[i] = grades[i];
    }
    return newarr;
}

int main() {
	int arr[6] = {37,38,44,41,67,68};
	int res;
	int *arr1 = gradingStudents(6, arr, &res);

	for (int i = 0; i < res; i++)		
		printf("%d\n", arr1[i]);
}



