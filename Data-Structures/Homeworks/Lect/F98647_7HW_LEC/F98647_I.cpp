#include <iostream>

typedef struct stack {
	struct stack *p;
	char ch;
} stack_t;


void stack_init(stack **st);
void stack_add(char add, stack **st);
void stack_get(stack **st);

int main() {
	char charact;
	stack_t *my_stack;
	
	stack_init(&my_stack); //Initializing stack with NULL and '0'
	
	for (int i = 0; i < 3; i++) {
		std::cout << "Enter char: ";
		std::cin >> charact;
		stack_add(charact, &my_stack); // Basically entering some garbage values in the stack
	}
	
	while(my_stack->p != NULL)
		stack_get(&my_stack);		// Printing the values out
}

void stack_init(stack **st) {
	*st =  new stack;
	(*st)->p = NULL;
	(*st)->ch = '0';
	
	return;
}
void stack_add(char add, stack **st) {
	struct stack *new_stack;
	new_stack = new stack;
	
	new_stack->ch = add;
	new_stack->p = (*st);
	(*st) = new_stack;
	
	return;
}

void stack_get(stack **st) {
	stack_t *holder;
	holder = *st;
	std::cout << (*st)->ch;
	(*st) =  (*st)->p;
	delete holder;	
		
	return;
}
