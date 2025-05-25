// Function_Constant.h
// Class definition

#ifndef FUNCTION_CONSTANT_H
#define FUNCTION_CONSTANT_H

// Project includes
#include "AbstractFunction.h"



// Define class
class Function_Constant
    : public AbstractFunction
{
    // ============================================================== Lifecycle
private:
    // Constructor
    Function_Constant();

public:
    // Create representation of a given function
    static AbstractFunction * CreateFunction(QString mFunction);

    // Destructor
    virtual ~Function_Constant();



    // ========================================================== Functionality
public:
    // If the function is simple or cosists of complex terms
    virtual bool IsSimple() const;

    // Serialize to String
    virtual QString ToString() const;

    // Evaluate
    virtual double Evaluate(const QHash < QString, double > mcVariables) const;

private:
    QString m_ConstantText;
    double m_Constant;
};

#endif
