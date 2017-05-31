void void_func()
{
  int a;
  a = 1;
}

int int_func()
{
  int a;
  return a = 1;
}

void main()
{
  int g;
  g = int_func(); /* pass */
  g = void_func(); /* fail */
}
