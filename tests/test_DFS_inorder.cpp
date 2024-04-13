#include "tree_DFS_inorder.h"
#include <catch_amalgamated.hpp>

TEST_CASE("Malformed trees", "[tree_DFS_inorder]")
{
    SECTION("Empty tree")
    {
        std::shared_ptr<Node<int>> root = nullptr;
        REQUIRE_THROWS(tree_DFS_inorder(root));
    }

    SECTION("Self-loop")
    {
        SECTION("Self-loop on root")
        {
            const int x = 23;
            std::shared_ptr<Node<int>> root = std::make_shared<Node<int>>(23);
            root->data = 1;
            SECTION("left leaf is self")
            {
                root->left = root;
                REQUIRE_THROWS(tree_DFS_inorder(root));
            }
            SECTION("right leaf is self")
            {
                root->right = root;
                REQUIRE_THROWS(tree_DFS_inorder(root));
            }
        }
    }

    SECTION("Left chain iteration with a loop")
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
        REQUIRE(result[i]->data == i);
    }
}