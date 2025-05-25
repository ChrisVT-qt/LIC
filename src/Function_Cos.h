// Function_Cos.h
// Class definition

#ifndef FUNCTION_COS_H
#define FUNCTION_COS_H

// Project includes
#include "AbstractFunction.h"



// Define class
class Function_Cos
    : public AbstractFunction
{
    // ============================================================== Lifecycle
private:
    // Constructor
    Function_Cos();

public:
    // Create representation of a given function
    static AbstractFunction * CreateFunction(QString mFunction);

    // Destructor
    virtual ~Function_Cos();



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
