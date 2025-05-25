// Function_Variable.h
// Class definition

#ifndef FUNCTION_VARIABLE_H
#define FUNCTION_VARIABLE_H

// Project includes
#include "AbstractFunction.h"



// Define class
class Function_Variable
    : public AbstractFunction
{
    // ============================================================== Lifecycle
private:
    // Constructor
    Function_Variable();

public:
    // Create representation of a given function
    static AbstractFunction * CreateFunction(QString mFunction);

    // Destructor
    virtual ~Function_Variable();



    // ========================================================== Functionality
public:
    // If the function is simple or cosists of complex terms
    virtual bool IsSimple() const;

    // Serialize to String
    virtual QString ToString() const;

    // Evaluate
    virtual double Evaluate(const QHash < QString, double > mcVariables) const;

private:
    QString m_VariableName;
};

#endif
