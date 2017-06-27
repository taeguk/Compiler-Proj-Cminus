int N;
int X[21];
int Y[21];
int ans;
void dfs(int x, int y, int p, int m, int n) {
    if (p == N / 2) {
        int i;
        i = n;
        while(i<N) {
            x = x - X[i];
            y = y - Y[i];
            i = i + 1;
        }
        if (ans > x*x + y*y)
          ans = x*x + y*y;
        return;
    }
    if (m == N / 2) {
        int i;
        i = n;
        while(i<N) {
            x = x + X[i];
            y = y + Y[i];
            i = i + 1;
        }
        if (x*x + y*y)
          ans = x*x + y*y;
        return;
    }
    dfs(x + X[n], y + Y[n], p + 1, m, n + 1);
    dfs(x - X[n], y - Y[n], p, m + 1, n + 1);
}

void main(void) {
    int tc;
    int T;
    T = input();
    while(T) {
        int i;
        ans = 987651345;
        N = input();
        i = 0;
        while(i<N) {
            X[i] = input();
            Y[i] = input();
            i = i + 1;
        }
        dfs(0, 0, 0, 0, 0);
        output(ans);
        T = T - 1;
    }
    return;
}
