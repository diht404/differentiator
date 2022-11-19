#include "differentiator.h"

int main()
{
    set_latex_file("matan.tex");
    treeClearGraphLogFile();
    treeSetLogFile();

    Tree tree = {};
    treeCtor(&tree);
    readTree(&tree, DIFFERENTIATOR_FILE);

    treeDump(&tree);

    printLatex(&tree);

    tree.root = diff(tree.root, 'x');
    treeDump(&tree);

    treeCloseLogFile();
    close_latex_file();
    return 0;
}

// TODO: calculator
// TODO: sin, cos, log
// TODO: long variables