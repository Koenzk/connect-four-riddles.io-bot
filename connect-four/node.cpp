//
// Created by koenzk on 27-3-18.
//

#include "node.h"

Node::Node(Node* parent, State state, Move m, double utility, int visits, int type): //state is copied
        parent(parent),  state(state), move(m), utility(utility), visits(visits), type(type), children()
{
}

Node::Node(Node& node):
        Node(node.parent, node.state, node.move , node.utility, node.visits, node.type)
{

}

void Node::visit() {
    visits++;
}

void Node::addUtility(int score) {
    utility = ((utility* (visits - 1)) + score / 100.0) / static_cast<double>(visits);
}

int Node::getVisits() {
    return visits;
}

double Node::getUtility() {
    return utility;
}

Node* Node::getParent() {
    return parent;
}

State Node::getState() {
    return state;
}

Children* Node::getChildren() {
    return &children;
}

void Node::addChild(Node* n) {
    children.push_back(n);
}

Move Node::getMove()
{
    return move;
}

Node::~Node(){
    for(size_t i =0; i<children.size(); i++){
        delete children.at(i);
    }
}
