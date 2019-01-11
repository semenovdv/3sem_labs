#ifndef BTREE_H
#define BTREE_H

struct tnode{
	int id;
	long debet_acc;
	long credit_acc;
	struct tnode *left;
	struct tnode *right;
};

typedef struct tnode tnode;


//int max(int a, int b);
//void print_tnode(tnode *tn, int depth);
tnode** find_node(tnode **t, int val);
void delete_node(tnode **tn);
void add(tnode **tn, double val);
//int depth_tree(tnode *tn);

#endif