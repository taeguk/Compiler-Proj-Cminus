
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main() {
	int n;
	scanf("%d", &n);

	int stack[100000], top_idx = -1, max_data = -1;
	bool need_update = false;

	while (n--) {
		int type;
		scanf("%d", &type);

		switch (type) {
		case 1:
				{
					int x;
					scanf("%d", &x);
					stack[++top_idx] = x;
					if (!need_update && max_data < x)
						max_data = x;
					break;
				}
		case 2:
				{
					if (top_idx != -1) {
						--top_idx;
						need_update = true;
					}
					break;
				}
		case 3:
				{
					if (need_update) {
						max_data = -1;
						int limit = top_idx+1;
						for (int i = 0; i < limit; ++i)
							if (max_data < stack[i])
								max_data = stack[i];
					}
					printf("%d\n", max_data);    
					break;
				}
		}
	}

	return 0;
}
