#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;



int main() {
	std::string line;
	std::string buffer = "missing ";
	int flag;
	int pos;
	int arr[26];
	int n;
	
	std::cin >> n;
	std::getline(std::cin, line);

	
	for (int j = 0; j < n; j++) {
		std::getline(std::cin, line);
		pos = 0;
		flag = false;
		buffer = "missing ";
		
		for (int i = 0; i < 26; i++)
			arr[i] = false;
		
		while (line[pos]) {
			if (line[pos] >= 'a' && line[pos] <= 'z') {
				arr[line[pos] - 'a'] = true;		
			}
			
			else if (line[pos] >= 'A' && line[pos] <= 'Z') {
				arr[line[pos] - 'A'] = true;	
			}
			pos++;
		}
		
		for (int i = 0; i < 26; i++) {
			if(arr[i] == false) {
				buffer += ('a' + i);
				flag = true;
			} 
		}
		
		
		if (flag != true) {
			cout << "pangram";
		}
		
		else cout << buffer;
		std::cout << std:: endl;
	}
	
 
    return 0;
}
