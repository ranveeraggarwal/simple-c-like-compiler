/*

The actual code generated should not have I and F, it should be replaced with size of integer
and float of the target machine.

*/

void main()
{
	// Code for fact(6)
	pushi(0); //To make space in stack for return val
	pushi(6); // argument to fact
	fact(); // invoking fact
	popi(1); // Clenaup parameters
	loadi(ind(esp), eax);
	print_int(eax); // Special method to print contents of registers
	print_char('\n');
	popi(1); // Clean up return value

	// Code for avg3floats(4.8, 7, 5.6)
	pushf(0.0); //To make space in stack for return val
	pushf(4.8); //param1
	move(7, eax); //param2 - requires casting
	intTofloat(eax);
	pushf(eax);
	pushf(5.6); //param3
	avg3floats();
	popf(3); //Cleanup parameters
	loadf(ind(esp), eax);
	print_float(eax); // Special method to print contents of registers
	print_char('\n');
	popf(1); // pop the return value

	return;
}

//Function to compute factorial
//int fact(int n)
void fact()
{
	pushi(ebp); // Setting dynamic link
	move(esp,ebp); // Setting dynamic link
	loadi(ind(ebp, I), eax); // load n to eax
	cmpi(0, eax);
	jne(l1); // Jump if not equal
	storei(1, ind(ebp, 2*I)); // Save the return value in stack
	j(e); // Unconditional jump

l1:	loadi(ind(ebp, I), eax); // load n to eax
	pushi(eax); //Save register
	loadi(ind(ebp, I), eax); // load n to eax
	addi(-1, eax); // compute (n-1)
	pushi(0); //To make space in stack for return value
	pushi(eax); // argument to fact
	fact(); // call fact
	loadi(ind(esp, I), ebx); // receives the return value
	popi(2); // pop the stack after func call
	loadi(ind(esp), eax); //Load saved register
	popi(1); // pop the stack after loading saved regs
	muli(eax, ebx); // result is in ebx
	storei(ebx, ind(ebp, 2*I)); // Save the return value in stack

e:	loadi(ind(ebp), ebp); // restoring dynamic link
	popi(1); //pop stack
	return; //return
}

// Function to find average of 3 floating point numbers
// float avg3floats(float a, float b, float c) return a*b*c;
void avg3floats()
{
	pushi(ebp); // Setting dynamic link
	move(esp,ebp); // Setting dynamic link
	loadf(ind(ebp, 2*F + I), eax); // load a to eax
	loadf(ind(ebp, F + I), ebx); // load b to ebx
	mulf(eax, ebx);
	move(ebx, eax);
	loadf(ind(ebp, I), ebx); // load c to ebx
	mulf(eax, ebx);
	storef(ebx, ind(ebp, 3*F + I)); // Save the return value in stack

e:	loadi(ind(ebp), ebp); // restoring dynamic link
	popi(1); //pop stack
	return; //return
}











