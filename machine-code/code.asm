void abc() {
	pushi(ebp);
	move(esp,ebp);
	addi(-4,esp);
	move(20,eax);
	intTofloat(eax);
	storef(eax,ind(ebp,-4));
	loadf(ind(ebp,-4),eax);
	move(20,eax);
	intTofloat(eax);
	cmpf(ebx,eax);
	jne(l0);
	move(10,eax);
	storei(eax,ind(ebp,0));
	jl(l1);
l0:
	move(5,eax);
	storei(eax,ind(ebp,0));
l1:
	pushi(0);
	loadi(ind(ebp,0),eax);
	print_int(eax);
	loadi(ind(esp),eax);
	popi(1);
	loadf(ind(ebp,-4),eax);
	floatToint(eax);
	storei(eax,ind(ebp, 8));
	addi(4,esp);
	popi(1);
	return;
}

void main() {
	pushi(ebp);
	move(esp,ebp);
	addi(-4,esp);
	pushi(0);
	pushi(5);
	abc();
	popi(1);
	loadi(ind(esp),eax);
	popi(1);
	storei(0,ind(ebp,12));
	addi(4,esp);
	popi(1);
	return;
}