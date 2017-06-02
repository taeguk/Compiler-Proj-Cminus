void func(int param, int paramTwoo, int paramThreee[])
{
}

void main(void)
{
  int arr[30];
  int arrTwo[20];
  int arrThree;

  func(1, 2, arr);
  func(3, 4, arrTwo);
  func(5, 6, arrThree);

  func();
  func(1,2);
}
