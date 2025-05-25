// Function_Difference.cpp
// Class implementation

// Project includes
#include "Function_Difference.h"
#include "Macros.h"
#include "MessageLogger.h"

// Qt includes
#include <QDebug>



// ================================================================== Lifecycle



///////////////////////////////////////////////////////////////////////////////
// Constructor
Function_Difference::Function_Difference()
{
    // No left and right function
    m_LeftFunction = nullptr;
    m_RightFunction = nullptr;
}



///////////////////////////////////////////////////////////////////////////////
// Create representation of a given function
AbstractFunction * Function_Difference::CreateFunction(QString mFunction)
{
    // Return value
    Function_Difference * ret = new Function_Difference();

    // Remove unnecessary parentheses
    mFunction = ret -> RemoveUnnecessaryParentheses(mFunction);

    // Search top level "-"
    int level = 0;
    for (int idx = 0; idx < mFunction.size(); idx++)
    {
        // Check for parentheses
        if (mFunction[idx] == '(')
        {
            level++;
            continue;
        }
        if (mFunction[idx] == ')')
        {
            level--;
            continue;
        }
        if (level != 0)
        {
            // Operator inside a parenthesis
            continue;
        }
        if (mFunction[idx] != '-')
        {
            // Not a difference operator
            continue;
        }
        if (idx == 0)
        {
            // If at the beginning, it's a sign, not an operator
            continue;
        }

        // Break function into pieces
        const QString left = mFunction.left(idx);
        const QString right = mFunction.mid(idx + 1);
        AbstractFunction * left_function =
            AbstractFunction::CreateFunction(left);
        AbstractFunction * right_function =
            AbstractFunction::CreateFunction(right);
        if (left_function &&
            right_function)
        {
            // It worked!
            ret -> m_LeftFunction = left_function;
            ret -> m_RightFunction = right_function;
            return ret;
        } else
        {
            // Didn't work.
            if (left_function)
            {
                delete left_function;
            } else
            {
                MessageLogger::Error(METHOD_NAME,
                    QString("Couldn't parse left expression %1").arg(left));
            }
            if (right_function)
            {
                delete right_function;
            } else
            {
                MessageLogger::Error(METHOD_NAME,
                    QString("Couldn't parse right expression %1").arg(right));
            }
            delete ret;
            return nullptr;
        }
    }

    // If we get here, we've been unssuccessful; that's okay, the expression
    // is just not a difference!
    return nullptr;
}


///////////////////////////////////////////////////////////////////////////////
// Destructor
Function_Difference::~Function_Difference()
{
    // Delete the left and right function
    if (m_LeftFunction)
    {
        delete m_LeftFunction;
    }
    if (m_RightFunction)
    {
        delete m_RightFunction;
    }
}



// ============================================================== Functionality



///////////////////////////////////////////////////////////////////////////////
// If the function is simple or consists of complex terms
bool Function_Difference::IsSimple() const
{
    return false;
}



///////////////////////////////////////////////////////////////////////////////
// Serialize to String
QString Function_Difference::ToString() const
{
    const QString left = m_LeftFunction -> ToString();
    const QString right = m_RightFunction -> ToString();
    return QString("(%1-%2)")
        .arg(left,
             right);
}



///////////////////////////////////////////////////////////////////////////////
// Evaluate
double Function_Difference::Evaluate(
    const QHash < QString, double > mcVariables) const
{
    const double left = m_LeftFunction -> Evaluate(mcVariables);
    const double right = m_RightFunction -> Evaluate(mcVariables);
    return left - right;
}
