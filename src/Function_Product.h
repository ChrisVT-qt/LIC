// Function_Product.h
// Class definition

#ifndef FUNCTION_PRODUCT_H
#define FUNCTION_PRODUCT_H

// Project includes
#include "AbstractFunction.h"



// Define class
class Function_Product
    : public AbstractFunction
{
    // ============================================================== Lifecycle
private:
    // Constructor
    Function_Product();

public:
    // Create representation of a given function
    static AbstractFunction * CreateFunction(QString mFunction);

    // Destructor
    virtual ~Function_Product();



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
