void helloworld(int a)
{
  a;
}

void main(void)
{
  int a;
  int arr[2];
  a = 1;

  arr[a] = 3; // pass
  arr[helloworld(a)] = 3; // fail, An array index must be int type
}
