#include "differentiator.h"

Node *diff(const Node *node)
{
    switch (node->node_type)
    {
        case NUMBER:
            return createNum(0);
        case VARIABLE:
            return createNum(1);
        case OPERATION:
            switch (node->value.op_value)
            {
                case ADD_OP:
                    return ADD(dL, dR);
                case SUB_OP:
                    return SUB(dL, dR);
                case MUL_OP:
                    return ADD(MUL(dL, cR),
                               MUL(cL, dR));
                case DIV_OP:
                    return DIV(SUB(MUL(dL, cR),
                                   MUL(cL, dR)),
                               MUL(cR, cR));
                case POW_OP:
                    return diffPop(node);
                case LOG_OP:
                    return diffLog(node);
                case SIN_OP:
                    return MUL(COS(createNum(0), cR), dR);
                case COS_OP:
                    return MUL(SUB(createNum(-1),
                                   SIN(createNum(0), cR)), dR);
                case INCORRECT_OP:
                    fprintf(stderr, "Incorrect operation.");
                    return nullptr;
                default:
                    fprintf(stderr, "Unknown operation.");
                    return nullptr;
            }
        case INCORRECT_TYPE:
            fprintf(stderr, "Incorrect node type.");
            return nullptr;
        default:
            fprintf(stderr, "Unknown node type.");
            return nullptr;
    }
}

Node *diffLog(const Node *node)
{
    // log_a(b)
    if (node->left->node_type == NUMBER &&
        node->right->node_type == NUMBER)
        return createNum(0);
        // log_a(f(x))
    else if (node->left->node_type == NUMBER &&
        node->right->node_type != NUMBER)
        return DIV(dR, MUL(LOG(createNum(EXP), cL), cR));
        // log_f(x)(g(x))
    else
        return DIV(SUB(DIV(MUL(LOG(createNum(EXP), cL), dR), cR),
                       DIV(MUL(LOG(createNum(EXP), cR), dL), cL)),
                   MUL(LOG(createNum(EXP), cL),
                       LOG(createNum(EXP), cL)));
}

Node *diffPop(const Node *node)
{
    // a^b
    if (node->left->node_type == NUMBER &&
        node->right->node_type == NUMBER)
        return createNum(0);
        // f(x)^1
    else if (node->left->node_type != NUMBER &&
        node->right->node_type == NUMBER &&
        abs(node->right->value.val_value - 1) < EPS)
        return dL;
        // f(x)^a, a != 1
    else if (node->left->node_type != NUMBER &&
        node->right->node_type == NUMBER &&
        abs(node->right->value.val_value - 1) >= EPS)
        return MUL(MUL(cR, dL),
                   POW(cL, SUB(cR, createNum(1))));
        // a^f(x)
    else if (node->left->node_type == NUMBER &&
        node->right->node_type != NUMBER)
        return MUL(MUL(LOG(createNum(EXP), cL), dR),
                   POW(cL, cR));
        // f(x)^g(x)
    else
        return MUL(POW(cL, SUB(cR, createNum(1))),
                   ADD(MUL(cR, dL),
                       MUL(MUL(cL, dR),
                           LOG(createNum(EXP), cL))));
}

Node *createNum(double value)
{
    Node *node = (Node *) calloc(1, sizeof(node[0]));
    node->left = nullptr;
    node->right = nullptr;
    node->node_type = NUMBER;
    node->value.val_value = value;

    return node;
}

Node *createNode(NodeType node_type,
                 NodeValue node_value,
                 Node *left_node,
                 Node *right_node)
{
    Node *node = (Node *) calloc(1, sizeof(node[0]));
    node->left = left_node;
    node->right = right_node;
    node->node_type = node_type;
    node->value = node_value;
}

Node *copyNode(Node *node)
{
    Node *new_node = (Node *) calloc(1, sizeof(new_node[0]));
    *new_node = *node;

    Node *new_left = nullptr;
    Node *new_right = nullptr;
    if (new_node->left)
        new_left = copyNode(node->left);

    if (new_node->right)
        new_right = copyNode(node->right);

    new_node->left = new_left;
    new_node->right = new_right;

    return new_node;
}

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