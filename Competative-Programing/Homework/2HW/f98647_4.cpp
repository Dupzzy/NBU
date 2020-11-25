#include <iostream>
#include <bits/stdc++.h>

using namespace std;

int gcd(int a, int b) { 
    if (a == 0)
        return b; 
    return gcd(b % a, a); 
} 
 
// Function to return LCM of two numbers 
int lcm(int a, int b) { 
    return (a / gcd(a, b)) * b;
} 

int main() {
	string line;
	int flag1 = false;
	
	while (getline(cin, line)) {
	
		if(line[line.size() - 1] == '\0' || line[0] == ' ') {
			break;
		}
		
		int whole = 0;
		int fraction = 0;
		int pos = 0;
		int warr[1000] = { 0 }; int wpos = 0;
		int farr[1000] = { 0 }; int fpos = 0;
		char carr[1000]; int cpos = 0;
		int flag = false;
	
	
		// Reading all the plus, minus, fractional and whole parts of the string
		while(line[pos]) {
			if(line[pos]  == '+' || line[pos]  == '-') {
				carr[cpos++] = line[pos]; 
				flag = false;
				fpos++;
				pos++;
				continue;
			}
			
			if(line[pos]  == '/') {
				flag = true;
				wpos++;
				pos++;
				continue;
			}
			
			if((line[pos]  >= '0' && line[pos]  <= '9') && flag == true) {
				farr[fpos] = (farr[fpos] * 10) + (line[pos] - '0');
				pos++;
				continue;
			}
			
			if((line[pos]  >= '0' && line[pos]  <= '9') && flag == false) {
				warr[wpos] = warr[wpos] * 10 + (line[pos]  - '0');
				pos++;
				continue;
			}
			
			pos++; 
		}
		fpos++;
		
		// Finding the lcm 
		fraction = farr[0];
		for (int i = 1; i < fpos; i++) {
			fraction = lcm(fraction, farr[i]);
		}
		
		// Multiplying the numbers to with the correct fraction part
		for (int i = 0; i < wpos; i++) {
			warr[i] = warr[i] * (fraction / farr[i]);
		}
		
		// adding and substracting whole parts
		whole = warr[0];
		for (int i = 0; i < cpos; i++) {
			if (carr[i] == '-') {
				whole -= warr[i + 1];
				continue;
			}
			
			if (carr[i] == '+') {
				whole += warr[i + 1];
				continue;
			}
		}
		if (whole != 0) {
		int gcdd;
		if(whole < 0)
			gcdd = gcd(-whole, fraction);
		else
			gcdd = gcd(whole, fraction);
		if (flag1 == false) {
			flag1 = true;
		}
		
		if(fraction / gcdd != 1)
			cout << whole / gcdd << "/" << fraction / gcdd;
		else 	
			cout << whole / gcdd;
		}
		else 
			cout << "0";
			
		cout << endl;
	}	
		
}
