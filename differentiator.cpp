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

void taylorN(Node *node, const char variable, double x, int n)
{
    fprintf(LATEX_FILE, "\n\n\\textbf{Taylor of function}\n\n");

    double *taylor_coefficients =
        (double *) calloc((size_t) (n + 1),
                          sizeof(taylor_coefficients[0]));
    taylor_coefficients[0] = oneVariableCalculateNode(node, variable, x);

    Node *diff_node = node;
    for (int i = 1; i < n + 1; i++)
    {
        Node *new_node = diff(diff_node, variable);
        if (i > 1)
            nodeDtor(diff_node);
        diff_node = new_node;
        simplifyNode(diff_node);
        taylor_coefficients[i] =
            oneVariableCalculateNode(diff_node, variable, x);
    }
    printTaylorN(node, taylor_coefficients, variable, x, n);
    nodeDtor(diff_node);
    free(taylor_coefficients);
}

void printTaylorN(Node *node,
                  double *taylor_coefficients,
                  const char variable,
                  double x,
                  int n)
{
    fprintf(LATEX_FILE, "\n\n\\textbf{Answer:}\n\n$");
    printLatexNode(node, LATEX_FILE);
    fprintf(LATEX_FILE, " = ");

    if (abs(taylor_coefficients[0]) > EPS)
        fprintf(LATEX_FILE, "%lg", taylor_coefficients[0]);
    for (int i = 1; i < n + 1; i++)
    {
        if (abs(taylor_coefficients[i]) < EPS)
            continue;
        if (i == 1)
            if (abs(x) > EPS)
                fprintf(LATEX_FILE,
                        "%c %lg (%c - %lg)",
                        taylor_coefficients[i] > 0 ? '+' : ' ',
                        taylor_coefficients[i],
                        variable,
                        x);
            else
                fprintf(LATEX_FILE,
                        "%c %lg %c",
                        taylor_coefficients[i] > 0 ? '+' : ' ',
                        taylor_coefficients[i],
                        variable);
        else if (abs(x) > EPS)
            fprintf(LATEX_FILE,
                    "%c %lg \\frac{(%c - %lg)^{%d}}{%d!}",
                    taylor_coefficients[i] > 0 ? '+' : ' ',
                    taylor_coefficients[i],
                    variable,
                    x,
                    i,
                    i);
        else
            fprintf(LATEX_FILE,
                    "%c %lg \\frac{%c^{%d}}{%d!}",
                    taylor_coefficients[i] > 0 ? '+' : ' ',
                    taylor_coefficients[i],
                    variable,
                    i,
                    i);

    }
    if (abs(x) > EPS)
        fprintf(LATEX_FILE,
                "+ o((%c - %lg)^%d)$\n\n",
                variable,
                x,
                n + 1);
    else
        fprintf(LATEX_FILE, "+ o(%c^%d)$\n\n", variable, n + 1);
}

void diffXYZ(Tree *tree, double x, double y, double z)
{
    Tree diff_x_tree = {};
    treeCtor(&diff_x_tree);
    diff_x_tree.root = diff(tree->root, 'x');
    treeDump(&diff_x_tree);
    simplifyNode(diff_x_tree.root);

    Tree diff_y_tree = {};
    treeCtor(&diff_y_tree);
    diff_y_tree.root = diff(tree->root, 'y');
    treeDump(&diff_y_tree);
    simplifyNode(diff_y_tree.root);

    Tree diff_z_tree = {};
    treeCtor(&diff_z_tree);
    diff_z_tree.root = diff(tree->root, 'z');
    treeDump(&diff_z_tree);
    simplifyNode(diff_z_tree.root);

    fprintf(LATEX_FILE, "\n\n\\textbf{Answer:} \n\n$ ");
    fprintf(LATEX_FILE, "f'_x = ");
    printLatexNode(diff_x_tree.root, LATEX_FILE);
    fprintf(LATEX_FILE, "$\n\n$f'_y = ");
    printLatexNode(diff_y_tree.root, LATEX_FILE);
    fprintf(LATEX_FILE, "$\n\n$f'_z = ");
    printLatexNode(diff_z_tree.root, LATEX_FILE);
    fprintf(LATEX_FILE, "$\n\n");

    double dfdx = calculateNode(diff_x_tree.root, x, y, z);
    double dfdy = calculateNode(diff_y_tree.root, x, y, z);
    double dfdz = calculateNode(diff_z_tree.root, x, y, z);

    fprintf(LATEX_FILE,
            "$f'(%lg, %lg, %lg) = (%lg, %lg, %lg)$\n\n",
            x, y, z, dfdx, dfdy, dfdz);
    fprintf(LATEX_FILE, "$|f'(%lg, %lg, %lg)| = %lg$\n\n",
            x, y, z, sqrt(x * x + y * y + z * z));
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
    //    fprintf(LATEX_FILE, "(");
    startLatexFormula(node, true);
    fprintf(LATEX_FILE, ")'_{%c} = ", variable);
    endLatexFormula(return_node);
    return return_node;
}

Node *diffOperation(const Node *node, const char variable)
{
    switch (OP_VALUE)
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
            return diffPow(node, variable);
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
    if (IS_LEFT_TYPE(NUMBER) &&
        IS_RIGHT_TYPE(NUMBER))
        return createNum(0);
        // log_a(f(x))
    else if (IS_LEFT_TYPE(NUMBER) &&
        !IS_RIGHT_TYPE(NUMBER))
        return DIV(dR, MUL(LOG(createNum(EXP), cL), cR));
        // log_f(x)(g(x))
    else
        return DIV(SUB(DIV(MUL(LOG(createNum(EXP), cL), dR), cR),
                       DIV(MUL(LOG(createNum(EXP), cR), dL), cL)),
                   MUL(LOG(createNum(EXP), cL),
                       LOG(createNum(EXP), cL)));
}

Node *diffPow(const Node *node, const char variable)
{
    // a^b
    if (IS_LEFT_TYPE(NUMBER) &&
        IS_RIGHT_TYPE(NUMBER))
        return createNum(0);
        // f(x)^1
    else if (!IS_LEFT_TYPE(NUMBER) &&
              IS_RIGHT_TYPE(NUMBER) &&
              abs(RIGHT_VALUE - 1) < EPS)
        return dL;
        // f(x)^a, a != 1
    else if (!IS_LEFT_TYPE(NUMBER) &&
              IS_RIGHT_TYPE(NUMBER) &&
              abs(RIGHT_VALUE - 1) >= EPS)
        return MUL(MUL(cR, dL),
                   POW(cL, SUB(cR, createNum(1))));
        // a^f(x)
    else if (IS_LEFT_TYPE(NUMBER) &&
             IS_RIGHT_TYPE(NUMBER))
        return MUL(MUL(LOG(createNum(EXP), cL), dR),
                   POW(cL, cR));
        // f(x)^g(x)
    else
        return MUL(POW(cL, SUB(cR, createNum(1))),
                   ADD(MUL(cR, dL),
                       MUL(MUL(cL, dR),
                           LOG(createNum(EXP), cL))));
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
    fprintf(LATEX_FILE, "\n\n\\textbf{Answer:}\n\n$");
    printLatexNode(node, LATEX_FILE);
    fprintf(LATEX_FILE, "$\n");
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
        startLatexFormula(node, false);
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
        startLatexFormula(node, false);
        fprintf(LATEX_FILE, " = ");
    }
    // 1 ^ f(x), f(x) ^ 0
    if ((IS_ONE_LEFT || IS_ZERO_RIGHT) && IS_OP(POW_OP))
        changeNodeTypeToNumber(node, 1, changed);
    // 0 ^ f(x), 0 * f(x), 0 / f(x), f(x) * 0
    else if ((IS_ZERO_LEFT  && (IS_OP(POW_OP) ||
                                IS_OP(MUL_OP) ||
                                IS_OP(DIV_OP))) ||
             (IS_ZERO_RIGHT && IS_OP(MUL_OP)))
        changeNodeTypeToNumber(node, 0, changed);
    // f(x) ^ 1, f(x) * 1, f(x) / 1, f(x) + 0, f(x) - 0
    else if ((IS_ONE_RIGHT  && (IS_OP(POW_OP) ||
                                IS_OP(MUL_OP) ||
                                IS_OP(DIV_OP))) ||
             (IS_ZERO_RIGHT && (IS_OP(ADD_OP)||
                                IS_OP(SUB_OP))))
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
    nodeDtor(LEFT_NODE);
    nodeDtor(RIGHT_NODE);

    NODE_TYPE = NUMBER;
    VAL_VALUE = value;
    LEFT_NODE = nullptr;
    RIGHT_NODE = nullptr;

    *changed = true;
}

void startLatexFormula(const Node *node, bool printStartBracket)
{
    fprintf(LATEX_FILE, "\n");
    addRandomCringePhrase();

    fprintf(LATEX_FILE, "\n$");
    if (printStartBracket)
        fprintf(LATEX_FILE, "(");
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
    double f_at_value =
        oneVariableCalculateNode(tree->root, var_name, value);
    double k =
        oneVariableCalculateNode(diff_tree->root, var_name, value);
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
        f_at_x = oneVariableCalculateNode(tree->root, var_name, x);
        fprintf(fp, "%lf\t%lf\n", x, f_at_x);
    }
    fclose(fp);

    char command[BUFFER_SIZE] = "";
    sprintf(command,
            "gnuplot -e \"set terminal png size 480,360; "
            "set output 'graph.png'; plot [%lg: %lg] '%s'\"",
            left,
            right,
            filename);
    system(command);
    fprintf(LATEX_FILE, "\n\n\\includegraphics{./graph.png} \n\n");
}

double calculateNode(Node *node, double x, double y, double z)
{
    double leftValue = NAN;
    double rightValue = NAN;

    if (LEFT_NODE)
        leftValue = calculateNode(LEFT_NODE, x, y, z);
    if (RIGHT_NODE)
        rightValue = calculateNode(RIGHT_NODE, x, y, z);

    if (NODE_TYPE == NUMBER)
        return node->value.val_value;

    if (NODE_TYPE == VARIABLE)
    {
        if (VAR_VALUE == 'x')
            return x;
        else if (VAR_VALUE == 'y')
            return y;
        else if (VAR_VALUE == 'z')
            return z;
        else
        {
            fprintf(stderr, "Unknown variable %c\n", VAR_VALUE);
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
    fprintf(LATEX_FILE, "$\n");
}

void printLatexNode(const Node *node, FILE *fp)
{
    char node_value[BUFFER_SIZE] = "";
    getValueOfNode(node, &node_value);
    if (NODE_TYPE == NUMBER || NODE_TYPE == VARIABLE)
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
    fprintf(fp, "sin");
    if (RIGHT_NODE)
        if (printLatexRequireBrackets(RIGHT_NODE))
            printLatexNode(RIGHT_NODE, fp);
        else
        {
            fprintf(fp, "(");
            printLatexNode(RIGHT_NODE, fp);
            fprintf(fp, ")");
        }
}

void printLatexCosNode(const Node *node, FILE *fp)
{
    fprintf(fp, "cos");
    if (RIGHT_NODE)
        if (printLatexRequireBrackets(RIGHT_NODE))
            printLatexNode(RIGHT_NODE, fp);
        else
        {
            fprintf(fp, "(");
            printLatexNode(RIGHT_NODE, fp);
            fprintf(fp, ")");
        }
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
    bool require_left_bracket = IS_LEFT_TYPE(OPERATION) &&
        getPriority(node) > getPriority(LEFT_NODE);
    if (require_left_bracket)
        fprintf(fp, "(");
    if (LEFT_NODE)
        printLatexNode(LEFT_NODE, fp);
    if (require_left_bracket)
        fprintf(fp, ")");

    if (strcmp(node_value, "*") == 0)
    {
        if (RIGHT_NODE->left)
        {
            if (IS_LEFT_TYPE(NUMBER)
                && RIGHT_NODE->left->node_type == VARIABLE);
            else
                fprintf(fp, " \\cdot ");
        }
        else
        {
            if (IS_LEFT_TYPE(NUMBER) && !IS_RIGHT_TYPE(NUMBER));
            else
                fprintf(fp, " \\cdot ");
        }
    }
    else
        fprintf(fp, " %s ", node_value);

    bool require_right_bracket = IS_RIGHT_TYPE(OPERATION) &&
        getPriority(node) > getPriority(RIGHT_NODE);
    if (require_right_bracket)
        fprintf(fp, "(");
    if (RIGHT_NODE)
        printLatexNode(RIGHT_NODE, fp);
    if (require_right_bracket)
        fprintf(fp, ")");
}

bool printLatexRequireBrackets(Node *node)
{
    return (!(node->node_type == OPERATION)) ||
        ((!(node->left->node_type == OPERATION)) &&
            (!(node->right->node_type == OPERATION)));
}

int getPriority(const Node *node)
{
    if (IS_OP(ADD_OP) || IS_OP(SUB_OP))
        return 0;
    if (IS_OP(MUL_OP) || IS_OP(DIV_OP))
        return 1;
    if (IS_OP(LOG_OP) || IS_OP(SIN_OP) || IS_OP(COS_OP))
        return 2;
    if (IS_OP(POW_OP))
        return 3;
    return -1;
}

double oneVariableCalculateNode(Node *node,
                                const char variable,
                                double value)
{
    if (variable == 'x')
        return calculateNode(node, value, 0, 0);
    else if (variable == 'y')
        return calculateNode(node, 0, value, 0);
    else if (variable == 'z')
        return calculateNode(node, 0, 0, value);
}
