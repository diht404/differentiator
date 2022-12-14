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
    "\\usepackage{graphicx}\n"
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
    {"Легко видеть, что \n",
     "Очевидно, что \n",
     "Используя Wolfram легко получить, что \n",
     "В любом учебнике написано, что \n",
     "Как рассказывали в начальной школе, \n",
     "Оставим доказательство данного факта читателю "
     "в качестве несложного упражнения: \n",
     "Применяя знания, полученные на прошлой лекции, читатель "
     "без труда получит, что \n",
     "Используя теорему 1000 из тома 7 главы 666 и лемму 42 из тома 13 "
     "главы 66 нетрудно получить, что \n",
     "В результате простых рассуждений можно получить \n",
     "Нетрудно догадаться, что \n",
     "Отсюда очевидно следует, что \n",
     "Примем без доказательства, что \n",
     "Доказательство данного факта можно найти в "
     "\\href{https://www.youtube.com/watch?v=dQw4w9WgXcQ}{видеолекции} \n",
     "Доказательство будет дано в следующем издании учебника. \n",
     "Мне было лень доказывать этот факт: \n",
     "Зачем Вы читаете эти комментарии, в них нет никакого смысла... \n",
     "Тут могла быть Ваша реклама, а теперь несложное утверждение\n",
     "(((Какой-то комментарий))) \n"};

size_t set_latex_file(const char *filename);

size_t close_latex_file();

void taylorN(Node *node, const char variable, double x, int n);

void printTaylorN(Node *node,
                  double *taylor_coefficients,
                  const char variable,
                  double x,
                  int n);

void diffXYZ(Tree *tree, double x, double y, double z);

Node *diff(const Node *node, const char variable);

Node *diffOperation(const Node *node, const char variable);

Node *diffLog(const Node *node, const char variable);

Node *diffPow(const Node *node, const char variable);

Node *copyNode(Node *node);

void simplifyNode(Node *node);

void convConst(Node *node, bool *changed);

void deleteNeutralElements(Node *node, bool *changed);

void moveNodeUp(Node *node,
                Node *node_to_up,
                Node *node_to_delete,
                bool *changed);

void changeNodeTypeToNumber(Node *node,
                            double value,
                            bool *changed);

void startLatexFormula(const Node *node, bool printStartBracket);

void endLatexFormula(const Node *node);

void getTangentEquation(Tree *tree,
                        Tree *diff_tree,
                        const char var_name,
                        double value);

void plotGraph(Tree *tree,
               char var_name,
               double left,
               double right,
               size_t num,
               const char *filename);

double calculateNode(Node *node, double x, double y, double z);

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

bool printLatexRequireBrackets(Node *node);

int getPriority(const Node *node);

double oneVariableCalculateNode(Node *node,
                                const char variable,
                                double value);

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

#endif //DIFFERENTIATOR__DIFFERENTIATOR_H
