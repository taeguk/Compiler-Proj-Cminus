void main(void){
    int a;
    int b;
    int c;
    int d;
    a = 1;
    b = 2;
    c = 3;
    d = 4;

    output(a+b);
    output(a-b);
    output(d/b);
    output(d*b);
    a = a + b + ((c + d) + 5 + 6) + 7 + 8 * 9;
    output(a);
    b = c * d - b;
    output(b);
    c = d / (c - 1);
    output(c);
    d = a / b - c * c;
    output(d);
}
