// Function_Difference.h
// Class definition

#ifndef FUNCTION_DIFFERENCE_H
#define FUNCTION_DIFFERENCE_H

// Project includes
#include "AbstractFunction.h"



// Define class
class Function_Difference
    : public AbstractFunction
{
    // ============================================================== Lifecycle
private:
    // Constructor
    Function_Difference();

public:
    // Create representation of a given function
    static AbstractFunction * CreateFunction(QString mFunction);

    // Destructor
    virtual ~Function_Difference();



    // ========================================================== Functionality
public:
    // If the function is simple or cosists of complex terms
    virtual bool IsSimple() const;

    // Serialize to String
    virtual QString ToString() const;

    // Evaluate
    virtual double Evaluate(const QHash < QString, double > mcVariables) const;

private:
    AbstractFunction * m_LeftFunction;
    AbstractFunction * m_RightFunction;
};

#endif
