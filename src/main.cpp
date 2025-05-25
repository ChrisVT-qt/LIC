// main.cpp

// Project includes
#include "LIC.h"

// Qt includes
#include <QApplication>
#include <QDebug>



///////////////////////////////////////////////////////////////////////////////
// Main
int main(int mNumParameters, char * mpParameter[])
{
    // Application
    QApplication app(mNumParameters, mpParameter);

    // Check for correct number of arguments
    if (mNumParameters < 2)
    {
        const QString command_name = mpParameter[0];
        qDebug().noquote() <<
            QString("Usage: %1 [config.xml]\n").arg(command_name);
        return 0;
    }

    // Read configuration XML file
    const QString filename = mpParameter[1];
    LIC * lic = new LIC();
    lic -> ReadXMLConfiguration(filename);

    // Do it.
    lic -> Execute();

    // Done
    return 0;
}
