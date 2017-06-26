int a;
int b[100];
int arr[10];

void main(void)
{
  a = input();
  output(a);

  b[5] = input() + a;
  output(b[5]);

  arr[3] = input() + b[5];
  output(arr[3]);
}
