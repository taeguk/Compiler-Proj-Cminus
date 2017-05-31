void func1(int a) { return a; } // error
int func2(int b) {  }
int func3(int c) { return ; } // error
int func4(int c) { return func1(c); } // error

void main(void) {}
