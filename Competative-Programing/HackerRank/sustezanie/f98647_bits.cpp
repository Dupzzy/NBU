// C++ program to implement division with large 
// number 
#include <iostream>
using namespace std; 

string increment(string number) {
	int start = 0;
	int carry = 1;
	string ha = "1";
	
	while (number[start])
		start++;
	start--;
	
	while (carry) {
		if (number[start] == '9') {
			if (start == 0) {
			number = "1" + number;
			number[start + 1] = '0';
			break;
			} else 
			number[start] = '0';
			start--;	
			carry = 1;
		} else {
			number[start] += 1;
			carry = 0;
		}
	}
	
	return number;
}
  

string longDivision(string number, int divisor, int* num) { 
        string ans; 
        int leftover;
        int idx = 0; 
        int temp = number[idx] - '0'; 
        
        while (temp < divisor) 
        	temp = temp * 10 + (number[++idx] - '0'); 
  
        while (number.size() > idx) { 
        	ans += (temp / divisor) + '0'; 
       	temp = (temp % divisor) * 10 + number[++idx] - '0'; 
        } 
  	
        if (ans.length() == 0) {
        	(*num)++;
        	return "0";
        }
      
    
      	(*num)++;
	return ans; 
} 
  
// Driver program to test longDivison() 
int main() 
{ 
	string a, b;
	string holder = new char [100];
	int num = 0;
	
/*	
	getline(cin, a);
	a = increment(a);
	cout << a;
	getline(cin, a, ' ');
	getline(cin, b);
*/	
/*
	while (a != b) {
		a = increment(a);
		cout << a << endl;
	}
*/

/*
	getline(cin, a);
	for (int i = 0; 1; i++) {
		holder[i] = a[i];
		if (a[i] == '\0')
			break;
	}
	
	while (holder[0] != '0') {
		holder = longDivision(holder, 2, &num); 
	}
	cout << num << endl;
	*/
	
	while (getline(cin, a, ' ') && getline(cin, b)) {
		num = 0;
		while(a != b) {
			holder = a;
			while (holder[0] != '0') {
				holder = longDivision(holder, 2, &num); 
			}
			a = increment(a);
		}
			holder = a;
			
		while (holder[0] != '0') {
				holder = longDivision(holder, 2, &num); 
			}
		cout << num << endl;
		
	}
	
	
	
	
	
	
	/*
	int num = 0;
	string number = "156"; 
	int divisor = 2; 
	while (number[0] != '0') {
		number = longDivision(number, divisor, &num); 
	}
	cout << num;
	*/
    return 0; 
} 
