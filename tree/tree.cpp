#include "tree.h"

size_t treeCtor(Tree *tree)
{
    CHECK_NULLPTR_ERROR(tree, TREE_IS_NULLPTR)

    tree->root = nullptr;
    tree->size = 0;
    return TREE_NO_ERRORS;
}

size_t nodeCtor(Node *node)
{
    CHECK_NULLPTR_ERROR(node, NODE_IS_NULLPTR)

    node->left = nullptr;
    node->right = nullptr;

    return TREE_NO_ERRORS;
}

size_t treeDtor(Tree *tree)
{
    CHECK_NULLPTR_ERROR(tree, TREE_IS_NULLPTR)

    nodeDtor(tree->root);
    tree->size = 0;
    return TREE_NO_ERRORS;
}

size_t nodeDtor(Node *node)
{
    CHECK_NULLPTR_ERROR(node, NODE_IS_NULLPTR)

    if (node->left)
        nodeDtor(node->left);
    if (node->right)
        nodeDtor(node->right);

    free(node);
    return TREE_NO_ERRORS;
}

size_t readTree(Tree *tree, const char *filename)
{
    CHECK_NULLPTR_ERROR(tree, TREE_IS_NULLPTR)
    CHECK_NULLPTR_ERROR(filename, STRING_IS_NULLPTR)

    FILE *fp = fopen(filename, "r");
    if (fp == nullptr)
        return CANT_OPEN_TREE_FILE;

    long lenOfFile = 0;
    char *tree_buffer = readFileToBuf(fp, &lenOfFile);
    fclose(fp);

    char *readPtr = tree_buffer;
    tree->root = readNode(&readPtr, &tree_buffer, lenOfFile);

    return TREE_NO_ERRORS;
}

Node *readNode(char **readPtr, char **buffer, long lenOfFile)
{
    NodeType node_type = INCORRECT_TYPE;
    OperationType op_type = INCORRECT_OP;
    char variable = 'a';
    double value = NAN;

    Node *node = nullptr;
    Node *left_node = nullptr;
    Node *right_node = nullptr;
    while (*readPtr < *buffer + lenOfFile)
    {
        if (**readPtr == '(')
        {
            (*readPtr)++;
            Node *new_node = readNode(readPtr, buffer, lenOfFile);
            if (left_node == nullptr)
                left_node = new_node;
            else
            {
                right_node = new_node;
                break;
            }
        }
        else if (**readPtr == ')')
        {
            (*readPtr)++;
        }
        else if (isdigit(**readPtr))
        {
            int len = 0;
            sscanf(*readPtr, "%lf %n", &value, &len);
            // len of value
            (*readPtr) += len;
            // bracket ) after value
            (*readPtr)++;
            Node *new_node = createNewNode(NUMBER,
                                           value,
                                           INCORRECT_OP,
                                           'a',
                                           nullptr,
                                           nullptr);
            return new_node;
        }
        else
        {
            switch (**readPtr)
            {
                case '+':
                    node_type = OPERATION;
                    op_type = ADD_OP;
                    (*readPtr)++;
                    break;
                case '-':
                    node_type = OPERATION;
                    op_type = SUB_OP;
                    (*readPtr)++;
                    break;
                case '*':
                    node_type = OPERATION;
                    op_type = MUL_OP;
                    (*readPtr)++;
                    break;
                case '/':
                    node_type = OPERATION;
                    op_type = DIV_OP;
                    (*readPtr)++;
                    break;
                case '^':
                    node_type = OPERATION;
                    op_type = POW_OP;
                    (*readPtr)++;
                    break;
                case 'l':
                    node_type = OPERATION;
                    op_type = LOG_OP;
                    (*readPtr)++;
                    break;
                case 's':
                    node_type = OPERATION;
                    op_type = SIN_OP;
                    (*readPtr)++;
                    break;
                case 'c':
                    node_type = OPERATION;
                    op_type = COS_OP;
                    (*readPtr)++;
                    break;
                default:
                    variable = **readPtr;
                    (*readPtr)++;
                    // bracket ) after variable
                    (*readPtr)++;
                    Node *new_node = createNewNode(VARIABLE,
                                                   NAN,
                                                   INCORRECT_OP,
                                                   variable,
                                                   nullptr,
                                                   nullptr);
//                    fprintf(stderr, "NODE_TYPE = %d\n", new_node->node_type);
                    return new_node;
            }
        }
    }
    return createNewNode(OPERATION,
                         NAN,
                         op_type,
                         'a',
                         left_node,
                         right_node);
}

Node *createNewNode(NodeType node_type,
                    double value,
                    OperationType op_type,
                    char variable,
                    Node *left_node,
                    Node *right_node)
{
    Node *node = (Node *) calloc(1, sizeof(node[0]));
    node->left = left_node;
    node->right = right_node;
    node->node_type = node_type;
    if (node_type == OPERATION)
    {
        node->op_value = op_type;
    }
    else if (node_type == VARIABLE)
    {
        node->var_value = variable;
    }
    else if (node_type == NUMBER)
    {
        node->val_value = value;
    }
    else
    {
        fprintf(stderr, "Incorrect node type %d\n", node_type);
    }
    return node;
}
