#include "differentiator.h"

char LATEX_FILENAME[BUFFER_SIZE] = "";
FILE *LATEX_FILE = stderr;

size_t set_latex_file(const char *filename)
{
    memcpy(LATEX_FILENAME, filename, 1024);
    LATEX_FILE = fopen(filename, "w");
    srand(time(NULL));
    if (LATEX_FILE == nullptr)
    {
        LATEX_FILE = stderr;
        return CANT_OPEN_FILE;
    }
    fprintf(LATEX_FILE, "%s\n", LATEX_START_DOCUMENT);
    return NO_ERRORS;
}

size_t close_latex_file()
{
    fprintf(LATEX_FILE, "\n%s\n", LATEX_END_DOCUMENT);
    fclose(LATEX_FILE);

    char command[BUFFER_SIZE] = "";
    sprintf(command,
            "pdflatex -interaction batchmode %s",
            LATEX_FILENAME);
    system(command);
    return NO_ERRORS;
}

size_t addRandomCringePhrase()
{
    size_t len = sizeof(CRINGE_PHRASES) / sizeof(CRINGE_PHRASES[0]);
    fprintf(LATEX_FILE, "%s", CRINGE_PHRASES[rand() % len]);
}

Node *diff(const Node *node, const char variable)
{
    Node *return_node = nullptr;
    switch (node->node_type)
    {
        case NUMBER:
            return_node = createNum(0);
            break;
        case VARIABLE:
            if (node->value.var_value == variable)
                return_node = createNum(1);
            else
                return_node = createNum(0);
            break;
        case OPERATION:
            return_node = diffOperation(node, variable);
            break;
        case INCORRECT_TYPE:
            fprintf(stderr, "Incorrect node type.\n");
            break;
        default:
            fprintf(stderr, "Unknown node type.\n");
            break;
    }
    fprintf(LATEX_FILE, "\n");
    addRandomCringePhrase();

    fprintf(LATEX_FILE, "\n");
    fprintf(LATEX_FILE, "$");
    printLatexNode(node, LATEX_FILE);
    fprintf(LATEX_FILE, "' = ");
    printLatexNode(return_node, LATEX_FILE);
    fprintf(LATEX_FILE, "$\n");
    return return_node;
}

Node *diffOperation(const Node *node, const char variable)
{
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
            return diffPop(node, variable);
        case LOG_OP:
            return diffLog(node, variable);
        case SIN_OP:
            return MUL(COS(createNum(0), cR), dR);
        case COS_OP:
            return MUL(SUB(createNum(-1),
                           SIN(createNum(0), cR)), dR);
        case INCORRECT_OP:
            fprintf(stderr, "Incorrect operation.\n");
            return nullptr;
        default:
            fprintf(stderr, "Unknown operation.\n");
            return nullptr;
    }
}

Node *diffLog(const Node *node, const char variable)
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

Node *diffPop(const Node *node, const char variable)
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

    Node *node = createNode(NUMBER,
                            {.val_value = value},
                            nullptr,
                            nullptr);
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
    fprintf(LATEX_FILE, "$");
    printLatexNode(tree->root, LATEX_FILE);
    fprintf(LATEX_FILE, "'$\n");
}

void printLatexNode(const Node *node, FILE *fp)
{
    char node_value[BUFFER_SIZE] = "";
    getValueOfNode(node, &node_value);
    if (strcmp(node_value, "sin") == 0)
        printLatexSinNode(node, fp);
    else if (strcmp(node_value, "cos") == 0)
        printLatexCosNode(node, fp);
    else if (strcmp(node_value, "log") == 0)
        printLatexLogNode(node, fp);
    else if (strcmp(node_value, "/") == 0)
        printLatexDivNode(node, fp);
    else if (strcmp(node_value, "^") == 0)
        printLatexPowNode(node, fp);
    else
        printLatexOrdinaryNode(node, node_value, fp);
}

void printLatexSinNode(const Node *node, FILE *fp)
{
    fprintf(fp, "sin");
    if (node->right)
        printLatexNode(node->right, fp);
}

void printLatexCosNode(const Node *node, FILE *fp)
{
    fprintf(fp, "cos");
    if (node->right)
        printLatexNode(node->right, fp);
}

void printLatexLogNode(const Node *node, FILE *fp)
{
    fprintf(fp, "\\log_{");
    if (node->left)
        printLatexNode(node->left, fp);
    fprintf(fp, "}");

    fprintf(fp, "{");
    if (node->right)
        printLatexNode(node->right, fp);
    fprintf(fp, "}");
}

void printLatexDivNode(const Node *node, FILE *fp)
{
    fprintf(fp, "\\frac{");
    if (node->left)
        printLatexNode(node->left, fp);
    fprintf(fp, "}{");
    if (node->right)
        printLatexNode(node->right, fp);
    fprintf(fp, "}\n");
}

void printLatexPowNode(const Node *node, FILE *fp)
{
    fprintf(fp, "{");
    if (node->left)
        printLatexNode(node->left, fp);
    fprintf(fp, "}^{");
    if (node->right)
        printLatexNode(node->right, fp);
    fprintf(fp, "}");
}

void printLatexOrdinaryNode(const Node *node,
                            char node_value[BUFFER_SIZE],
                            FILE *fp)
{
    fprintf(fp, "(");
    if (node->left)
        printLatexNode(node->left, fp);

    fprintf(fp, " %s ", node_value);

    if (node->right)
        printLatexNode(node->right, fp);
    fprintf(fp, ")");
}