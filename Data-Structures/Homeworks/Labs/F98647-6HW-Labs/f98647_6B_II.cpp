#include <iostream>
#include <string>
#include <unistd.h>
#include <cstdlib>

/* THERE IS 100% a better way to solve this problem, this is just how I did it */

using namespace std;

void move_tower(int n, string **from, string **to, string **help);


int main() {

	for(int amount_disks = 5; amount_disks <= 30; amount_disks += 5) {
		clock_t d0, d1;
		string a[amount_disks];   
		string b[amount_disks];   
		string c[amount_disks]; 
		string *a1 = a;
		string *b1 = b;
		string *c1 = c;

		// Initializing the arrays
		for (int i = 0; i < amount_disks; i++) {
			a1[i] = "*";
			for (int j = 0; j < amount_disks - 1 - i; j++)
				a[i] += "**";
			b1[i] = "-";
			c1[i] = "-";
		}
	
		//Setting up the right addresses
		a1 += amount_disks - 1;
		b1 -= 1;
		c1 -= 1;
	
		d0 = clock();
		move_tower(amount_disks, &a1, &b1, &c1);
		d1 = clock();
	
		cout << amount_disks << ", " << (float)(d1 - d0) / CLOCKS_PER_SEC << endl;	
	}

	return 0;
}


void move_tower(int n, string **from, string **to, string **help) {
	if(n > 1) {
		move_tower(n - 1, from, help, to);
		*(++(*to)) = **from;
		*((*from)--) = "-";
		move_tower(n - 1, help, to, from);
	} 
	
	else {
		*(++(*to)) = **from;
		*((*from)--) = "-";
	}
	
	return;
}



