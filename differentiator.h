#ifndef DIFFERENTIATOR__DIFFERENTIATOR_H
#define DIFFERENTIATOR__DIFFERENTIATOR_H
#include "stdio.h"
#include "stdlib.h"
#include "assert.h"
#include "time.h"

#include "tree/tree.h"
#include "tree/logs.h"

const char DIFFERENTIATOR_FILE[] = "tree.txt";

const char LATEX_START_DOCUMENT[] =
    "\\documentclass[12pt,a4paper,fleqn]{article}\n"
    "\\usepackage[utf8]{inputenc}\n"
    "\\usepackage[russian]{babel}\n"
    "\\usepackage[shortcuts,cyremdash]{extdash}\n"
    "\\usepackage{wrapfig}\n"
    "\\usepackage{floatflt}\n"
    "\\usepackage{lipsum}\n"
    "\\usepackage{concmath}\n"
    "\\usepackage{euler}\n"
    "\\usepackage{libertine}\n"
    "\\usepackage{amsfonts}\n"
    "\\usepackage{amsmath}\n"
    "\\usepackage{hyperref}\n"
    "\n"
    "\\oddsidemargin=6mm\n"
    "\n"
    "\\parindent=0pt\n"
    "\\parskip=8pt\n"
    "\\pagestyle{empty}\n"
    "\\usepackage[normalem]{ulem} % uline\n"
    "\\usepackage{mdframed}\n"
    "\\usepackage{amsthm}\n"
    "\\theoremstyle{definition}\n"
    "\\newtheorem{Def}{Def.}\n"
    "\\newtheorem{Ex}{Ex.}\n"
    "\n"
    "\\newenvironment{Sol}[1][]{\\textcolor{blue}{\\bfseries Решение. }}{}\n"
    "\n"
    "\\flushbottom"
    "\n"
    "\\begin{document}"
    ""
    "Решим элементарную задачу на дифференцирование, которую автор "
    "данного учебника решал еще в 5 классе.\n\n";

const char LATEX_END_DOCUMENT[] = "\\end{document}";

const char CRINGE_PHRASES[][BUFFER_SIZE] =
    {"Легко видеть, что\n",
     "Очевидно, что\n",
     "Используя Wolfram легко получить, что\n",
     "В любом учебнике написано, что\n",
     "Как рассказывали в начальной школе,\n",
     "Оставим доказательство данного факта читателю в качестве упражнения.\n",
     "В результате простых рассуждений можно получить\n",
     "Нетрудно догадаться, что\n",
     "Отсюда очевидно следует, что\n",
     "Примем без доказательства, что",
     "Доказательство данного факта можно найти в "
     "\\href{https://www.youtube.com/watch?v=dQw4w9WgXcQ}{видеолекции}\n",
     "Доказательство будет дано "
     "в следующем издании учебника.\n",
     "Мне было лень доказывать этот факт.\n",
     "Зачем Вы читаете эти комментарии, в них нет никакого смысла...\n",
     "Тут могла быть Ваша реклама.\n",
     "(((Какой-то комментарий)))\n"};

size_t set_latex_file(const char *filename);

size_t close_latex_file();

Node *diff(const Node *node, const char variable);

Node *diffOperation(const Node *node, const char variable);

Node *diffLog(const Node *node, const char variable);

Node *diffPop(const Node *node, const char variable);

Node *createNum(double value);

Node *createNode(NodeType node_type,
                 NodeValue node_value,
                 Node *left_node,
                 Node *right_node);

Node *copyNode(Node *node);

void simplifyTree(Tree *tree);

void convConst(Node *node, bool *changed);

void deleteNeutralElements(Node *node, bool *changed);

void getTangentEquation(Tree *tree,
                        Tree *diff_tree,
                        const char var_name,
                        double value);

double calculateNode(Node *node, const char variable, double value);

void printLatex(Tree *tree);

void printLatexNode(const Node *node, FILE *fp);

void printLatexSinNode(const Node *node, FILE *fp);

void printLatexCosNode(const Node *node, FILE *fp);

void printLatexLogNode(const Node *node, FILE *fp);

void printLatexDivNode(const Node *node, FILE *fp);

void printLatexPowNode(const Node *node, FILE *fp);

void printLatexOrdinaryNode(const Node *node,
                            char node_value[BUFFER_SIZE],
                            FILE *fp);


#define dL diff(node->left, variable)
#define dR diff(node->right, variable)
#define cL copyNode(node->left)
#define cR copyNode(node->right)
#define ADD(dL, dR) createNode(OPERATION, {.op_value = ADD_OP}, dL, dR)
#define SUB(dL, dR) createNode(OPERATION, {.op_value = SUB_OP}, dL, dR)
#define MUL(dL, dR) createNode(OPERATION, {.op_value = MUL_OP}, dL, dR)
#define DIV(dL, dR) createNode(OPERATION, {.op_value = DIV_OP}, dL, dR)
#define POW(dL, dR) createNode(OPERATION, {.op_value = POW_OP}, dL, dR)
#define LOG(dL, dR) createNode(OPERATION, {.op_value = LOG_OP}, dL, dR)
#define SIN(dL, dR) createNode(OPERATION, {.op_value = SIN_OP}, dL, dR)
#define COS(dL, dR) createNode(OPERATION, {.op_value = COS_OP}, dL, dR)
#define VAL_VALUE (node->value.val_value)
#define OP_VALUE (node->value.op_value)
#define VAR_VALUE (node->value.var_value)
#define IS_OP(OPP) (node->node_type == OPERATION && OP_VALUE == OPP)
#define IS_NUM_LEFT (node->left->node_type == NUMBER)
#define IS_NUM_RIGHT (node->right->node_type == NUMBER)
#define IS_ZERO_LEFT (IS_NUM_LEFT && abs(LEFT_VALUE) < EPS)
#define IS_ZERO_RIGHT (IS_NUM_RIGHT && abs(RIGHT_VALUE) < EPS)
#define IS_ONE_LEFT (IS_NUM_LEFT && abs(LEFT_VALUE - 1) < EPS)
#define IS_ONE_RIGHT (IS_NUM_RIGHT && abs(RIGHT_VALUE - 1) < EPS)
#define LEFT_VALUE (node->left->value.val_value)
#define RIGHT_VALUE (node->right->value.val_value)

#endif //DIFFERENTIATOR__DIFFERENTIATOR_H
