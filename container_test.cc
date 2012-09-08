#include <stdio.h>
#include <stdlib.h>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <list>
using namespace std;

#define Node_ptr boost::shared_ptr<Node> 

class Node {
    public:
        Node(Node_ptr new_parent, int new_value);
        Node_ptr parent;
        int value;
        void print();
        ~Node();
};

Node::Node(Node_ptr new_parent, int new_value) {
    parent = parent = new_parent;
    value = new_value;
}

Node::~Node() {
//    printf("destroy\n");
}

void Node::print() {
    printf("&=%p parent&=%p value=%d\n", this, parent.get(), value);
}

void test_vector();

void test_list();

int main(int argc, char** argv) {
    printf("vector\n");
    test_vector();    

//    printf("\nlist\n");
//    test_list();
    printf("end\n");
}

void test_vector() {
    vector<Node_ptr> nodes;

    printf("original nodes\n");
    
    Node_ptr parent = Node_ptr();
    for (int i = 0; i < 10; i++) {
        Node_ptr node = Node_ptr(new Node(parent, i));
        parent = node;
        nodes.push_back(node);
        (node.get())->print();
    }
    
    printf("\iterated nodes\n");
    
    vector<Node_ptr>::iterator it; 
	for (it = nodes.begin(); it != nodes.end(); it++ ) {
	    (*it).get()->print();
	}
}
/*
void test_list() {
    list<Node> nodes;

    printf("original nodes\n");
    
    Node *parent = NULL;
    for (int i = 0; i < 10; i++) {
        Node *node = new Node(parent, i);
        parent = node;
        nodes.push_back(*node);
        node->print();
    }
    
    printf("\iterated nodes\n");
    
    list<Node>::iterator it; 
	for (it = nodes.begin(); it != nodes.end(); it++ ) {
	    it->print();
	}

}
*/