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

void taylorN(Node *node, const char variable, int n)
{
    fprintf(LATEX_FILE, "\n\nTaylor of function\n\n");
    for (int i = 0; i < n; i++)
    {
        node = diff(node, variable);
        simplifyNode(node);
    }
}

Node *diff(const Node *node, const char variable)
{
    Node *return_node = nullptr;
    switch (NODE_TYPE)
    {
        case NUMBER:
            return_node = createNum(0);
            break;
        case VARIABLE:
            if (VAR_VALUE == variable)
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
    startLatexFormula(node);
    fprintf(LATEX_FILE, "' = ");
    endLatexFormula(return_node);
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
    LEFT_NODE = left_node;
    RIGHT_NODE = right_node;
    NODE_TYPE = node_type;
    VALUE = node_value;
}

Node *copyNode(Node *node)
{
    Node *new_node = (Node *) calloc(1, sizeof(new_node[0]));
    *new_node = *node;

    Node *new_left = nullptr;
    Node *new_right = nullptr;
    if (new_node->left)
        new_left = copyNode(LEFT_NODE);

    if (new_node->right)
        new_right = copyNode(RIGHT_NODE);

    new_node->left = new_left;
    new_node->right = new_right;

    return new_node;
}

void simplifyNode(Node *node)
{
    bool changed = true;
    Tree tree = {};
    treeCtor(&tree);
    tree.root = node;
    while (changed)
    {
        changed = false;
        convConst(node, &changed);
        if (changed)
            treeDump(&tree);

        changed = false;
        deleteNeutralElements(node, &changed);
        if (changed)
            treeDump(&tree);
    }
}

void convConst(Node *node, bool *changed)
{
    if (LEFT_NODE)
        convConst(LEFT_NODE, changed);
    if (RIGHT_NODE)
        convConst(RIGHT_NODE, changed);

    if (LEFT_NODE == nullptr || RIGHT_NODE == nullptr)
        return;

    if (*changed)
    {
        startLatexFormula(node);
        fprintf(LATEX_FILE, " = ");
    }
    if (IS_NUM_LEFT && IS_NUM_RIGHT)
    {
        *changed = true;
        if (IS_OP(ADD_OP))
            VAL_VALUE = LEFT_VALUE + RIGHT_VALUE;
        else if (IS_OP(SUB_OP))
            VAL_VALUE = LEFT_VALUE - RIGHT_VALUE;
        else if (IS_OP(MUL_OP))
            VAL_VALUE = LEFT_VALUE * RIGHT_VALUE;
        else if (IS_OP(DIV_OP))
            VAL_VALUE = LEFT_VALUE / RIGHT_VALUE;
        else if (IS_OP(POW_OP))
            VAL_VALUE = pow(LEFT_VALUE, RIGHT_VALUE);
        else if (IS_OP(LOG_OP))
            VAL_VALUE = log(RIGHT_VALUE) / log(RIGHT_VALUE);
        else if (IS_OP(SIN_OP))
            VAL_VALUE = sin(RIGHT_VALUE);
        else if (IS_OP(COS_OP))
            VAL_VALUE = cos(RIGHT_VALUE);
        else if (IS_OP(INCORRECT_OP))
            VAL_VALUE = NAN;

        nodeDtor(LEFT_NODE);
        nodeDtor(RIGHT_NODE);
        NODE_TYPE = NUMBER;
        LEFT_NODE = nullptr;
        RIGHT_NODE = nullptr;
    }
    if (*changed)
        endLatexFormula(node);
}

void deleteNeutralElements(Node *node, bool *changed)
{
    if (LEFT_NODE)
        deleteNeutralElements(LEFT_NODE, changed);
    if (RIGHT_NODE)
        deleteNeutralElements(RIGHT_NODE, changed);

    if (LEFT_NODE == nullptr || RIGHT_NODE == nullptr)
        return;

    if (*changed)
    {
        startLatexFormula(node);
        fprintf(LATEX_FILE, " = ");
    }
    // 1 ^ f(x), f(x) ^ 0
    if ((IS_ONE_LEFT || IS_ZERO_RIGHT) && IS_OP(POW_OP))
        changeNodeTypeToNumber(node, 1, changed);
    // 0 ^ f(x), 0 * f(x), 0 / f(x)
    else if (IS_ZERO_LEFT && (IS_OP(POW_OP) ||
                              IS_OP(MUL_OP) ||
                              IS_OP(DIV_OP)))
        changeNodeTypeToNumber(node, 0, changed);
    // f(x) ^ 1, f(x) * 1, f(x) / 1, f(x) + 0
    else if ((IS_ONE_RIGHT && (IS_OP(POW_OP) ||
                               IS_OP(MUL_OP) ||
                               IS_OP(DIV_OP)))
            || (IS_ZERO_RIGHT && IS_OP(ADD_OP)))
        moveNodeUp(node, LEFT_NODE, RIGHT_NODE, changed);
    // 0 + f(x), 1 * f(x)
    else if ((IS_ZERO_LEFT && IS_OP(ADD_OP)) ||
             (IS_ONE_LEFT && IS_OP(MUL_OP)))
        moveNodeUp(node, RIGHT_NODE, LEFT_NODE, changed);
    // 0 - f(x)
    else if (IS_ZERO_LEFT && IS_OP(SUB_OP))
    {
        LEFT_VALUE = -1;
        NODE_TYPE = OPERATION;
        OP_VALUE = SUB_OP;
        *changed = true;
    }
    // sin(number)
    else if (IS_OP(SIN_OP) && IS_NUM_RIGHT)
        changeNodeTypeToNumber(node, sin(RIGHT_VALUE), changed);
    // cos(number)
    else if (IS_OP(COS_OP) && IS_NUM_RIGHT)
        changeNodeTypeToNumber(node, cos(RIGHT_VALUE), changed);

    if (*changed)
        endLatexFormula(node);
}

void moveNodeUp(Node *node,
                Node *node_to_up,
                Node *node_to_delete,
                bool *changed)
{
    nodeDtor(node_to_delete);
    LEFT_NODE = nullptr;
    RIGHT_NODE = nullptr;
    NODE_TYPE = node_to_up->node_type;
    VALUE = node_to_up->value;
    if (node_to_up->left)
        LEFT_NODE = copyNode(node_to_up->left);
    if (node_to_up->right)
        RIGHT_NODE = copyNode(node_to_up->right);
    nodeDtor(node_to_up);
    *changed = true;
}

void changeNodeTypeToNumber(Node *node,
                            double value,
                            bool *changed)
{
    *changed = true;
    nodeDtor(LEFT_NODE);
    nodeDtor(RIGHT_NODE);
    NODE_TYPE = NUMBER;
    VAL_VALUE = value;
    LEFT_NODE = nullptr;
    RIGHT_NODE = nullptr;
}

void startLatexFormula(const Node *node)
{
    fprintf(LATEX_FILE, "\n");
    addRandomCringePhrase();

    fprintf(LATEX_FILE, "\n");
    fprintf(LATEX_FILE, "$");
    printLatexNode(node, LATEX_FILE);
}

void endLatexFormula(const Node *node)
{
    printLatexNode(node, LATEX_FILE);
    fprintf(LATEX_FILE, "$\n");
}

void getTangentEquation(Tree *tree,
                        Tree *diff_tree,
                        const char var_name,
                        double value)
{
    double f_at_value = calculateNode(tree->root, var_name, value);
    double k = calculateNode(diff_tree->root, var_name, value);
    fprintf(LATEX_FILE,
            "\n\n\\textbf{Tangent equation at %lg:}\n\n",
            value);
    if (abs(k) < EPS)
        fprintf(LATEX_FILE, "$y = %lg$\n", f_at_value);
    else
        fprintf(LATEX_FILE,
                "$y = %lg * x + %lg$\n",
                k,
                f_at_value - k * value);
}

void plotGraph(Tree *tree,
               char var_name,
               double left,
               double right,
               size_t num,
               const char *filename)
{
    FILE *fp = fopen(filename, "w");

    double step = (right - left) / num;
    double x = NAN;
    double f_at_x = NAN;
    for (size_t i = 0; i < num; i++)
    {
        x = left + step * i;
        f_at_x = calculateNode(tree->root, var_name, x);
        fprintf(fp, "%lf\t%lf\n", x, f_at_x);
    }
    fclose(fp);

    char command[BUFFER_SIZE] = "";
    sprintf(command,
            "gnuplot -e \"set terminal png size 768,480; "
            "set output 'graph.png'; plot [%lg: %lg] '%s'\"",
            left,
            right,
            filename);
    system(command);
}

double calculateNode(Node *node, const char variable, double value)
{
    double leftValue = NAN;
    double rightValue = NAN;

    if (LEFT_NODE)
        leftValue = calculateNode(LEFT_NODE, variable, value);
    if (RIGHT_NODE)
        rightValue = calculateNode(RIGHT_NODE, variable, value);

    if (NODE_TYPE == NUMBER)
        return node->value.val_value;

    if (NODE_TYPE == VARIABLE)
    {
        if (VAR_VALUE == variable)
            return value;
        else
        {
            fprintf(stderr, "Unknown variable %c\n", variable);
            return NAN;
        }
    }

    switch (OP_VALUE)
    {
        case ADD_OP:
            return leftValue + rightValue;
        case SUB_OP:
            return leftValue - rightValue;
        case MUL_OP:
            return leftValue * rightValue;
        case DIV_OP:
            return leftValue / rightValue;
        case POW_OP:
            return pow(leftValue, rightValue);
        case LOG_OP:
            return log(rightValue) / log(leftValue);
        case SIN_OP:
            return sin(rightValue);
        case COS_OP:
            return cos(rightValue);
        case INCORRECT_OP:
            fprintf(stderr, "Incorrect operation.\n");
            return NAN;
        default:
            fprintf(stderr, "Unknown operation.\n");
            return NAN;
    }
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
    if (node->node_type == NUMBER || node->node_type == VARIABLE)
        fprintf(fp, " %s ", node_value);
    else if (strcmp(node_value, "sin") == 0)
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
    fprintf(fp, "sin(");
    if (RIGHT_NODE)
        printLatexNode(RIGHT_NODE, fp);
    fprintf(fp, ")");
}

void printLatexCosNode(const Node *node, FILE *fp)
{
    fprintf(fp, "cos(");
    if (RIGHT_NODE)
        printLatexNode(RIGHT_NODE, fp);
    fprintf(fp, ")");
}

void printLatexLogNode(const Node *node, FILE *fp)
{
    fprintf(fp, "\\log_{");
    if (LEFT_NODE)
        printLatexNode(LEFT_NODE, fp);
    fprintf(fp, "}");

    fprintf(fp, "{");
    if (RIGHT_NODE)
        printLatexNode(RIGHT_NODE, fp);
    fprintf(fp, "}");
}

void printLatexDivNode(const Node *node, FILE *fp)
{
    fprintf(fp, "\\frac{");
    if (LEFT_NODE)
        printLatexNode(LEFT_NODE, fp);
    fprintf(fp, "}{");
    if (RIGHT_NODE)
        printLatexNode(RIGHT_NODE, fp);
    fprintf(fp, "}\n");
}

void printLatexPowNode(const Node *node, FILE *fp)
{
    fprintf(fp, "{");
    if (LEFT_NODE)
        printLatexNode(LEFT_NODE, fp);
    fprintf(fp, "}^{");
    if (RIGHT_NODE)
        printLatexNode(RIGHT_NODE, fp);
    fprintf(fp, "}");
}

void printLatexOrdinaryNode(const Node *node,
                            char node_value[BUFFER_SIZE],
                            FILE *fp)
{
    fprintf(fp, "(");
    if (LEFT_NODE)
        printLatexNode(LEFT_NODE, fp);

    fprintf(fp, " %s ", node_value);

    if (RIGHT_NODE)
        printLatexNode(RIGHT_NODE, fp);
    fprintf(fp, ")");
}
