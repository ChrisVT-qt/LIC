// Function_Exponent.h
// Class definition

#ifndef FUNCTION_EXPONENT_H
#define FUNCTION_EXPONENT_H

// Project includes
#include "AbstractFunction.h"



// Define class
class Function_Exponent
    : public AbstractFunction
{
    // ============================================================== Lifecycle
private:
    // Constructor
    Function_Exponent();

public:
    // Create representation of a given function
    static AbstractFunction * CreateFunction(QString mFunction);

    // Destructor
    virtual ~Function_Exponent();



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
