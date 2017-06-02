void func(int param1, int param2, int param3[])
{
}

void main(void)
{
  int arr[30];
  int arr2[20];
  int arr3;

  func(1, 2, arr);
  func(3, 4, arr2);
  func(5, 6, arr3);

  func();
  func(1,2);
}
