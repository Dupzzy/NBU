// C++ program to implement division with large 
// number 
#include <bits/stdc++.h> 
using namespace std; 

string increment(string number) {
	int start = 0;
	int carry = 1;
	
	while (number[start])
		start++;
	start--;	
	while (carry) {
		if (number[start] == '9') {
			number[start] = 0;
			start++;
			carry = 1;
		} else {
		//cout << number[start] << endl;
			number[start] += 1;
			//cout << number[start] << endl;
			carry = 0;
		}
	}
	//cout << number;
	
	return number;
}
  
// A function to perform division of large numbers 
string longDivision(string number, int divisor, int* num) 
{ 
    // As result can be very large store it in string 
    string ans; 
    int leftover;
  
    // Find prefix of number that is larger 
    // than divisor. 
    int idx = 0; 
    int temp = number[idx] - '0'; 
    while (temp < divisor) 
        temp = temp * 10 + (number[++idx] - '0'); 
  
    // Repeatedly divide divisor with temp. After 
    // every division, update temp to include one 
    // more digit. 
    while (number.size() > idx) { 
        // Store result in answer i.e. temp / divisor 
        ans += (temp / divisor) + '0'; 
  	leftover = (temp % divisor);
  	
        // Take next digit of number 
        temp = (temp % divisor) * 10 + number[++idx] - '0'; 
    } 
  	
    // If divisor is greater than number 
    if (ans.length() == 0) {
        (*num)++;
        return "0";
        }
      
    
      	(*num)++;
    // else return ans 
    return ans; 
} 
  
// Driver program to test longDivison() 
int main() 
{ 
	string a, b;
	//getline(cin, a);
	//cout << endl << a;
	//a = increment(a);
	//cout << endl << a;
	
	while (getline(cin, a, ' ') && getline(cin, b)) {
		int num = 0;
		while (!(a == b)) {
			longDivision(a, 2, &num); 
			a = increment(a);
		}
		
		longDivision(a, 2, &num); 
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
