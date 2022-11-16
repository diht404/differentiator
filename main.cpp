#include "differentiator.h"

int main()
{
    treeClearGraphLogFile();
    treeSetLogFile();

    Tree tree = {};
    treeCtor(&tree);
    readTree(&tree, DIFFERENTIATOR_FILE);

    treeDump(&tree);
    printLatex(&tree);
    treeCloseLogFile();
    return 0;
}