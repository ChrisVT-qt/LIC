// Function_Sign.h
// Class definition

#ifndef FUNCTION_SIGN_H
#define FUNCTION_SIGN_H

// Project includes
#include "AbstractFunction.h"



// Define class
class Function_Sign
    : public AbstractFunction
{
    // ============================================================== Lifecycle
private:
    // Constructor
    Function_Sign();

public:
    // Create representation of a given function
    static AbstractFunction * CreateFunction(QString mFunction);

    // Destructor
    virtual ~Function_Sign();



    // ========================================================== Functionality
public:
    // If the function is simple or cosists of complex terms
    virtual bool IsSimple() const;

    // Serialize to String
    virtual QString ToString() const;

    // Evaluate
    virtual double Evaluate(const QHash < QString, double > mcVariables) const;

private:
    QString m_Sign;
    AbstractFunction * m_Argument;
};

#endif
