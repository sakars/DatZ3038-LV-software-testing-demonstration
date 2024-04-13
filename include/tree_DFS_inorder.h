

#include <memory>
#include <set>
#include <stack>
#include <stdexcept>
#include <vector>
#pragma once

/// @brief Helper struct for a node of a binary tree, with left and right children
/// NOTE FOR TESTERS: This struct is not the object of the test, it is just a necessary class for DFS in-order traversal.
/// @tparam T type of data stored in the node.
/// @note The node is a shared pointer, so you can use it as a pointer without worrying about memory management.
template <typename T>
struct Node {
    T data;
    std::shared_ptr<Node<T>> left = nullptr;
    std::shared_ptr<Node<T>> right = nullptr;

    Node(std::remove_reference_t<T>&& data)
        : data(data)
    {
    }
    Node(const std::remove_reference_t<T>& data)
        : data(data)
    {
    }
    Node(const std::remove_reference_t<T> data)
        : data(data)
    {
    }
};

/// @brief in-order depth-first iteration of a binary tree
/// In-order means that the left child is visited first, then the current node, then the right child.
/// If the tree is malformed (e.g. a node appears twice or a loop is present), the function throws.
///
/// NOTE FOR TESTERS: This is the function to test.
///
/// @tparam T type of data stored in the node.
/// @param root the root of the tree. Throws if the root is null.
/// @return a vector of shared pointers to the nodes of the tree, in the order they should be visited.
template <typename T>
std::vector<std::shared_ptr<Node<T>>> tree_DFS_inorder(const std::shared_ptr<Node<T>>& root)
{
    std::vector<std::shared_ptr<Node<T>>> result;
    std::stack<std::shared_ptr<Node<T>>> stack;
    std::set<std::shared_ptr<Node<T>>> visited;

    // First verify that the tree is there.
    if (!root) {
        throw std::runtime_error("Error: tree is malformed, root is null");
    }
    auto n = root;
    // Go to the leftmost node, while queuing the intermediary nodes in the stack
    do {
        if (visited.find(n) == visited.end()) {
            stack.push(n);
            visited.insert(n);
            n = n->left;
        } else {
            // If at any point we find a node that we have already visited, the tree must be malformed.
            // This is because in a tree, a node should only appear once.
            throw std::runtime_error("Error: tree is malformed, node appeared twice");
        }
    } while (n);

    // Now we are at the leftmost node, so we can start popping the stack
    // and visiting the nodes. At each node, we go to the right child if it exists,
    // and then go to the leftmost node of the right subtree.
    // We repeat this until the stack is empty.
    while (!stack.empty()) {
        auto current = stack.top();
        result.push_back(current);
        stack.pop();
        auto n = current->right;
        while (n) {
            // We perform the same operation as before, but with the right child.
            if (visited.find(n) == visited.end()) {
                stack.push(n);
                visited.insert(n);
                n = n->left;
            } else {
                throw std::runtime_error("Error: tree is malformed, node appeared twice");
            }
        }
    }
    return result;
}