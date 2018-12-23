#ifndef BinarySearchTree_H
#define BinarySearchTree_H

class node {
public:
	node();
	node *left;		//left child
	node *right;	//right child
	node *par;		//parent
	int value;
	int size;		//size of subtree
	int adjacent_nodes[2];		//no. of adjacent nodes in graph in fully dynamic-tree(1) and non -tree(0)
	int sum_adjacent_nodes[2];		//sum of adjacent_nodes in subtree
	void update();		//update size,value,etc
};

class BinarySearchTree {
public:
	static void rotate(node * nn);
	static void delete_node(node * nn);
	static void change_root(node * nn);
	static void remove_child(node * nn);
	static node * leftmost(node * nn);
	static node * rightmost(node * nn);
	static node * next_in_seq(node * nn);
	static node * insert_new(node * nn);

private:
    static void rotate_right(node * nn);
	static void rotate_left(node * nn);
};

#endif
