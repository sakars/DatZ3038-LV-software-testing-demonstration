#include "tree_DFS_inorder.h"
#include <catch_amalgamated.hpp>

std::shared_ptr<Node<int>> generate_random_tree(size_t node_count)
{
    std::shared_ptr<Node<int>> root = std::make_shared<Node<int>>(0);
    std::stack<std::tuple<size_t, size_t, std::shared_ptr<Node<int>>>> stack;
    stack.push({ node_count, 0, root });
    while (!stack.empty()) {
        auto [n, offset, node] = stack.top();
        stack.pop();
        if (n > 1) {
            size_t left = GENERATE(take(1, random(0, RAND_MAX))) % (n - 1);
            size_t right = n - left - 1;
            node->data = offset + left;
            if (left > 0) {
                node->left = std::make_shared<Node<int>>(0);
                stack.push({ left, offset, node->left });
            }
            if (right > 0) {
                node->right = std::make_shared<Node<int>>(0);
                stack.push({ right, offset + left + 1, node->right });
            }
        } else {
            node->data = offset;
        }
    }
    return root;
}

TEST_CASE("Empty tree", "[tree_DFS_inorder]")
{
    std::shared_ptr<Node<int>> root = nullptr;
    REQUIRE_THROWS(tree_DFS_inorder(root));
}

TEST_CASE("Left chain with a loop (Malformed tree)", "[tree_DFS_inorder]")
{
    int chain_length = GENERATE(take(10, random(6, 100)));
    int reference_index = GENERATE_COPY(take(1, random(1, chain_length - 1)));
    std::shared_ptr<Node<int>> root = std::make_shared<Node<int>>(0);
    std::shared_ptr<Node<int>> n = root;
    std::shared_ptr<Node<int>> reference_node = nullptr;
    for (int i = 1; i < chain_length; i++) {
        if (i == reference_index) {
            reference_node = n;
        }
        n->left = std::make_shared<Node<int>>(i);
        n = n->left;
    }
    n->left = reference_node;
    REQUIRE_THROWS(tree_DFS_inorder(root));
}

TEST_CASE("Left chain", "[tree_DFS_inorder]")
{
    int chain_length = GENERATE(take(10, random(6, 100)));
    std::shared_ptr<Node<int>> root = std::make_shared<Node<int>>(0);
    std::shared_ptr<Node<int>> n = root;
    for (int i = 1; i < chain_length; i++) {
        n->left = std::make_shared<Node<int>>(i);
        n = n->left;
    }

    auto result = tree_DFS_inorder(root);
    REQUIRE(result.size() == chain_length);
    for (int i = 0; i < chain_length; i++) {
        REQUIRE(result[chain_length - i - 1]->data == i);
    }
}
TEST_CASE("Random tree", "[tree_DFS_inorder]")
{
    auto node_count = GENERATE(take(200, random(3, 4000)));
    auto root = generate_random_tree(node_count);
    auto order = tree_DFS_inorder(root);
    for (size_t i = 0; i < order.size(); i++) {
        REQUIRE(order[i]->data == i);
    }
}
TEST_CASE("Random tree with the same node twice (malformed)", "[tree_DFS_inorder]")
{
    // Create a tree with a node that appears twice
    auto node_count = GENERATE(take(200, random(3, 4000)));
    auto root = generate_random_tree(node_count);
    auto order = tree_DFS_inorder(root);
    for (size_t i = 0; i < order.size(); i++) {
        REQUIRE(order[i]->data == i);
    }

    auto swapped_node = GENERATE_COPY(take(1, random(1, node_count - 1)));
    auto node = order[swapped_node];
    if (!node->left && !node->right) {
        node = order[swapped_node - 1];
    }
    if (node->left) {
        node->left = root;
    } else {
        node->right = root;
    }
    REQUIRE_THROWS(tree_DFS_inorder(root));
}
