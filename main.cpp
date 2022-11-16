#include "differentiator.h"

int main()
{
    treeClearGraphLogFile();
    treeSetLogFile();

    Tree tree = {};
    treeCtor(&tree);
    readTree(&tree, DIFFERENTIATOR_FILE);

    treeDump(&tree);


    tree.root = diff(tree.root);
    treeDump(&tree);

    printLatex(&tree);
    treeCloseLogFile();
    return 0;
}