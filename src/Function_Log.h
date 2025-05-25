// Function_Log.h
// Class definition

#ifndef FUNCTION_LOG_H
#define FUNCTION_LOG_H

// Project includes
#include "AbstractFunction.h"



// Define class
class Function_Log
    : public AbstractFunction
{
    // ============================================================== Lifecycle
private:
    // Constructor
    Function_Log();

public:
    // Create representation of a given function
    static AbstractFunction * CreateFunction(QString mFunction);

    // Destructor
    virtual ~Function_Log();



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
