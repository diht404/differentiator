#ifndef DIFFERENTIATOR__DIFFERENTIATOR_H
#define DIFFERENTIATOR__DIFFERENTIATOR_H

#include "stdio.h"
#include "stdlib.h"
#include "assert.h"

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
    "\n"
    "\\oddsidemargin=6mm\n"
    "\\textwidth=150mm\n"
    "\\headheight=-32.4mm\n"
    "\\textheight=277mm\n"
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
    "\\begin{document}";

const char LATEX_END_DOCUMENT[] = "\\end{document}";

Node *diff(const Node *node);

Node *diffLog(const Node *node);

Node *diffPop(const Node *node);

Node *createNum(double value);

Node *createNode(NodeType node_type,
                 OperationType op_type,
                 Node *left_node,
                 Node *right_node);

Node *copyNode(Node *node);

void printLatex(Tree *tree);

void printLatexNode(Node *node, FILE *fp);

#define dL diff(node->left)
#define dR diff(node->right)
#define cL copyNode(node->left)
#define cR copyNode(node->right)
#define ADD(dL, dR) createNode(OPERATION, ADD_OP, dL, dR)
#define SUB(dL, dR) createNode(OPERATION, SUB_OP, dL, dR)
#define MUL(dL, dR) createNode(OPERATION, MUL_OP, dL, dR)
#define DIV(dL, dR) createNode(OPERATION, DIV_OP, dL, dR)
#define POW(dL, dR) createNode(OPERATION, POW_OP, dL, dR)
#define LOG(dL, dR) createNode(OPERATION, LOG_OP, dL, dR)
#define SIN(dL, dR) createNode(OPERATION, SIN_OP, dL, dR)
#define COS(dL, dR) createNode(OPERATION, COS_OP, dL, dR)

#endif //DIFFERENTIATOR__DIFFERENTIATOR_H
