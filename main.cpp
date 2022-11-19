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
    double x = 1.2;
    char var_name = 'x';
    printf("f(%lg) = %lg\n",
           x,
           calculateNode(tree.root, var_name, x));

    treeCloseLogFile();
    close_latex_file();
    return 0;
}

// TODO: calculator
// TODO: sin, cos, log
// TODO: long variables