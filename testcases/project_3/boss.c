int globalVar;
int g;

int helloworld(int a)
{
  int level;
  level = 1;

  a = a+1;
  return a;
}

void voidworld(void)
{
  int a;
  int level;
  level = 2;

  a = 1;
}

void main (void)
{
  int intArray[10];
  int a;
  int level;
  level = 1;

  a = 5;

  while (a)
    {
      int a;
      int arr[10];
      int intArray[5];
      int level;
      level = 2;

      a = 1;
      intArray[0] = 2;

      arr[intArray[0]] = helloworld(a);

      voidworld();
      if (arr[0])
        {
          int a;
          int level;

          level = 3;

          if (a)
            {
              int a;
              int b;
              int level;
              level = 4;

              if (a)
                {
                  int c;
                  int level;
                  level = 5;
                }
              if (b)
                {
                  int level;
                  level = 5;
                  globalVar = 1;
                  if (g)
                    {
                      int anotherLevel;
                      int level;
                      level = 6;

                      if (g)
                        {
                          int newLevel;
                          int level;
                          level = 7;

                          if (newLevel)
                            {
                              int superStack;
                              int level;
                              level = 8;

                              if (superStack)
                                {
                                  int hell;
                                  int level;
                                  level = 9;

                                  if (hell)
                                    {
                                      int pass;
                                      int level;
                                      level = 10;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }


      if (arr[1])
        {
          int a;
          int b;
          int c;
          int level;
          level = 3;

          if (b)
            {
              int arr[10];
              int intArray[3];
              int level;
              level = 4;
              arr[212];
            }
        }
    }
}
