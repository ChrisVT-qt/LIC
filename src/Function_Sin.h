// Function_Sin.h
// Class definition

#ifndef FUNCTION_SIN_H
#define FUNCTION_SIN_H

// Project includes
#include "AbstractFunction.h"



// Define class
class Function_Sin
    : public AbstractFunction
{
    // ============================================================== Lifecycle
private:
    // Constructor
    Function_Sin();

public:
    // Create representation of a given function
    static AbstractFunction * CreateFunction(QString mFunction);

    // Destructor
    virtual ~Function_Sin();



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
