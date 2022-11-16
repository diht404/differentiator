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

//size_t treeSaveToFile(Tree *tree, FILE *fp)
//{
//    CHECK_NULLPTR_ERROR(tree, TREE_IS_NULLPTR)
//
//    return nodePreOrderPrint(tree->root, fp, 0);
//}
//
//size_t nodePreOrderPrint(Node *node, FILE *fp, size_t num_spaces)
//{
//    CHECK_NULLPTR_ERROR(node, NODE_IS_NULLPTR)
//    CHECK_NULLPTR_ERROR(fp, FILE_IS_NULLPTR)
//
//    size_t error = TREE_NO_ERRORS;
//
//    for (size_t i = 0; i < num_spaces; i++)
//    {
//        fprintf(fp, " ");
//    }
//
//    if (node->left == nullptr and node->right == nullptr)
//    {
//        fprintf(fp, "{ %s }\n", node->value);
//        return TREE_NO_ERRORS;
//    }
//    else
//        fprintf(fp, "{ %s \n", node->value);
//
//    if (node->left)
//        error = nodePreOrderPrint(node->left,
//                                  fp,
//                                  num_spaces + LOGS_NUM_SPACES);
//    if (error)
//        return error;
//
//    if (node->right)
//        error = nodePreOrderPrint(node->right,
//                                  fp,
//                                  num_spaces + LOGS_NUM_SPACES);
//    if (error)
//        return error;
//
//    for (size_t i = 0; i < num_spaces; i++)
//    {
//        fprintf(fp, " ");
//    }
//    fprintf(fp, "}\n");
//
//    return TREE_NO_ERRORS;
//}
//
//size_t nodeInOrderPrint(Node *node, FILE *fp, size_t num_spaces)
//{
//    CHECK_NULLPTR_ERROR(node, NODE_IS_NULLPTR)
//    CHECK_NULLPTR_ERROR(fp, FILE_IS_NULLPTR)
//
//    size_t error = TREE_NO_ERRORS;
//
//    if (node->left)
//        error = nodeInOrderPrint(node->left,
//                                 fp,
//                                 num_spaces);
//    if (error)
//        return error;
//
//    fprintf(fp, "%s\n", node->value);
//
//    if (node->right)
//        error = nodeInOrderPrint(node->right,
//                                 fp,
//                                 num_spaces);
//    if (error)
//        return error;
//
//    return TREE_NO_ERRORS;
//}
//
//size_t nodePostOrderPrint(Node *node, FILE *fp, size_t num_spaces)
//{
//    CHECK_NULLPTR_ERROR(node, NODE_IS_NULLPTR)
//    CHECK_NULLPTR_ERROR(fp, FILE_IS_NULLPTR)
//
//    size_t error = TREE_NO_ERRORS;
//
//    if (node->left)
//        error = nodePostOrderPrint(node->left,
//                                   fp,
//                                   num_spaces);
//    if (error)
//        return error;
//
//    if (node->right)
//        error = nodePostOrderPrint(node->right,
//                                   fp,
//                                   num_spaces);
//    if (error)
//        return error;
//
//    fprintf(fp, "%s \n", node->value);
//
//    return TREE_NO_ERRORS;
//}

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
                    op_type = PLUS_OP;
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
    return createNewNode(OPERATION, NAN, op_type, 'a', left_node, right_node);
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


//size_t createNode(Node **new_node,
//                  Tree *tree,
//                  Node *node,
//                  char **startTokenPtr,
//                  char **endTokenPtr)
//{
//    CHECK_NULLPTR_ERROR(new_node, NODE_IS_NULLPTR)
//    CHECK_NULLPTR_ERROR(tree, TREE_IS_NULLPTR)
//    CHECK_NULLPTR_ERROR(node, NODE_IS_NULLPTR)
//    CHECK_NULLPTR_ERROR(startTokenPtr, STRING_IS_NULLPTR)
//    CHECK_NULLPTR_ERROR(*startTokenPtr, STRING_IS_NULLPTR)
//    CHECK_NULLPTR_ERROR(endTokenPtr, STRING_IS_NULLPTR)
//    CHECK_NULLPTR_ERROR(*endTokenPtr, STRING_IS_NULLPTR)
//
//    size_t error = TREE_NO_ERRORS;
//    if (node == tree->root and tree->size == 0)
//        *new_node = tree->root;
//    else
//        *new_node = (Node *) calloc(1, sizeof(Node));
//    nodeCtor(*new_node);
//
//    if (tree->root->value != nullptr)
//        error = createNoRootNode(node, *new_node);
//    if  (error) return error;
//
//    error = setNodeValue(tree, *new_node, startTokenPtr, endTokenPtr);
//    tree->size++;
//
//    return error;
//}
//
//size_t setNodeValue(Tree *tree,
//                    Node *new_node,
//                    char **startTokenPtr,
//                    char **endTokenPtr)
//{
//    CHECK_NULLPTR_ERROR(tree, TREE_IS_NULLPTR)
//    CHECK_NULLPTR_ERROR(new_node, NODE_IS_NULLPTR)
//    CHECK_NULLPTR_ERROR(startTokenPtr, STRING_IS_NULLPTR)
//    CHECK_NULLPTR_ERROR(*startTokenPtr, STRING_IS_NULLPTR)
//    CHECK_NULLPTR_ERROR(endTokenPtr, STRING_IS_NULLPTR)
//    CHECK_NULLPTR_ERROR(*endTokenPtr, STRING_IS_NULLPTR)
//
//    if (**startTokenPtr == '\"')
//        (*startTokenPtr)++;
//    if (**endTokenPtr == '\"')
//        (*endTokenPtr)--;
//
//    new_node->value =
//        (char *) calloc(*endTokenPtr - *startTokenPtr + 1,
//                        sizeof(new_node->value[0]));
//    CHECK_NULLPTR_ERROR(new_node->value,
//                        TREE_CANT_ALLOCATE_MEMORY)
//    memcpy(new_node->value,
//           *startTokenPtr,
//           *endTokenPtr - *startTokenPtr + 1);
//    return TREE_NO_ERRORS;
//}
//
//size_t createNoRootNode(Node *node, Node *new_node)
//{
//    CHECK_NULLPTR_ERROR(node, NODE_IS_NULLPTR)
//    CHECK_NULLPTR_ERROR(new_node, NODE_IS_NULLPTR)
//
//    if (node->left == nullptr)
//        node->left = new_node;
//    else if (node->right == nullptr)
//        node->right = new_node;
//    else
//        fprintf(stderr,
//                "Both is not NULLPTR\n");
//}
//
//size_t insertNode(Node *node,
//                  char *value,
//                  size_t value_size,
//                  char *delimiter,
//                  size_t delimiter_size)
//{
//    CHECK_NULLPTR_ERROR(node, NODE_IS_NULLPTR)
//    CHECK_NULLPTR_ERROR(value, STRING_IS_NULLPTR)
//    CHECK_NULLPTR_ERROR(delimiter, STRING_IS_NULLPTR)
//
//    node->left = (Node *) calloc(1, sizeof(Node));
//    CHECK_NULLPTR_ERROR(node->left, TREE_CANT_ALLOCATE_MEMORY)
//    nodeCtor(node->left);
//
//    node->right = (Node *) calloc(1, sizeof(Node));
//    CHECK_NULLPTR_ERROR(node->right, TREE_CANT_ALLOCATE_MEMORY)
//    nodeCtor(node->right);
//
//    node->right->value = node->value;
//
//    node->left->value = (char *) calloc(1, value_size);
//    memcpy(node->left->value, value, value_size);
//    node->value = (char *) calloc(1, delimiter_size);
//    memcpy(node->value, delimiter, delimiter_size);
//
//    return TREE_NO_ERRORS;
//}
