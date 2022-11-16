#include "logs.h"

FILE *TREE_LOG_FILE = stderr;
const char *TREE_GRAPH_LOG_FILENAME = "logs.html";

size_t TREE_GRAPH_LOG_VERSION = 0;

void treeSetLogFile()
{
    FILE *fp = fopen(TREE_GRAPH_LOG_FILENAME, "a");
    if (fp == nullptr)
        return;

    TREE_LOG_FILE = fp;
    setvbuf(TREE_LOG_FILE, nullptr, _IONBF, 0);
}

void treeClearGraphLogFile()
{
    FILE *fp = fopen(TREE_GRAPH_LOG_FILENAME, "w");
    if (fp == nullptr)
        return;
    fclose(fp);
}

void treeCloseLogFile()
{
    if (TREE_LOG_FILE != nullptr)
        fclose(TREE_LOG_FILE);
}

size_t treeDump(Tree *tree)
{
    CHECK_NULLPTR_ERROR(tree, TREE_IS_NULLPTR)

    char photo_name[128] = "";

    sprintf(photo_name,
            "log_v.%zu.jpg",
            TREE_GRAPH_LOG_VERSION);

    fprintf(TREE_LOG_FILE, "<pre>\n");

    createGraph(tree, photo_name);
    fprintf(TREE_LOG_FILE, "<img src=%s />\n", photo_name);
    TREE_GRAPH_LOG_VERSION++;
}

size_t createGraph(Tree *tree, const char *photo_name)
{
    CHECK_NULLPTR_ERROR(tree, TREE_IS_NULLPTR)
    CHECK_NULLPTR_ERROR(photo_name, TREE_FILENAME_IS_NULLPTR)

    FILE *fp = fopen(GRAPH_FILENAME, "w");
    fprintf(fp, "digraph TREE {\n"
                "    rankdir=TB;\n");

    createGraphNodes(tree->root, fp);
    createGraphEdges(tree->root, fp);

    // close graph with }
    fprintf(fp, "}\n");

    char command[128] = "";
    fclose(fp);
    sprintf(command,
            "dot %s -T jpg -o %s",
            GRAPH_FILENAME,
            photo_name);
    system(command);
    return TREE_NO_ERRORS;
}

size_t createGraphNodes(Node *node, FILE *fp)
{
    if (!node)
        return TREE_NO_ERRORS;

    size_t error = TREE_NO_ERRORS;

    fprintf(fp,
            "    node_%p[shape=\"record\", \n"
            "        color=%s,"
            "        fillcolor=%s, \n"
            "        style=\"rounded, filled\", \n"
            "        label=\"\n"
            "            {%s? |\n"
            "            {{LEFT | %p} | {INDEX | %p} | {RIGHT | %p}}}\"]\n",
            node,
            GREEN_COLOR,
            LIGHT_GREEN_COLOR,
            node->value,
            node->left,
            node,
            node->right);

    if (node->left)
        error = createGraphNodes(node->left, fp);
    if (error)
        return error;

    if (node->right)
        error = createGraphNodes(node->right, fp);
    if (error)
        return error;

    return TREE_NO_ERRORS;
}

size_t createGraphEdges(Node *node, FILE *fp)
{
    if (!node)
        return TREE_NO_ERRORS;

    size_t error = TREE_NO_ERRORS;

    if (node->left)
    {
        fprintf(fp,
                "    node_%p->node_%p[label=\"YES\", color=%s, fontcolor=%s];",
                node,
                node->left,
                PURPLE_COLOR,
                GREEN_COLOR);
        error = createGraphEdges(node->left, fp);
        if (error)
            return error;
    }

    if (node->right)
    {
        fprintf(fp,
                "    node_%p->node_%p[label=\"NO\",  color=%s, fontcolor=%s];",
                node,
                node->right,
                PURPLE_COLOR,
                RED_COLOR);
        error = createGraphEdges(node->right, fp);
        if (error)
            return error;
    }

    return TREE_NO_ERRORS;
}