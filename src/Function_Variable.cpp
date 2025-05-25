// Function_Variable.cpp
// Class implementation

// Project includes
#include "Function_Variable.h"
#include "Macros.h"

// Qt includes
#include <QDebug>
#include <QRegularExpression>



// ================================================================== Lifecycle



///////////////////////////////////////////////////////////////////////////////
// Constructor
Function_Variable::Function_Variable()
{
    // Nothing to do
}



///////////////////////////////////////////////////////////////////////////////
// Create representation of a given function
AbstractFunction * Function_Variable::CreateFunction(QString mFunction)
{
    // Return value
    Function_Variable * ret = new Function_Variable();

    // Remove unnecessary parentheses
    mFunction = ret -> RemoveUnnecessaryParentheses(mFunction);

    // Any variable starts with a character, then continues with characters
    // digits, or an underscore
    static const QRegularExpression format("^[a-zA-Z][a-zA-Z0-9_]*$");
    const QRegularExpressionMatch match = format.match(mFunction);
    if (match.hasMatch())
    {
        // It worked
        ret -> m_VariableName = mFunction;
        return ret;
    }

    // Didn't work - that's okay, it just wasn't a variable name
    delete ret;
    return nullptr;
}



///////////////////////////////////////////////////////////////////////////////
// Destructor
Function_Variable::~Function_Variable()
{
    // Nothing to do
}



// ============================================================== Functionality



///////////////////////////////////////////////////////////////////////////////
// If the function is simple or consists of complex terms
bool Function_Variable::IsSimple() const
{
    return true;
}



///////////////////////////////////////////////////////////////////////////////
// Serialize to String
QString Function_Variable::ToString() const
{
    return m_VariableName;
}



///////////////////////////////////////////////////////////////////////////////
// Evaluate
double Function_Variable::Evaluate(const QHash < QString, double > mcVariables
    ) const
{
    // Check if variable is known
    if (mcVariables.contains(m_VariableName))
    {
        return mcVariables[m_VariableName];
    } else
    {
        return 0./0.;
    }
}
