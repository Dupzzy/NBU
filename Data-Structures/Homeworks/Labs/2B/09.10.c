#include <stdio.h>
#include <time.h>

int result = 0;

void GCD(int a, int b) {
	if ((b % a) == 0) {
		result = a;
		return;
	}  
	GCD(b % a, a);
	return;
}

int GCD2(int a, int b) {
	if ((b % a) == 0) {
		return a;
	}  
	return GCD2(b % a, a);
}

int fact(int a) {
	if (a == 1) 
		return a;
	else
		return fact(a - 1) * a;
}

void binary_search(int left, int right, int* arr, int num) {
	int midle = ((right - left) / 2) + left;

	if (num == arr[midle]) {
		printf("The matching number is at index: %d\n", midle);
		return;
	}	

	if (left == right) {
		printf("There is no match!\n");
		return;
	}

	if (arr[midle] > num) {
		right = midle - 1;
		binary_search(left, right, arr, num);
		return;
	} 

	else if (arr[midle] < num) {
		left = midle + 1;
		binary_search(left, right, arr, num);
		return;
	}
	
}

int main() {
	clock_t c1, c2;
	
	c1 = clock();
	GCD(35, 100);
	c2 = clock();
	printf("Result:%d, time:%f\n", result, (float)(c2 - c1) / CLOCKS_PER_SEC);
	
	c1 = clock();
	result = GCD2(13, 54564345);
	c2 = clock();
	printf("Result:%d, time:%f\n", result, (float)(c2 - c1) / CLOCKS_PER_SEC);

	printf("\n\n-------------------------------------------------------\n2nd exercise\n\n");

	printf("Result: %d\n", fact(5));

	printf("\n\n-------------------------------------------------------\n3rd exercise\n\n");

	int arr[5] = {1,2,3,4,5};
	binary_search(0,4,arr,6);
}
