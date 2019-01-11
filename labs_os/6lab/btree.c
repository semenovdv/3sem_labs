#include <stdlib.h>
#include <stdio.h>
#include "btree.h"
/*
int max(int a, int b){
	if(a > b) return a;
	return b;
}

void print_tnode(tnode *tn, int depth){
	if(tn == NULL && depth == 0) printf("Empty!\n");
	if(tn == NULL){
		return;
	}
	print_tnode(tn->right, depth+1);
	for(long i = 0; i < depth; i++){
		printf("  ");
	}
	printf("%d\n", tn->data);

	print_tnode(tn->left, depth+1);
}
*/

tnode** find_node(tnode **t, int val){
	if(*t == NULL) return NULL;
	if((*t)->id < val) return find_node(&((*t)->right), val);
	if((*t)->id > val) return find_node(&((*t)->left), val);
	return t;
}

void delete_node(tnode **tn){
	if(*tn == NULL) return;
	
	if((*tn)->right == NULL){
		tnode *tmp = (*tn)->left;
		free(*tn);
		(*tn) = tmp;
	}
	
	else if((*tn)->left == NULL){
		tnode *tmp = (*tn)->right;
		free(*tn);
		(*tn) = tmp;
	}
	else{
		tnode **tmp = &(*tn)->left;

		while((*tmp)->right != NULL){
			tmp = &(*tmp)->right;
		}
		(*tn)->id = (*tmp)->id;
		(*tn)->debet_acc = (*tmp)->debet_acc;
		(*tn)->credit_acc = (*tmp)->credit_acc;
		free(tmp);
		
	}
}
void add(tnode **tn, double val){
	if(*tn == NULL) {
		(*tn) = (tnode*)malloc(sizeof(tnode));
		(*tn)->left = NULL;
		(*tn)->right = NULL;
		(*tn)->id = val;
		(*tn)->debet_acc = 0;
		(*tn)->credit_acc = 0;	
	}else if(val < (*tn)->id){
		add(&((*tn)->left), val);
	}else if(val > (*tn)->id){
		add(&((*tn)->right), val);
	}
}
/*
int depth_tree(tnode *tn){
	if(!tn){
		return 1;
	}else{
		return max(depth_tree(tn->left)+1, depth_tree(tn->right)+1);
	}
}*/