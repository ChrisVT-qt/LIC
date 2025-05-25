// Function_Sqrt.h
// Class definition

#ifndef FUNCTION_SQRT_H
#define FUNCTION_SQRT_H

// Project includes
#include "AbstractFunction.h"



// Define class
class Function_Sqrt
    : public AbstractFunction
{
    // ============================================================== Lifecycle
private:
    // Constructor
    Function_Sqrt();

public:
    // Create representation of a given function
    static AbstractFunction * CreateFunction(QString mFunction);

    // Destructor
    virtual ~Function_Sqrt();



    // ========================================================== Functionality
public:
    // If the function is simple or cosists of complex terms
    virtual bool IsSimple() const;

    // Serialize to String
    virtual QString ToString() const;

    // Evaluate
    virtual double Evaluate(const QHash < QString, double > mcVariables) const;

private:
    AbstractFunction * m_Argument;
};

#endif
