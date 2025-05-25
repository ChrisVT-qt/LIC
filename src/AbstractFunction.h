// AbstractFunction.h
// Class definition

#ifndef ABSTRACTFUNCTION_H
#define ABSTRACTFUNCTION_H

// Qt includes
#include <QObject>

// Define class
class AbstractFunction
    : public QObject
{
    Q_OBJECT
    
    
    // ============================================================== Lifecycle
public:
    // Constructor
    AbstractFunction();

    // Create representation of a given function
    static AbstractFunction * CreateFunction(QString mFunction);

    // Destructor
    virtual ~AbstractFunction();


    
    // ========================================================== Functionality
public:
    // If the function is simple or cosists of complex terms
    virtual bool IsSimple() const = 0;

    // Serialize to String
    virtual QString ToString() const = 0;

    // Evaluate
    virtual double Evaluate(const QHash < QString, double > mcVariables
        ) const = 0;

protected:
    // Remove unnecessary parenteses
    QString RemoveUnnecessaryParentheses(QString mFunction) const;
};

#endif
