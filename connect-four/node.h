//
// Created by koenzk on 27-3-18.
//

#pragma once

#include <vector>
#include "c4.h"

class Node;
typedef std::vector<Move> Moves;
typedef std::vector<Node*> Children;
class Node {
private:
    Node* parent;
    Children children;
    State state;
    Move move;
    double utility;
    int visits;
    int type; //1 for max, 2 for min, only max nodes have state
public:
    Node(Node* parent, State state, Move , double utility, int visits, int type);
    Node(Node& node);
    void visit();
    void addUtility(int score);
    int getVisits();
    double getUtility();
    Node* getParent();
    State getState();
    Children* getChildren();
    void addChild(Node* n);
    Move getMove();
    ~Node();
};

