// Function_Exp.cpp
// Class implementation

// Project includes
#include "Function_Exp.h"
#include "Macros.h"
#include "MessageLogger.h"

// Qt includes
#include <QDebug>

// System includes
#include <cmath>



// ================================================================== Lifecycle



///////////////////////////////////////////////////////////////////////////////
// Constructor
Function_Exp::Function_Exp()
{
    // No argument
    m_Argument = nullptr;
}



///////////////////////////////////////////////////////////////////////////////
// Create representation of a given function
AbstractFunction * Function_Exp::CreateFunction(QString mFunction)
{
    // Return value
    Function_Exp * ret = new Function_Exp();

    // Remove unnecessary parentheses
    mFunction = ret -> RemoveUnnecessaryParentheses(mFunction);

    // Search top level "exp(...)"
    if (!mFunction.startsWith("exp(") ||
        !mFunction.endsWith(")"))
    {
        // Didn't work.
        delete ret;
        return nullptr;
    }
    mFunction = mFunction.mid(3);

    // Check if we have only one pair of parentheses at the top level
    int num_top_open_parentheses = 0;
    int level = 0;
    for (int idx=0; idx < mFunction.size(); idx++)
    {
        if (mFunction[idx] == '(')
        {
            if (level == 0)
            {
                num_top_open_parentheses++;
            }
            level++;
        }
        if (mFunction[idx] == ')')
        {
            level--;
        }
    }

    // Check we have multiple pairs of top level parentheses
    if (num_top_open_parentheses > 1)
    {
        // Yes. There's certainly other stuff to interpret before the exp().
        delete ret;
        return nullptr;
    }

    // Successful
    mFunction = mFunction.mid(1, mFunction.size()-2);
    AbstractFunction * argument = AbstractFunction::CreateFunction(mFunction);
    if (argument)
    {
        // It worked!
        ret -> m_Argument = argument;
        return ret;
    } else
    {
        // Did not work
        delete argument;
        MessageLogger::Error(METHOD_NAME,
            QString("Couldn't parse argument: %1").arg(mFunction));
        delete ret;
        return nullptr;
    }
}


///////////////////////////////////////////////////////////////////////////////
// Destructor
Function_Exp::~Function_Exp()
{
    // Delete argument
    if (m_Argument)
    {
        delete m_Argument;
    }
}



// ============================================================== Functionality



///////////////////////////////////////////////////////////////////////////////
// If the function is simple or consists of complex terms
bool Function_Exp::IsSimple() const
{
    return true;
}



///////////////////////////////////////////////////////////////////////////////
// Serialize to String
QString Function_Exp::ToString() const
{
    const QString argument = m_Argument -> ToString();
    return QString("exp(%1)").arg(argument);
}



///////////////////////////////////////////////////////////////////////////////
// Evaluate
double Function_Exp::Evaluate(
    const QHash < QString, double > mcVariables) const
{
    const double argument = m_Argument -> Evaluate(mcVariables);
    return exp(argument);
}
