#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	int depth;
	int left_idx;
	int right_idx;
} node;

void print_inorder(node *tree, int idx);
void swap_subtree(node *tree, int depth, int n);

int main() {
	/* Part : Construct a tree */
	int n;
	scanf("%d", &n);
	node *tree = calloc(n+1, sizeof(node));
	tree[1].depth = 1; // root

	for (int tree_idx = 1; tree_idx <= n; ++tree_idx) {
		int left_idx, right_idx;
		scanf("%d %d", &left_idx, &right_idx);

		tree[tree_idx].left_idx = left_idx;
		tree[tree_idx].right_idx = right_idx;

		if (left_idx != -1) tree[left_idx].depth = tree[tree_idx].depth + 1;
		if (right_idx != -1) tree[right_idx].depth = tree[tree_idx].depth + 1;
	}

	/* Part : Swap */
	int t;
	scanf("%d", &t);
	for (int t_i = 0; t_i < t; ++t_i) {
		int depth;
		scanf("%d", &depth);
		swap_subtree(tree, depth, n);
		print_inorder(tree, 1);
		printf("\n");
	}

	free(tree);

	return 0;
}

void swap_subtree(node *tree, int depth, int n) {
	for (int i = 1; i <= n; ++i) {
		if (!(tree[i].depth%depth)) {
			int temp = tree[i].left_idx;
			tree[i].left_idx = tree[i].right_idx;
			tree[i].right_idx = temp;
		}
	}
}

void print_inorder(node *tree, int idx) {
	if (tree[idx].left_idx != -1)
		print_inorder(tree, tree[idx].left_idx);

	printf("%d ", idx);

	if (tree[idx].right_idx != -1)
		print_inorder(tree, tree[idx].right_idx);
}
