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

    Tree diff_tree = {};
    treeCtor(&diff_tree);
    diff_tree.root = diff(tree.root, 'x');
    treeDump(&diff_tree);
    double x = 1.2;
    char var_name = 'x';
    printf("f'(%lg) = %lg\n",
           x,
           calculateNode(diff_tree.root, var_name, x));

    treeDtor(&tree);
    treeDtor(&diff_tree);
    treeCloseLogFile();
    close_latex_file();
    return 0;
}

// TODO: sin, cos, log
// TODO: long variables