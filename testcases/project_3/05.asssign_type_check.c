void voidFunc(void)
{
  int a;
  a = 1;
}

int intFunc(void)
{
  int a;
  return a = 1;
}

void main(void)
{
  int g;
  g = intFunc(); /* pass */
  g = voidFunc(); /* fail */
}
