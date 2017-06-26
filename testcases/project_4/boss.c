int globalA;
int globalArr[20];

int helloWorld(int a)
{
  int b;
  a = a+1;
  b = 5;

  {
    int a;
    a = b;
  }

  return a;
}

int sumTwo(int a, int b)
{
  return a + b;
}

int sumThree(int a, int b, int c)
{
  return a + b + c;
}

int sumFour (int a, int b, int c, int d)
{
  return a + b + c +d;
}

int polynomial(int n, int arr[], int x)
{
  int res;
  res = 0;
  while (n > 0)
    {
      n = n-1;
      res = res * x + arr[n];
    }
  return res;
}

void helloGlobal(int globalA, int b)
{
  globalA = b; /* fake globaA */
  globalArr[0] = b;
}

void checkGlobalA(void)
{
  int i;
  i = globalA;
  while (i)
    {
      output(globalA);
      i = i-1;
    }
}


void main(void)
{
  int a;
  int b;
  int c;
  int arr[3];
  int testArr[50];

  testArr[(a = input()) * 2] 
    = c
    = arr[0] 
    = testArr[(b = input()) * 3]
    = c
    = arr[0]
    = arr[0]
    = testArr[0]
    = input();

  output(a);
  output(b);
  output(c);
  output(arr[0]);
  output(testArr[0]);
  output(testArr[a*2]);
  output(testArr[b*3]);

  arr[0] = a = input();
  arr[1] = input();
  arr[2] = input();

  /* result: a+2 */
  output(helloWorld(helloWorld(a)));

  /* result: sum~ */
  output(sumTwo(a, arr[0]));
  output(sumTwo(arr[1], a));
  output(sumTwo(
    sumTwo(3, 5),
    sumTwo(5, 7)
  ));

  output(sumThree(arr[0], arr[1], arr[2]));
  output(sumFour(a, arr[0], arr[1], arr[2]));

  globalA = 0;
    {
      int a;
      int b;
      int globalA;

      globalA = 5;
    }

  /* helloGlobal */
  output(globalA);
  globalA = 0;
  helloGlobal(globalA, 5);
  output(globalA);
  output(globalArr[0]);


    {
      int res;
      int arr[5];

      output (888888); /* polynomial mark */

      arr[0] = 1;
      arr[1] = 2;
      arr[2] = 3;
      arr[3] = 4;
      arr[4] = 5;

      res = polynomial(5, arr, arr[0]);
      output(res);
    }

  /* if statement */
  if (globalA < 2) /* always true */
    {
      output(5);
    }
  else
    output(99999999);
  if (5 > 3) /* always true */
    {
      output(55);
    }
  if (5 == 5)
    {
      output(555);
    }

  if (a > a-1)
    {
      int a;
      a = 33;
      output(a); /* 33 */

      if (a != a-4)
        {
          int c;
          c = a;
          output(c); /* 33 */
          while(0);
        }
    }


  globalA = 3;
  while (globalA)
    {
      checkGlobalA();
      globalA = globalA-1;
    }
}
