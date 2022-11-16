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

void printLatex(Tree *tree);

void printLatexNode(Node *node, FILE *fp);

#endif //DIFFERENTIATOR__DIFFERENTIATOR_H
