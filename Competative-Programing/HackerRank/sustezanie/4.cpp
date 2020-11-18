#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

//len - минималната дължината на паролата;
//cap - минимален брой главни букви;
//dig - минимален брой цифри;
//spec - минимален брой специални символи (символи от клавиетурата, които не са букви, цифри или интервал).



int main() {
	int len;
	int cap;
	int dig;
	int spec;
	int tests;
	string pass;
	string hold;
	
	cin >> len >> cap >> dig >> spec >> tests;
	getline(cin, hold);
	for (int i = 0; i < tests; i++) {
		string pass;
		getline(cin, pass);
		int len1 = 0;
		int cap1 = 0;
		int dig1 = 0;
		int spec1 = 0;
		int pos = 0;
		int flag = false;
			
		while (pass[pos]) {
		
			if (pass[pos] >= 'a' && pass[pos] <= 'z') {
				len1++;
			} 
				
			else if(pass[pos] >= 'A' && pass[pos] <= 'Z') {
				cap1++;
				len1++;
			}
			
			else if (pass[pos] >= '0' && pass[pos] <= '9') {
				dig1++;
				len1++;
			}
			
			else if (pass[pos] != ' ') {
				spec1++;
				len1++;
			} 
			
			else if (true) {
				cout << "no\n";
				flag = true;
				break;
			}
			pos++;
		}
		cout << len1 << " " << cap1 << " " << dig1 << " " << spec1;
		if (flag != true) { 
		if (len1 >= len && cap1 >= cap && dig1 >= dig && spec1 >= spec)
			cout << "yes\n";
		else 	cout << "no\n";
		}
		
	}
	  
	return 0;
}


