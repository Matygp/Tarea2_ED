#ifndef GENERAL_TREE_H
#define GENERAL_TREE_H

#include <string>
#include <vector>

struct SimilarBook {
    std::string title;
    std::string isbn;
    int publicationYear;
};

struct BookData {
    std::string id;
    std::string title;
    std::string isbn;
    int publicationYear;
    std::string language;
    std::string description;
    double averageRating;
    int numPages;
    std::vector<SimilarBook> similarBooks;
};

class TreeNode {
public:
    BookData data;
    std::vector<TreeNode*> children;
    TreeNode(BookData data);
    ~TreeNode();
};

class GeneralTree {
private:
    TreeNode* root;
    void preOrderList(TreeNode* node);
    TreeNode* pruneRatings(TreeNode* node, double r);
    void findPrecursors(TreeNode* node);

public:
    GeneralTree();
    ~GeneralTree();
    void listar();
    void borrar_ratings(double r);
    void precursores();
    void setRoot(TreeNode* newRoot);
};

#endif // GENERAL_TREE_H