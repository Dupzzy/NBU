#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;


int main() {
	int n;
	int m;
	
	
	while (cin >> n) {
		int arr[n];
		int pos = 0;
		int w = n;
		m = 0;
		
		while (w != 1) {
			w /= 2;
			m++;
		}
		
		for (int i = 0; i < n; i++)  {
			cin >> arr[i];
		}
		
		for (int i = 0; i < m; i++) {
			for (int j = 1, pos = 0; j < n; j += 2, pos++) {
				arr[pos] = arr[j] - arr[j - 1];
			}
			n /= 2;
		}
		
		cout << arr[0];
	}
	
	
	return 0;
}
