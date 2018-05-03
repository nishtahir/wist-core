#ifndef __NODE_H__
#define __NODE_H__
#include <iostream>

using namespace std;

struct Node
{
    string ruleName;
};

struct TreeNode
{
    Node node;
    vector<TreeNode> children;
};

#endif