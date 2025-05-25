// LIC.h
// Class definition

#ifndef LIC_H
#define LIC_H

// Qt includes
#include <QDomElement>
#include <QHash>
#include <QObject>

// Forward declaration
class AbstractFunction;



// Define class
class LIC
    : public QObject
{
    // ============================================================== Lifecycle
public:
    // Constructor
    LIC();

    // Destructor
    virtual ~LIC();



    // ========================================================== Configuration
public:
    // Read configuration
    bool ReadXMLConfiguration(const QString mcFilename);

private:
    // Vectorfield
    bool ParseVectorfield(QDomElement & mrDomVectorfield);

    QHash < QString, double > m_Parameters;
    QHash < QString, AbstractFunction * > m_Vectorfield;
    int m_Vectorfield_Iterate;

    // Background
    bool ParseBackground(QDomElement & mrDomBackground);

    QString m_BackgroundType;
    int m_BackgroundSeed;
    double m_WhiteNoise_Cutoff;
    int m_Checkerboard_Width;
    double m_Gaussian_Sigma;
    double m_Gaussian_Low;
    double m_Gaussian_High;

    // Image
    bool ParseImage(QDomElement & mrDomImage);

    int m_Steps;
    double m_Image_XMin;
    double m_Image_XMax;
    double m_Image_YMin;
    double m_Image_YMax;
    int m_Image_Width;
    int m_Image_Height;
    QString m_OutputFilename;

    bool m_IsValid;

public:
    // Create the LIC image
    void Execute();

private:
    // Noise Generator
    void GenerateNoise();

    QList < double > m_Noise_R;
    QList < double > m_Noise_G;
    QList < double > m_Noise_B;

    // Generate LIC
    void GenerateLIC();
    QPair < double, double > EvaluateVectorfield(double mX, double mY);

    QList < double > m_LIC_R;
    QList < double > m_LIC_G;
    QList < double > m_LIC_B;
    QList < double > m_LIC_Strength;

    // Generate Image
    void GenerateImage();
};

#endif
