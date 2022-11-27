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

    simplifyNode(diff_tree.root);

    double x = 0;
    char var_name = 'x';
    printf("f'(%lg) = %lg\n",
           x,
           oneVariableCalculateNode(diff_tree.root, var_name, x));

    getTangentEquation(&tree, &diff_tree, var_name, x);
    plotGraph(&tree, 'x', 0.1, 5, 10000, "graph.txt");

    int n = 3;
    taylorN(tree.root, 'x', 0, n);
//    diffXYZ(&tree, 3, 1, 2);
    treeDtor(&diff_tree);
    treeDtor(&tree);
    treeCloseLogFile();
    close_latex_file();
    return 0;
}

// TODO: replace expressions
