#ifndef DIFFERENTIATOR__TREE_H
#define DIFFERENTIATOR__TREE_H

#include "stdio.h"
#include "stdlib.h"
#include "assert.h"
#include "config.h"
#include "../utils.h"
#include "cstring"
#include <cmath>
#include <cctype>

const size_t BUFFER_SIZE = 1024;
const double EXP = exp(1);
const double EPS = 1e-5;

enum NodeType
{
    INCORRECT_TYPE = -1,
    OPERATION = 0,
    VARIABLE = 1,
    NUMBER = 2,
};

enum OperationType
{
    INCORRECT_OP = -1,
    ADD_OP = 0,
    SUB_OP = 1,
    MUL_OP = 2,
    DIV_OP = 3,
    POW_OP = 4,
    LOG_OP = 5,
    SIN_OP = 6,
    COS_OP = 7,
};

union NodeValue
{
    double val_value;
    OperationType op_value;
    char var_value;
};

struct Node
{
    NodeType node_type = INCORRECT_TYPE;
    NodeValue value = {.val_value = NAN};
    Node *left = nullptr;
    Node *right = nullptr;
};

struct Tree
{
    Node *root = nullptr;
//    size_t size = 0;
};

enum TreeErrors
{
    NO_ERRORS                 = 0,
    TREE_NO_ERRORS            = 0,
    TREE_IS_NULLPTR           = 1,
    NODE_IS_NULLPTR           = 2,
    TREE_FILENAME_IS_NULLPTR  = 3,
    CANT_OPEN_TREE_FILE       = 4,
    TREE_CANT_ALLOCATE_MEMORY = 5,
    TREE_NO_SUCH_ELEMENT      = 6,
    TOO_MANY_ATTEMPTS_TO_READ = 7,
    STRING_IS_NULLPTR         = 8,
    FILE_IS_NULLPTR           = 9,
    CANT_OPEN_FILE            = 10,
};

#include "recursive_descent.h"

/**
 * @brief constructor of tree
 *
 * @param tree tree to construct
 * @return error code
 */
size_t treeCtor(Tree *tree);

/**
 * @brief constructor of node
 *
 * @param node node to construct
 * @return error code
 */
size_t nodeCtor(Node *node);

/**
 * @brief destructor of tree
 *
 * @param tree tree to destruct
 * @return error code
 */
size_t treeDtor(Tree *tree);

/**
* @brief destructor of node
*
* @param node node to destruct
* @return error code
*/
size_t nodeDtor(Node *node);

/**
 * @brief saves tree to file
 *
 * @param tree tree to save
 * @param fp file for saving
 * @return error code
 */
size_t treeSaveToFile(Tree *tree, FILE *fp);

/**
 * @brief prints nodes in preorder in specific format
 *
 * @param node node to print
 * @param fp file to print
 * @param num_spaces number of formatting spaces
 * @return error code
 */
size_t nodePreOrderPrint(Node *node, FILE *fp, size_t num_spaces = 0);

/**
 * @brief prints nodes in inorder
 *
 * @param node node to print
 * @param fp file to print
 * @param num_spaces number of formatting spaces
 * @return error code
 */
size_t nodeInOrderPrint(Node *node, FILE *fp, size_t num_spaces = 0);

/**
 * @brief prints nodes in postorder
 *
 * @param node node to print
 * @param fp file to print
 * @param num_spaces number of formatting spaces
 * @return error code
 */
size_t nodePostOrderPrint(Node *node, FILE *fp, size_t num_spaces = 0);

/**
 * @brief reads tree from file
 *
 * @param tree tree to save data from file
 * @param filename name of file to read
 * @return error code
 */
size_t readTree(Tree *tree, const char *filename);

Node *readNode(char **readPtr, char **buffer, long lenOfFile);

Node *readRecursiveDescentNode(const char **readPtr);

Node *createNewNode(NodeType node_type,
                    NodeValue node_value,
                    Node *left_node,
                    Node *right_node);

Node *createNum(double value);

Node *createNode(NodeType node_type,
                 NodeValue node_value,
                 Node *left_node,
                 Node *right_node);


#define CHECK_NULLPTR_ERROR(value, error) \
    {                                     \
        assert(#value && (int)error);     \
        if (value == nullptr)             \
            return error;                 \
    }

#define VALUE (node->value)
#define VAL_VALUE (VALUE.val_value)
#define OP_VALUE (VALUE.op_value)
#define VAR_VALUE (VALUE.var_value)
#define LEFT_NODE (node->left)
#define RIGHT_NODE (node->right)
#define NODE_TYPE (node->node_type)
#define IS_OP(OPP) (NODE_TYPE == OPERATION && OP_VALUE == (OPP))
#define IS_NUM_LEFT (LEFT_NODE->node_type == NUMBER)
#define IS_NUM_RIGHT (RIGHT_NODE->node_type == NUMBER)
#define IS_ZERO_LEFT (IS_NUM_LEFT && abs(LEFT_VALUE) < EPS)
#define IS_ZERO_RIGHT (IS_NUM_RIGHT && abs(RIGHT_VALUE) < EPS)
#define IS_ONE_LEFT (IS_NUM_LEFT && abs(LEFT_VALUE - 1) < EPS)
#define IS_ONE_RIGHT (IS_NUM_RIGHT && abs(RIGHT_VALUE - 1) < EPS)
#define LEFT_VALUE (LEFT_NODE->value.val_value)
#define RIGHT_VALUE (RIGHT_NODE->value.val_value)
#define LEFT_TYPE LEFT_NODE->node_type
#define RIGHT_TYPE RIGHT_NODE->node_type
#define IS_LEFT_TYPE(TYPE) ((LEFT_TYPE) == (TYPE))
#define IS_RIGHT_TYPE(TYPE) ((RIGHT_TYPE) == (TYPE))

#endif //DIFFERENTIATOR__TREE_H
