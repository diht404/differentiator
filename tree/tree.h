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

//    double val_value = 0;
//    OperationType op_value = INCORRECT_OP;
//    char var_value = 0;

//    Val_t value = nullptr;
    Node *left = nullptr;
    Node *right = nullptr;
};

struct Tree
{
    Node *root = nullptr;
    size_t size = 0;
};

enum TreeErrors
{
    TREE_NO_ERRORS = 0,
    TREE_IS_NULLPTR = 1,
    NODE_IS_NULLPTR = 2,
    TREE_FILENAME_IS_NULLPTR = 3,
    CANT_OPEN_TREE_FILE = 4,
    TREE_CANT_ALLOCATE_MEMORY = 5,
    TREE_NO_SUCH_ELEMENT = 6,
    TOO_MANY_ATTEMPTS_TO_READ = 7,
    STRING_IS_NULLPTR = 8,
    FILE_IS_NULLPTR = 9,
};

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

Node *createNewNode(NodeType node_type,
                    NodeValue node_value,
//                    double value,
//                    OperationType op_type,
//                    char variable,
                    Node *left_node,
                    Node *right_node);

//
///**
// * @brief parses node to tree
// *
// * @param tree tree to add node
// * @param node node to add after
// * @param buffer buffer with text info about new node
// * @param readPtr pointer to element in buffer to start parsing
// * @param lenOfFile length of file to read
// * @return error code
// */
//size_t parseNode(Tree *tree,
//                 Node *node,
//                 char **buffer,
//                 char **readPtr,
//                 long lenOfFile);
//
//bool processEndNode(bool *isToken,
//                    Tree *tree,
//                    Node *node,
//                    char **startTokenPtr,
//                    char **endTokenPtr,
//                    char **readPtr);
//
//size_t processBeginNode(bool *isToken,
//                        Tree *tree,
//                        Node *node,
//                        char **startTokenPtr,
//                        char **endTokenPtr,
//                        char **readPtr,
//                        char **buffer,
//                        long lenOfFile);
//
//size_t processToken(bool *isToken,
//                    char **readPtr,
//                    char **startTokenPtr,
//                    char **endTokenPtr);
//
///**
// * @brief creates node from text buffer
// *
// * @param new_node node to create
// * @param tree tree of nodes
// * @param node node to insert after
// * @param startTokenPtr start pointer to value
// * @param endTokenPtr end pointer to value
// * @return error code
// */
//size_t createNode(Node **new_node,
//                  Tree *tree,
//                  Node *node,
//                  char **startTokenPtr,
//                  char **endTokenPtr);
//
///**
// * @brief sets value of node
// *
// * @param tree tree with nodes
// * @param new_node node to set value
// * @param startTokenPtr start pointer to value
// * @param endTokenPtr end pointer to value
// * @return error code
// */
//size_t setNodeValue(Tree *tree,
//                    Node *new_node,
//                    char **startTokenPtr,
//                    char **endTokenPtr);
//
///**
// * @brief creates node if node is not root
// *
// * @param node node to create after
// * @param new_node new node to create
// * @return
// */
//size_t createNoRootNode(Node *node, Node *new_node);
//
///**
// * @brief inserts node
// *
// * @param node node to change with delimiter value and
// * add value of this node as answer to "no"
// * @param value value to insert if "yes"
// * @param value_size length of value string
// * @param delimiter value to insert in node
// * @param delimiter_size length of delimiter string
// * @return error code
// */
//size_t insertNode(Node *node,
//                  char *value,
//                  size_t value_size,
//                  char *delimiter,
//                  size_t delimiter_size);
//

#define CHECK_NULLPTR_ERROR(value, error) \
    {                                     \
        assert(#value && (int)error);     \
        if (value == nullptr)             \
            return error;                 \
    }

#endif //DIFFERENTIATOR__TREE_H
