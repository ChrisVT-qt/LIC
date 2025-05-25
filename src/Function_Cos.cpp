// Function_Cos.cpp
// Class implementation

// Project includes
#include "Function_Cos.h"
#include "Macros.h"
#include "MessageLogger.h"

// Qt includes
#include <QDebug>

// System includes
#include <cmath>



// ================================================================== Lifecycle



///////////////////////////////////////////////////////////////////////////////
// Constructor
Function_Cos::Function_Cos()
{
    // No argument so far
    m_Argument = nullptr;
}



///////////////////////////////////////////////////////////////////////////////
// Create representation of a given function
AbstractFunction * Function_Cos::CreateFunction(QString mFunction)
{
    // Return value
    Function_Cos * ret = new Function_Cos();

    // Remove unnecessary parentheses
    mFunction = ret -> RemoveUnnecessaryParentheses(mFunction);

    // Search top level "cos(...)"
    if (!mFunction.startsWith("cos(") ||
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
        // Yes. There's certainly other stuff to interpret before the
        // cos().
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
Function_Cos::~Function_Cos()
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
bool Function_Cos::IsSimple() const
{
    return true;
}



///////////////////////////////////////////////////////////////////////////////
// Serialize to String
QString Function_Cos::ToString() const
{
    const QString argument = m_Argument -> ToString();
    return QString("cos(%1)").arg(argument);
}



///////////////////////////////////////////////////////////////////////////////
// Evaluate
double Function_Cos::Evaluate(
    const QHash < QString, double > mcVariables) const
{
    const double argument = m_Argument -> Evaluate(mcVariables);
    return cos(argument);
}
