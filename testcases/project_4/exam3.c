int save[1001];
int fibonacci(int a) {
    if(a <= 0)
      return 0;
    else if(a <= 2)
      return 1;
      {
	int k;
	k=a;
	  {
	    int y[10];
	    y[5]=0;
	    if(save[k]!=y[5]) return save[k];
	  }
	  {
	    int h;
	    h=fibonacci(k-1);
	      {
		int g;
		g=fibonacci(k-2);
		save[a]=h+g;
		  {
		    int aaa[400];
		    aaa[333]=save[a];
		    return aaa[333];
		  }
	      }
	  }
      }


}

void main(void) {
    int ret;
    ret=input();
    output( fibonacci(ret) );
    /* ret must has 610 : 0x0262*/
}
