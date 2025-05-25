// AbstractFunction.cpp
// Class implementation

// Project includes
#include "AbstractFunction.h"
#include "Function_Constant.h"
#include "Function_Cos.h"
#include "Function_Difference.h"
#include "Function_Exp.h"
#include "Function_Exponent.h"
#include "Function_Log.h"
#include "Function_Product.h"
#include "Function_Quotient.h"
#include "Function_Sign.h"
#include "Function_Sin.h"
#include "Function_Sqrt.h"
#include "Function_Sum.h"
#include "Function_Tan.h"
#include "Function_Variable.h"

// Qt includes
#include <QDebug>



// ================================================================== Lifecycle



///////////////////////////////////////////////////////////////////////////////
// Constructor
AbstractFunction::AbstractFunction()
{
    // Nothing to do
}
    


///////////////////////////////////////////////////////////////////////////////
// Create representation of a given function
AbstractFunction * AbstractFunction::CreateFunction(QString mFunction)
{
    // Try all the different function types, by precedence
    AbstractFunction * ret = nullptr;
    if (!ret)
    {
        ret = Function_Sum::CreateFunction(mFunction);
    }
    if (!ret)
    {
        ret = Function_Difference::CreateFunction(mFunction);
    }
    if (!ret)
    {
        ret = Function_Product::CreateFunction(mFunction);
    }
    if (!ret)
    {
        ret = Function_Sign::CreateFunction(mFunction);
    }
    if (!ret)
    {
        ret = Function_Quotient::CreateFunction(mFunction);
    }
    if (!ret)
    {
        ret = Function_Exponent::CreateFunction(mFunction);
    }
    if (!ret)
    {
        ret = Function_Constant::CreateFunction(mFunction);
    }
    if (!ret)
    {
        ret = Function_Variable::CreateFunction(mFunction);
    }
    if (!ret)
    {
        ret = Function_Cos::CreateFunction(mFunction);
    }
    if (!ret)
    {
        ret = Function_Sin::CreateFunction(mFunction);
    }
    if (!ret)
    {
        ret = Function_Tan::CreateFunction(mFunction);
    }
    if (!ret)
    {
        ret = Function_Exp::CreateFunction(mFunction);
    }
    if (!ret)
    {
        ret = Function_Log::CreateFunction(mFunction);
    }
    if (!ret)
    {
        ret = Function_Sqrt::CreateFunction(mFunction);
    }
    return ret;
}



///////////////////////////////////////////////////////////////////////////////
// Destructor
AbstractFunction::~AbstractFunction()
{
    // Nothing to do
}



// ============================================================== Functionality



///////////////////////////////////////////////////////////////////////////////
// Remove unnecessary parenteses
QString AbstractFunction::RemoveUnnecessaryParentheses(QString mFunction) const
{
    // There may be more than one unnecessary pair of parentheses
    while (true)
    {
        // Get rid of white space
        mFunction = mFunction.trimmed();

        // Check if first and last character are opening parenteses; if not, we
        // can simplify more before removing parentheses
        if (!mFunction.startsWith("(") ||
            !mFunction.endsWith(")"))
        {
            break;
        }

        // Now there are two cases...
        // - "(for + bar) + (yada + 2 * damn)" which can be simplified further
        // - "(foo + bar*7)" which is the case we want

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

        // Check if we have the first case
        if (num_top_open_parentheses > 1)
        {
            return mFunction;
        }

        // Second case
        mFunction = mFunction.mid(1, mFunction.size()-2);
    }

    // Done
    return mFunction;
}
