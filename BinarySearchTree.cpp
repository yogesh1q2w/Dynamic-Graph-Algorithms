#include "BinarySearchTree.h"
#include <iostream>
using namespace std;

//----------------node functions----------------------------

node::node() {
    size = 1;
    value = adjacent_nodes[0] = adjacent_nodes[1] = sum_adjacent_nodes[0] = sum_adjacent_nodes[1] = 0;
    left = right = par = NULL;
}


void node::update() {
    size = 1;
    sum_adjacent_nodes[0] = adjacent_nodes[0];
    sum_adjacent_nodes[1] = adjacent_nodes[1];
    if(left) {
        size += left->size;
        sum_adjacent_nodes[0] += left->sum_adjacent_nodes[0];
        sum_adjacent_nodes[1] += left->sum_adjacent_nodes[1];
    }
    if(right) {
        size += right->size;
        sum_adjacent_nodes[0] += right->sum_adjacent_nodes[0];
        sum_adjacent_nodes[1] += right->sum_adjacent_nodes[1];
    }
}

//----------------BinarySearchTree functions------------------

node * BinarySearchTree::insert_new(node *nn) {
    if(!nn)
        return new node();
    change_root(nn);
    while(nn->right)
        nn = nn->right;
    node * newnode = new node();
    newnode->par = nn;
    nn->right = newnode;
    nn->update();
    change_root(newnode);
    return newnode;
}


void BinarySearchTree::rotate_left(node* nn) {
    node * p = nn->par;
    node * gp = p->par;
    node * lc = nn->left;
    nn->par = gp;

    if(gp) {
        if(p == gp->left)
            gp->left = nn;
        else
            gp->right = nn;
    }

    nn->left = p;
    p->par = nn;

    p->right = lc;
    if(lc)
        lc->par = p;
    p->update();
    nn->update();
}

void BinarySearchTree::rotate_right(node * nn) {
    node * p = nn->par;
    node * gp = p->par;
    node * rc = nn->right;
    nn->par = gp;

    if(gp) {
        if(p == gp->left)
            gp->left = nn;
        else
            gp->right = nn;
    }

    nn->right = p;
    p->par = nn;

    p->left = rc;
    if(rc)
        rc->par = p;
    p->update();
    nn->update();
}

void BinarySearchTree::rotate(node * nn) {
    node * p = nn->par;
    if(!p)
        return;
    if(nn == p->left)
        rotate_right(nn);
    else
        rotate_left(nn);
}

void BinarySearchTree::remove_child(node * nn) {
    node * p = nn->par;
    if(!p)
        return;
    if(nn == p->left)
        p->left = NULL;
    else
        p->right = NULL;
    nn->par = NULL;
    p->update();
}

void BinarySearchTree::change_root(node * nn) {
    if(!nn)
        return;
    while(nn->par) {
        node * p = nn->par;
        node * gp = p->par;
        if(!gp) {
            rotate(nn);
            break;
        }

        if((nn == p->left) == (p == gp->left)) {
            rotate(p);          //left child, left child case
            rotate(nn);             //or right, right case
        }
        else {
            rotate(nn);         //zig zag 
            rotate(nn);
        }
    }
}

void BinarySearchTree::delete_node(node * nn) {
    change_root(nn);

    node * l = nn->left;
    node * r = nn->right;

    if(!l && !r) {
        return;
    }
    else if(!l) {
        remove_child(r);
    }
    else if(!r) {
        remove_child(l);
    }
    else {
        remove_child(l);
        remove_child(r);
        node * newroot = leftmost(r);
        newroot->left = l;
        newroot->update();
    }

}

node * BinarySearchTree::next_in_seq(node * nn) {
    change_root(nn);
    nn = nn->right;
    if(!nn)
        return NULL;
    while(nn->left)
        nn = nn->left;
    change_root(nn);
    return nn;
}

node * BinarySearchTree::leftmost(node * nn) {
    change_root(nn);
    while(nn->left)
        nn = nn->left;
    change_root(nn);
    return nn;
}

node * BinarySearchTree::rightmost(node * nn) {
    change_root(nn);
    while(nn->right)
        nn=nn->right;
    change_root(nn);
    return nn;
}
