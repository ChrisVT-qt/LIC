// Function_Quotient.h
// Class definition

#ifndef FUNCTION_QUOTIENT_H
#define FUNCTION_QUOTIENT_H

// Project includes
#include "AbstractFunction.h"



// Define class
class Function_Quotient
    : public AbstractFunction
{
    // ============================================================== Lifecycle
private:
    // Constructor
    Function_Quotient();

public:
    // Create representation of a given function
    static AbstractFunction * CreateFunction(QString mFunction);

    // Destructor
    virtual ~Function_Quotient();



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
