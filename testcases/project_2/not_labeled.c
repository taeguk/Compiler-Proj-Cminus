/* binary_search program */
int arr[11111];
int binarySearch(int x){
    int left; int right; int mid;
    left=0;
    right=1111;
    while(left<=right){
        mid = (left+right)/2;
        if(mid==x) return mid;
        else if(mid<x) left=mid+1;
        else right=mid-1;
    }
    return -1;
}

int main() {
    int i = 0, goal, res;
    while (i < 11111)
      {
        arr[i] = i;
        i = i + 1;
      }
    goal=100;
    res = binarySearch(goal);
    return 0;
}

