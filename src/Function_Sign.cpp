// Function_Sign.cpp
// Class implementation

// Project includes
#include "Function_Sign.h"
#include "Macros.h"
#include "MessageLogger.h"

// Qt includes
#include <QDebug>



// ================================================================== Lifecycle



///////////////////////////////////////////////////////////////////////////////
// Constructor
Function_Sign::Function_Sign()
{
    // No argument
    m_Argument = nullptr;
}



///////////////////////////////////////////////////////////////////////////////
// Create representation of a given function
AbstractFunction * Function_Sign::CreateFunction(QString mFunction)
{
    // Return value
    Function_Sign * ret = new Function_Sign();

    // Remove unnecessary parentheses
    mFunction = ret -> RemoveUnnecessaryParentheses(mFunction);

    // Search top level "+|-..."
    if (!mFunction.startsWith("+") &&
        !mFunction.startsWith("-"))
    {
        // Didn't work.
        delete ret;
        return nullptr;
    }

    // Remember sign
    ret -> m_Sign = mFunction.left(1);

    // The rest
    mFunction = mFunction.mid(1);
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
Function_Sign::~Function_Sign()
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
bool Function_Sign::IsSimple() const
{
    return false;
}



///////////////////////////////////////////////////////////////////////////////
// Serialize to String
QString Function_Sign::ToString() const
{
    const QString argument = m_Argument -> ToString();
    const QString sign = (m_Sign == "-" ? "-" : "");
    return QString("%1sign(%2)")
        .arg(sign,
             argument);
}



///////////////////////////////////////////////////////////////////////////////
// Evaluate
double Function_Sign::Evaluate(
    const QHash < QString, double > mcVariables) const
{
    const double argument = m_Argument -> Evaluate(mcVariables);
    if (m_Sign == "-")
    {
        return -argument;
    } else
    {
        return argument;
    }
}
