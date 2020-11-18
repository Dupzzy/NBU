#include <iostream>
#include <string>
#include <unistd.h>
#include <cstdlib>

/* THERE IS 100% a better way to solve this problem, this is just how I did it */

using namespace std;

#define amount_disks 3

void print(string *a, string *b, string *c); // With this function I'll be printing the towers after each transition
void move_tower(int n, string **from, string **to, string **help, 
			string *printA, string *printB, string *printC, // These variables are only used for printing purposes, there is no need of them
			string a2, string b2, string c2); // These variables are only used for printing purposes, there is no need of them

string output_buffer = "\n"; // With this buffer I'll be printing the transitions, example A to B, A to C

int main() {

	// Arrays used to store the disks of '***'
	string a[amount_disks];   
	string b[amount_disks];   
	string c[amount_disks];   

	// Since I am gonna use manipulators (double pointers "**") we need to pass pointer holders
	// Hard to explain...
	string *a1 = a;
	string *b1 = b;
	string *c1 = c;
	
	string A = "А",
	       B = "Б", 
	       C = "Ц";
 
	// Initializing the arrays
	for (int i = 0; i < amount_disks; i++) {
		a1[i] = "*";
		for (int j = 0; j < amount_disks - 1 - i; j++)
			a[i] += "**";
		b1[i] = "-";
		c1[i] = "-";
	}
	
	// Printing the arrays at the start 
	
	 /*      *	-	-   */
        /*      **	-	-  */
   	/*      *** 	-	-  */
   	
   	system("clear");
	print(a1, b1, c1);
	sleep(2);
	
	//Setting up the right addresses
	a1 += amount_disks - 1;
	b1 -= 1;
	c1 -= 1;
	
	move_tower(amount_disks, &a1, &b1, &c1, a ,b ,c, A, B, C);
	
	return 0;
}

// Most of the lines in the function are just for printing purposes, without them the code would've been like 5-10 lines max

void move_tower(int n, string **from, string **to, string **help, 
			string *printA, string *printB, string *printC,
			string a2, string b2, string c2) {
	
	if(n > 1) {
		move_tower(n - 1, from, help, to, printA, printB, printC, a2, c2, b2);
		*(++(*to)) = **from;
		*((*from)--) = "-";
		system("clear");
		print(printA, printB, printC);
		output_buffer = output_buffer + "Местене от " + a2 + " в " + b2 + "\n";
		cout << output_buffer;
		sleep(2);
		move_tower(n - 1, help, to, from, printA, printB, printC, c2, b2, a2);
	} 
	
	else {
		*(++(*to)) = **from;
		*((*from)--) = "-";
		system("clear");
		print(printA, printB, printC);
		output_buffer = output_buffer + "Местене от " + a2 + " в " + b2 + "\n";
		cout << output_buffer;
		sleep(2);
	}
	
	return;
}

void print(string *a, string *b, string *c) {

	for (int i = amount_disks - 1; i >= 0; i--) {
			int spaces = amount_disks * 5 - a[i].size();
			for (int j = 0; j < spaces / 2; j++) 
				cout << " ";
			cout << a[i];
			for (int j = 0; j < spaces / 2; j++) 
				cout << " ";
			spaces = amount_disks * 5 - b[i].size();
			for (int j = 0; j < spaces / 2; j++) 
				cout << " ";
			cout << b[i];
			for (int j = 0; j < spaces / 2; j++) 
				cout << " ";
			spaces = amount_disks * 5 - c[i].size();	
			for (int j = 0; j < spaces / 2; j++) 
				cout << " ";
			cout << c[i];
			for (int j = 0; j < spaces / 2; j++) 
				cout << " ";
			cout << endl;
	}

	return;
}

