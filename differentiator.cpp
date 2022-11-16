#include "differentiator.h"

void printLatex(Tree *tree)
{
    FILE *fp = fopen("matan.tex", "w");
    fprintf(fp, "%s\n$$", LATEX_START_DOCUMENT);

    printLatexNode(tree->root, fp);

    fprintf(fp, "$$\n%s\n", LATEX_END_DOCUMENT);
    fclose(fp);
    system("rm matan.pdf");
    system("pdflatex -interaction batchmode matan.tex");
    system("rm matan.aux matan.log");
}

void printLatexNode(Node *node, FILE *fp)
{
    char node_value[BUFFER_SIZE] = "";
    getValueOfNode(node, &node_value);

    if (strcmp(node_value, "^") == 0)
        fprintf(fp, "{");
    else
        fprintf(fp, "(");
    if (node->left)
        printLatexNode(node->left, fp);
    if (strcmp(node_value, "^") == 0)
        fprintf(fp, "}");

    fprintf(fp, "%s", node_value);

    if (strcmp(node_value, "^") == 0)
        fprintf(fp, "{");

    if (node->right)
        printLatexNode(node->right, fp);
    if (strcmp(node_value, "^") == 0)
        fprintf(fp, "}");
    else
        fprintf(fp, ")");
}