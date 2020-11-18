#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <cstring>
#include <stdlib.h>



using namespace std;


int main() {
	string line;
	int num = 0;
	int pos = 0;
	
	while (getline(std::cin, line)) {
		if (line[0] == '-') break;
		
		num = 0;
		pos = 0;
		string holder = line;
		
		while (1) {
			num = 0;
			pos = 0;
			while (line[pos]) {
				num += (line[pos++] - '0');
			}
		
			if (num < 10) {
				cout << Digital root of << holder << is << num << endl;
				break;
			}

			line = to_string(num);	
			
		}
	}
	
    
    	  
	return 0;
}
