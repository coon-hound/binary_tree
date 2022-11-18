#include "binary_tree.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

struct node {
	const void *key;
	uint32_t key_len;
	const void *data;
	struct node *left, *right;
};

int (*__compare)(const void *l, size_t l_len, const void *r, size_t r_len);

struct node *root = NULL;

extern void binary_tree_init(int (*compare)(const void *l, size_t l_len, const void *r, size_t r_len))
{
	__compare = compare;
}

extern void binary_tree_insert(const void *key, size_t key_len, const void *data)
{
	struct node *temp = malloc(sizeof(struct node));
	temp->key = key;
	temp->key_len  = key_len;
	temp->data = data;
	temp->left = NULL;
	temp->right = NULL;
	
	if(root == NULL){
		root = temp;
		return;
	}

	struct node *current = root;
	
	while(1){
		int ret = __compare(temp->key, temp->key_len, current->key, current->key_len);
		if(ret == 0){
			exit(0);
		}
		if(ret < 0){
			if(current->left == NULL){
				current->left = temp;
				return;
			}
			current = current->left;
		}
		if(ret > 0){
			if(current->right == NULL){
				current->right = temp;
				return;
			}
			current = current->right;
		}
	}
}

extern const void *binary_tree_find(const void *key, size_t key_len)
{
	if(root == NULL)
		return NULL;

	struct node *current = root;

	while(1){
		int ret = __compare(key, key_len, current->key, current->key_len);
		if(ret == 0){
			return current->data;	
		}
		if(ret < 0){
			if(current->left == NULL){
				return NULL;
			}
			current = current->left;
		}
		if(ret > 0){
			if(current->right == NULL){
				return NULL;
			}
			current = current->right;
		}
	}
}

void tra(struct node *n, void (*func)(const void *key, size_t key_len, const void *n))
{	
	if(n->left != NULL)
		tra(n->left, func);	

	func(n->key, n->key_len, n->data);

	if(n->right != NULL)
		tra(n->right, func);	
	
}

extern void binary_tree_traversal(void (*func)(const void *key, size_t key_len, const void *n))
{
	struct node *n = root;
	if(n == NULL)
		return;
	tra(n, func);
}

extern void binary_tree_delete_node1(const void *key, size_t key_len)
{
	struct node *current = root;
	struct node *current2 = root;
	struct node *parent;
	

	while(1){
		int ret = __compare(key, key_len, current->key, current->key_len);
		if(ret == 0){
			break;
		}
		parent = current;
		if(ret < 0){
			if(current->left == NULL){
				return;
			}
			current = current->left;

		}
		if(ret > 0){
			if(current->right == NULL){
				return;
			}
			current = current->right;
		}
	}

	if(current->right == NULL){
		if(current->left == NULL){
			if(parent->left == current)	
				parent->left = NULL;
			if(parent->right == current)	
				parent->right = NULL;
			free(current);
			return;
		}
		if(parent->left == current)
			parent->left = current->left;
		if(parent->right == current)
			parent->right = current->left;
		free(current);
		return;
	}
	current2 = current->right;
	struct node *current2_parent;
	while(1){
		if(current2->left == NULL){
			break;
		}	
		current2_parent = current2;
		current2 = current2->left;
	}
	current2_parent->left = NULL;
	if(parent->left == current)
		parent->left = current2;
	if(parent->right == current)
		parent->right = current2;
	current2_parent->right = current2->right;
	
	free(current);
}

void delete_node2(struct node *current, struct node *parent)
{
	if(current->right != NULL){
		current->data = current->right->data;
		current->key = current->right->key;
		current->key_len = current->right->key_len;

		delete_node2(current->right, current);
		return;
	}
	if(current->right == NULL){
		parent->right = NULL;
		free(current);
	}
}

extern void binary_tree_delete_node2(const void *key, size_t key_len)
{
	struct node *current = root;
	struct node *parent;
	while(1){
		int ret = __compare(key, key_len, current->key, current->key_len);
		if(ret == 0){
			break;
		}
		parent = current;
		if(ret < 0){
			if(current->left == NULL){
				return;
			}
			current = current->left;

		}
		if(ret > 0){
			if(current->right == NULL){
				return;
			}
			current = current->right;
		}
	}
	delete_node2(current, parent);	
}

void delete_node(struct node *del, struct node *parent)
{
	if(parent == NULL)
		free(del);

	if(parent->left == del){
		parent->left = NULL;
	}
	else if (parent->right == del){
		parent->right = NULL;
	}
	free(del);
}

void delete_subtree(struct node *current, struct node *prev)
{
	delete_subtree(current->left, current);
	delete_subtree(current->right, current);
	delete_node(current, prev);
}

extern void binary_tree_delete_subtree(const void *key, size_t key_len)
{
	if(root == NULL)
		return;

	struct node *current = root;

	struct node *subtree;
	struct node *prev = NULL;

	while(1){
		int ret = __compare(key, key_len, current->key, current->key_len);
		if(ret == 0){
			subtree = current;
			break;
		}
		if(ret < 0){
			if(current->left == NULL){
				return;
			}
			prev = current;
			current = current->left;
		}
		if(ret > 0){
			if(current->right == NULL){
				return;
			}
			prev = current;
			current = current->right;
		}
	}
	delete_subtree(subtree, prev);
}

extern void binary_tree_destroy()
{
	delete_subtree(root, NULL);
}
