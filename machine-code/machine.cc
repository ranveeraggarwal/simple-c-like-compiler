#include<iostream>
#define STACK_SIZE 1000

#define j(l) goto l
#define je(l) if(_flag==0) goto l
#define jne(l) if(_flag!=0) goto l
#define jl(l) if(_flag==-1) goto l
#define jle(l) if(_flag==-1||_flag==0) goto l
#define jg(l) if(_flag==1) goto l
#define jge(l) if(_flag==1||_flag==0) goto l

using namespace std;

class CMachineBase {
protected:

	int _flag;
	enum Reg {
		eax, ebx, ecx, edx, esp, ebp
	};
	int I, F;
	void intTofloat(Reg reg) {
		char* r = getReg(reg);
		*((float*) r) = (float) (*((int *) r));
	}
	void floatToint(Reg reg) {
		char* r = getReg(reg);
		*((int*) r) = (int) (*((float *) r));
	}
	void move(Reg rSrc, Reg rDes) {
		char* src = getReg(rSrc);
		char* des = getReg(rDes);
		for (int i = 0; i < F; i++) {
			des[i] = src[i];
		}
	}

	void move(int i, Reg r) {
		char* reg = getReg(r);
		*((int*) reg) = i;
	}

	void move(double f, Reg r) {
		char* reg = getReg(r);
		*((float*) reg) = (float) f;
	}

	// Load an  integer from memory to register
	void loadi(char* mem, Reg r) {
		char* reg = getReg(r);
		*((int*) reg) = *((int*) mem);
	}

	// Load a floating point number from memory to register
	void loadf(char* mem, Reg r) {
		char* reg = getReg(r);
		*((float*) reg) = *((float*) mem);
	}

	// Store an immediate integer to memory
	void storei(int i, char* mem) {
		*((int*) mem) = i;
	}

	// Store an integer from register to memory
	void storei(Reg r, char* mem) {
		char* reg = getReg(r);
		*((float*) mem) = *((float*) reg);
	}

	// Store an immediate floating point number to memory
	void storef(double i, char* mem) {
		*((float*) mem) = (float)i;
	}

	// Store a floating point number from register to memory
	void storef(Reg r, char* mem) {
		char* reg = getReg(r);
		*((float*) mem) = *((float*) reg);
	}

	void addi(int i, Reg r) {
		char* reg = getReg(r);
		int j = *((int*) reg);
		int ans = i + j;
		*((int*) reg) = ans;
	}
	void addi(Reg rSrc, Reg rDes) {
		char* src = getReg(rSrc);
		char* des = getReg(rDes);
		int i = *((int*) src);
		int j = *((int*) des);
		int ans = i + j;
		*((int*) des) = ans;
	}

	void addf(double i, Reg r) {
		char* reg = getReg(r);
		float j = *((float*) reg);
		float ans = ((float)i) + j;
		*((float*) reg) = ans;
	}

	void addf(Reg rSrc, Reg rDes) {
		char* src = getReg(rSrc);
		char* des = getReg(rDes);
		float i = *((float*) src);
		float j = *((float*) des);
		float ans = i + j;
		*((float*) des) = ans;
	}

	void muli(int i, Reg r) {
		char* reg = getReg(r);
		int j = *((int*) reg);
		int ans = i * j;
		*((int*) reg) = ans;
	}
	void muli(Reg rSrc, Reg rDes) {
		char* src = getReg(rSrc);
		char* des = getReg(rDes);
		int i = *((int*) src);
		int j = *((int*) des);
		int ans = i * j;
		*((int*) des) = ans;
	}

	void mulf(double i, Reg r) {
		char* reg = getReg(r);
		float j = *((float*) reg);
		float ans = ((float)i) * j;
		*((float*) reg) = ans;
	}

	void mulf(Reg rSrc, Reg rDes) {
		char* src = getReg(rSrc);
		char* des = getReg(rDes);
		float i = *((float*) src);
		float j = *((float*) des);
		float ans = i * j;
		*((float*) des) = ans;
	}

	void divi(int i, Reg r) {
		char* reg = getReg(r);
		int j = *((int*) reg);
		int ans = i / j;
		*((int*) reg) = ans;
	}
	void divi(Reg rSrc, Reg rDes) {
		char* src = getReg(rSrc);
		char* des = getReg(rDes);
		int i = *((int*) src);
		int j = *((int*) des);
		int ans = i / j;
		*((int*) des) = ans;
	}

	void divf(double i, Reg r) {
		char* reg = getReg(r);
		float j = *((float*) reg);
		float ans = ((float)i) / j;
		*((float*) reg) = ans;
	}

	void divf(Reg rSrc, Reg rDes) {
		char* src = getReg(rSrc);
		char* des = getReg(rDes);
		float i = *((float*) src);
		float j = *((float*) des);
		float ans = i / j;
		*((float*) des) = ans;
	}

	void cmpi(Reg r1, Reg r2) {
		char* reg1 = getReg(r1);
		char* reg2 = getReg(r2);
		int val1 = *((int*) reg1);
		int val2 = *((int*) reg2);
		if (val1 == val2)
			_flag = 0;
		else if (val1 < val2)
			_flag = -1;
		else
			_flag = 1;
	}

	void cmpi(int val1, Reg r2) {
		char* reg2 = getReg(r2);
		int val2 = *((int*) reg2);
		if (val1 == val2)
			_flag = 0;
		else if (val1 < val2)
			_flag = -1;
		else
			_flag = 1;
	}

	void cmpf(Reg r1, Reg r2) {
		char* reg1 = getReg(r1);
		char* reg2 = getReg(r2);
		float val1 = *((float*) reg1);
		float val2 = *((float*) reg2);
		if (val1 == val2)
			_flag = 0;
		else if (val1 < val2)
			_flag = -1;
		else
			_flag = 1;
	}

	void cmpf(double val1, Reg r2) {
		char* reg2 = getReg(r2);
		float val2 = *((float*) reg2);
		if (((float)val1) == val2)
			_flag = 0;
		else if (((float)val1) < val2)
			_flag = -1;
		else
			_flag = 1;
	}

	void pushi(int val) {
		*((char **) _esp) -= I;
		**((int**) _esp) = val;
	}

	void pushi(Reg r) {
		char* reg = getReg(r);
		*((char **) _esp) -= I;
		int val = *((int*) reg);
		**((int **) _esp) = val;
	}

	void pushf(double val) {
		*((char **) _esp) -= F;
		**((float**) _esp) = (float)val;
	}

	void pushf(Reg r) {
		char* reg = getReg(r);
		*((char **) _esp) -= F;
		float val = *((float*) reg);
		**((float **) _esp) = val;
	}

	// Pops 'i' integers from the stack
	void popi(int i) {
		*((char **) _esp) += i * I;
	}

	// Pops 'i' number of floats from the stack
	void popf(int i) {
		*((char **) _esp) += i * F;
	}

	// Register indirection operator
	char* ind(Reg r) {
		char* reg = getReg(r);
		return ((*((char**) (reg))));
	}

	// Register indirection operator
	char* ind(Reg r, int offset) {
		char* reg = getReg(r);
		return ((*((char**) (reg))) + offset);
	}

	// Special print methods
	void print_int(Reg r) {
		char* reg = getReg(r);
		int val = *((int*) reg);
		cout << val;
	}

	void print_int(int i) {
		cout << i;
	}

	void print_float(Reg r) {
		char* reg = getReg(r);
		float val = *((float*) reg);
		cout << val;
	}

	void print_float(double i) {
		cout << ((float)i);
	}

	void print_char(Reg r) {
		char* reg = getReg(r);
		char val = *((char*) reg);
		cout << val;
	}

	void print_char(char val) {
		cout << val;
	}

	void print_string(Reg r) {
		char* reg = getReg(r);
		cout << (char*) reg;
	}

	void print_string(char* val) {
		cout << val;
	}

	CMachineBase() {
		I = sizeof(int);
		F = sizeof(float);
		int max = F>I?F:I;
		_flag = 0;
		_eax = new char[max];
		_ebx = new char[max];
		_ecx = new char[max];
		_edx = new char[max];
		_ebp = new char[max];
		_esp = new char[max];
		*((char**) _ebp) = &_stack[STACK_SIZE - 1];
		*((char**) _esp) = &_stack[STACK_SIZE - 1];
	}

	virtual ~CMachineBase() {
		delete[] _eax;
		delete[] _ebx;
		delete[] _ecx;
		delete[] _edx;
		delete[] _ebp;
		delete[] _esp;
	}

	virtual void main() {

	}
public:
	void execute() {
		this->main();
	}

private:
	char *_eax, *_ebx, *_ecx, *_edx, *_esp, *_ebp;
	char _stack[STACK_SIZE];

	char* getReg(Reg r) {
		switch (r) {
		case eax:
			return _eax;
		case ebx:
			return _ebx;
		case ecx:
			return _ecx;
		case edx:
			return _edx;
		case esp:
			return _esp;
		case ebp:
			return _ebp;
		default:
			return (char*) 0;
		}
	}
};

class CMachine: public CMachineBase {
#include "code.asm"
};

int main() {
	CMachine M;
	M.execute();
}
