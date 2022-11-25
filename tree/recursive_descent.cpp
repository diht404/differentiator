#include "recursive_descent.h"

Node *recursiveDescent(const char **program)
{
    assert(program != nullptr);

    Node *value = getAddSub(program);

    ASSERT_OK(**program == '\0',
              "Incorrect symbol: _%c_\n",
              **program)

    return value;
}

Node *getAddSub(const char **program)
{
    assert(program != nullptr);
    assert(*program != nullptr);

    Node *leftValue = getMulDiv(program);

    while (**program == '+' || **program == '-')
    {
        char operation = **program;
        (*program)++;

        Node *rightValue = getMulDiv(program);
        if (operation == '+')
            leftValue = createNewNode(OPERATION,
                                      {.op_value = ADD_OP},
                                      leftValue,
                                      rightValue);
        else
            leftValue = createNewNode(OPERATION,
                                      {.op_value = SUB_OP},
                                      leftValue,
                                      rightValue);
    }

    return leftValue;
}

Node *getMulDiv(const char **program)
{
    assert(program != nullptr);
    assert(*program != nullptr);

    Node *leftValue = getPow(program);

    while (**program == '*' || **program == '/')
    {
        char operation = **program;
        (*program)++;

        Node *rightValue = getPow(program);

        if (operation == '*')
            leftValue = createNewNode(OPERATION,
                                      {.op_value = MUL_OP},
                                      leftValue,
                                      rightValue);
        else
            leftValue = createNewNode(OPERATION,
                                      {.op_value = DIV_OP},
                                      leftValue,
                                      rightValue);
    }

    return leftValue;
}

Node *getPow(const char **program)
{
    assert(program != nullptr);
    assert(*program != nullptr);

    Node *leftValue = getLog(program);
    Node *rightValue = nullptr;

    while (**program == '^')
    {
        (*program)++;

        rightValue = getPow(program);
        leftValue = createNewNode(OPERATION,
                                  {.op_value = POW_OP},
                                  leftValue,
                                  rightValue);
    }

    return leftValue;
}

Node *getLog(const char **program)
{
    assert(program != nullptr);
    assert(*program != nullptr);

    Node *value = nullptr;

    if (**program == 'l' && *((*program) + 1) == 'o'
        && *((*program) + 2) == 'g')
    {
        (*program) += 3;
        ASSERT_OK(**program == '(',
                  "Expected (, but got _%c_\n",
                  **program)
        value = getSin(program);
        value = createNewNode(OPERATION,
                              {.op_value = LOG_OP},
                              createNum(exp(1)),
                              value);
    }
    else
        value = getSin(program);

    return value;
}

Node *getSin(const char **program)
{
    assert(program != nullptr);
    assert(*program != nullptr);

    Node *value = nullptr;

    if (**program == 's' &&
        *((*program) + 1) == 'i' &&
        *((*program) + 2) == 'n')
    {
        (*program) += 3;
        ASSERT_OK(**program == '(',
                  "Expected (, but got _%c_\n",
                  **program)
        value = getCos(program);
        value = createNewNode(OPERATION,
                              {.op_value = SIN_OP},
                              nullptr,
                              value);
    }
    else
        value = getCos(program);

    return value;
}

Node *getCos(const char **program)
{
    assert(program != nullptr);
    assert(*program != nullptr);

    Node *value = nullptr;

    if (**program == 'c' &&
        *((*program) + 1) == 'o' &&
        *((*program) + 2) == 's')
    {
        (*program) += 3;
        ASSERT_OK(**program == '(',
                  "Expected (, but got _%c_\n",
                  **program)
        value = getBrackets(program);
        value = createNewNode(OPERATION,
                              {.op_value = COS_OP},
                              nullptr,
                              value);
    }
    else
        value = getBrackets(program);

    return value;
}

Node *getBrackets(const char **program)
{
    assert(program != nullptr);
    assert(*program != nullptr);

    Node *value = nullptr;

    if (**program == '(')
    {
        (*program)++;

        value = getAddSub(program);
        ASSERT_OK(**program == ')',
                  "Expected ), but got _%c_\n",
                  **program)
        (*program)++;
    }
    else
        value = getValue(program);

    return value;
}

Node *getValue(const char **program)
{
    assert(program != nullptr);
    assert(*program != nullptr);

    const char *startPtr = *program;

    if (isalpha(**program))
        return getVariable(program);

    double number = NAN;
    int len = 0;
    sscanf(*program, "%lf %n", &number, &len);
    // len of value
    (*program) += len;

    Node *value = createNewNode(NUMBER,
                                {.val_value = number},
                                nullptr,
                                nullptr);

    ASSERT_OK(startPtr != *program,
              "incorrect digit: _%c_\n",
              **program)

    return value;
}

Node *getVariable(const char **program)
{
    assert(program != nullptr);
    assert(*program != nullptr);

    Node *value = nullptr;
    char variable = '\0';

    if (isalpha(**program))
    {
        variable = **program;
        (*program) += 1;

        value = createNewNode(VARIABLE,
                              {.var_value = variable},
                              nullptr,
                              value);
    }
    else
        return nullptr;

    return value;
}
