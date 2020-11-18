#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_ITER 1000

int a, b, p, q, k;

long fib(int n) {
	if (n < 2)
		return 1;
	else if (k > n) {
		return n;
	} else {
		return (a * fib(n-p) + b * fib(n-q)) % 1000000;
	}
}

int isPrime(long n) {
	size_t i;
	long sqrtN = sqrt(n);
	for (i = 2; i < sqrtN; i++) {
		if (!(n % i)) {
			return 0;
		}
	}
	return 1;
}

int main(void) {
	size_t i, j, itrA, maxItr;
	char tmpChar;
	int args[100][5];
	long sum[100];
	long tmp;
	memset(sum, 0, 100);

	itrA = 0;
	do {
		tmpChar = scanf("%i %i %i %i %i", 
					&args[itrA][0], &args[itrA][1], &args[itrA][2], 
					&args[itrA][3], &args[itrA][4]
					);
		itrA += (tmpChar != EOF);
	} while (tmpChar != EOF);
	maxItr = itrA;

	for (itrA = 0; itrA < maxItr; itrA++) {
		a = args[itrA][0];
		b = args[itrA][1];
		p = args[itrA][2];
		q = args[itrA][3];
		k = args[itrA][4];

		for (j = 0; j <= MAX_ITER; j++) {
			sum[itrA] += isPrime(fib(j));
		}
	}

	for (itrA = 0; itrA < maxItr; itrA++) {
		printf("%ld\n", sum[itrA]);
	}

	return 0;
}