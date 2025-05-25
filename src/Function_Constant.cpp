// Function_Constant.cpp
// Class implementation

// Project includes
#include "Function_Constant.h"
#include "Macros.h"

// Qt includes
#include <QDebug>
#include <QRegularExpression>
#include <QString>



// ================================================================== Lifecycle



///////////////////////////////////////////////////////////////////////////////
// Constructor
Function_Constant::Function_Constant()
{
    // Nothing to do
}



///////////////////////////////////////////////////////////////////////////////
// Create representation of a given function
AbstractFunction * Function_Constant::CreateFunction(QString mFunction)
{
    // Return value
    Function_Constant * ret = new Function_Constant();

    // Remove unnecessary parentheses
    mFunction = ret -> RemoveUnnecessaryParentheses(mFunction);

    // Any constant is a number.
    static const QRegularExpression format("^[\\+\\-]?[0-9]+(\\.[0-9]*)?$");
    const QRegularExpressionMatch match = format.match(mFunction);
    if (match.hasMatch())
    {
        // It worked
        ret -> m_ConstantText = mFunction;
        ret -> m_Constant = mFunction.toDouble();
        return ret;
    }

    // Didn't work, but that's okay - expression just wasn't a constant
    delete ret;
    return nullptr;

}


///////////////////////////////////////////////////////////////////////////////
// Destructor
Function_Constant::~Function_Constant()
{
    // Nothing to do
}



// ============================================================== Functionality



///////////////////////////////////////////////////////////////////////////////
// If the function is simple or consists of complex terms
bool Function_Constant::IsSimple() const
{
    return true;
}



///////////////////////////////////////////////////////////////////////////////
// Serialize to String
QString Function_Constant::ToString() const
{
    return m_ConstantText;
}



///////////////////////////////////////////////////////////////////////////////
// Evaluate
double Function_Constant::Evaluate(
    const QHash < QString, double > mcVariables) const
{
    Q_UNUSED(mcVariables)

    return m_Constant;
}
