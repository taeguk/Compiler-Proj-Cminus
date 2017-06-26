/* int save[1001]; */
int fibonacci(int a)
{
  int ret;
  if(a <= 0) ret = 0;
  else {
      if(a <= 2) ret = 1;
      else {
          ret = fibonacci(a-1) + fibonacci(a-2);
      }
  }
  
  return ret;
}

void main(void)
{
  int ret;
  ret=input();
  output( fibonacci(ret) );
}
