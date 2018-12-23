#include "EulerTourTree.h"

node * EulerTourTree::get_node(int u) {
    if(!IDtoNode.count(u))
        return NULL;
    return IDtoNode[u];
}

node * EulerTourTree::get_edge(int u, int v) {
    pair <int ,int> p = make_pair(u, v);
    auto it = edgemap.find(p);
    if(it == edgemap.end())
        return NULL;
    return it->second;
}

void EulerTourTree::add_node(int u, node * nn) {
    BinarySearchTree::change_root(nn);
    NodeSet[u].insert(nn);
    if(!IDtoNode.count(u)) {
        IDtoNode[u]=nn;
        nn->adjacent_nodes[0] = adj_map[0][u];
        nn->adjacent_nodes[1] = adj_map[1][u];
    }
    nn->update();
}

void EulerTourTree::add_edge(int u, int v, node * nn) {
    pair <int, int> p = make_pair(u,v);
    edgemap[p] = nn;
}

void EulerTourTree::remove_node(int u, node * nn) {
    int n0 = nn->adjacent_nodes[0];
    int n1 = nn->adjacent_nodes[1];
    NodeSet[u].erase(nn);
    if(NodeSet[u].empty())
        IDtoNode.erase(u);
    else {
        node * next = *NodeSet[u].begin();
        IDtoNode[u] = next;
        BinarySearchTree::change_root(next);
        next->adjacent_nodes[0] = n0;
        next->adjacent_nodes[1] = n1;
        next->update();
    }
}

void EulerTourTree::remove_edge(int u, int v) {
    pair <int, int> p = make_pair(u,v);
    edgemap.erase(p);
}

void EulerTourTree::re_root(node * nn) {
    BinarySearchTree::change_root(nn);
    
    if(!nn->left) //nn already in front of the tree
        return;

    node * l = nn->left;
    BinarySearchTree::remove_child(l);
    node * front = BinarySearchTree::leftmost(l);
    front->left = nn;
    nn->par = front;
    front->update();
    BinarySearchTree::change_root(nn);
}

bool EulerTourTree::link(int u, int v) {
    if(is_connected(u,v))
        return false;
    node * x = get_node(u);
    node * y = get_node(v);
    if(x)
        re_root(x);
    if(y)
        re_root(y);
    node * utemp = BinarySearchTree::insert_new(x);
   // cout << "ok,y=" << v << endl;
    node * vtemp = BinarySearchTree::insert_new(y);
    utemp->value = u;
    vtemp->value = v;
    add_node(u, utemp);
    add_node(v, vtemp);
    if(!y)
        y = vtemp;
    BinarySearchTree::change_root(y);
    utemp->right = y;
    y->par = utemp;
    utemp->update();

    add_edge(u, v, utemp);
    add_edge(v, u, vtemp);
    return true;
}

bool EulerTourTree::cut(int u, int v) {
    node * x = get_edge(u, v);
    if(!x)
        return false;

    node * y = get_edge(v, u);
    re_root(x);
    BinarySearchTree::change_root(y);
    while(x->par != y)
        BinarySearchTree::rotate(x);
    BinarySearchTree::remove_child(x);

    node * next = BinarySearchTree::next_in_seq(y);
    if(next) {
        int temp = next->value;
        node * t = BinarySearchTree::rightmost(next);
        remove_edge(v,temp);
        add_edge(v, temp, t);
    }

    remove_node(u, x);
    remove_node(v, y);
    remove_edge(u, v);
    remove_edge(v, u);
    BinarySearchTree::delete_node(x);
    BinarySearchTree::delete_node(y);

    return true;
}

bool EulerTourTree::is_connected(int u, int v) {
    if(u == v)
        return true;
    node * x = get_node(u);
    node * y = get_node(v);
    if(!x || !y)
        return false;

    BinarySearchTree::change_root(x);
    BinarySearchTree::change_root(y);

    while(x->par && x->par!=y)
        BinarySearchTree::rotate(x);

    return x->par == y;

}

int EulerTourTree::size(int u) {
    node * x = get_node(u);
    if(!x)
        return 1;
    BinarySearchTree::change_root(x);
    return x->size/2 + 1;
}

int EulerTourTree::get_adjacent(int u, bool is_treeedge) {
    node * x = get_node(u);
    if(!x)
        return adj_map[is_treeedge][u] > 0 ? u : -1;
    BinarySearchTree::change_root(x);
    if(x->sum_adjacent_nodes[is_treeedge] <= 0)
        return -1;
    while(adj_map[is_treeedge][x->value] == 0) {
        node * l = x->left;
        node * r = x->right;
        if(l && l->sum_adjacent_nodes[is_treeedge] > 0)
            x = l;
        else if(r && r->sum_adjacent_nodes[is_treeedge] > 0)
            x = r;
    }

    BinarySearchTree::change_root(x);
    return x->value;
}

void EulerTourTree::update_adjacent(int u, int add_adj, bool is_treeedge) {
    adj_map[is_treeedge][u] += add_adj;
    node * x = get_node(u);
    if(!x)
        return;
    BinarySearchTree::change_root(x);
    x->adjacent_nodes[is_treeedge] += add_adj;
    x->update();
}