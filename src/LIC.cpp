// LIC.cpp
// Class implementation

// Project includes
#include <AbstractFunction.h>
#include <LIC.h>
#include <Macros.h>
#include <MessageLogger.h>
#include <QTextStream>

// Qt includes
#include <QDebug>
#include <QElapsedTimer>
#include <QFile>
#include <QImage>
#include <QPixmap>
#include <QRegularExpression>

// System includes
#include <cmath>



// This implementation is based on the original paper:
//
// Cabral, Brian; Leedom, Leith Casey (August 2–6, 1993). "Imaging Vector
// Fields Using Line Integral Convolution". Proceedings of the 20th annual
// conference on Computer graphics and interactive techniques. SIGGRAPH '93.
// Anaheim, California. pp. 263–270.



// ================================================================== Lifecycle



///////////////////////////////////////////////////////////////////////////////
// Constructor
LIC::LIC()
{
    m_IsValid = false;
}



///////////////////////////////////////////////////////////////////////////////
// Destructor
LIC::~LIC()
{
    // Delete functions
    for (auto coordinate_iterator = m_Vectorfield.keyBegin();
         coordinate_iterator != m_Vectorfield.keyEnd();
         coordinate_iterator++)
    {
        const QString coordinate = *coordinate_iterator;
        delete m_Vectorfield[coordinate];
    }
}



// ============================================================== Configuration



///////////////////////////////////////////////////////////////////////////////
// Read configuration
bool LIC::ReadXMLConfiguration(const QString mcFilename)
{
    QFile input_file(mcFilename);
    if (!input_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        MessageLogger::Error(METHOD_NAME,
            QString("File \"%1\" could not be found.").arg(mcFilename));
        return false;
    }
    QTextStream stream(&input_file);
    const QString xml = stream.readAll();

    // Parse as XML
    QDomDocument dom("stuff");
    QDomDocument::ParseResult parse_result = dom.setContent(xml);
    if (!parse_result.errorMessage.isEmpty())
    {
        const QString reason = tr("XML cannot be parsed - %1#%2#%3")
            .arg(parse_result.errorMessage,
                 QString::number(parse_result.errorLine),
                 QString::number(parse_result.errorColumn));
        MessageLogger::Error(METHOD_NAME,
            QString("XML cannot be parsed: %1.").arg(reason));
        return false;
    }

    // Parse everything
    QDomElement dom_lic = dom.firstChildElement("lic");
    QDomElement dom_vectorfield = dom_lic.firstChildElement("vectorfield");
    bool success = ParseVectorfield(dom_vectorfield);
    if (!success)
    {
        return false;
    }
    QDomElement dom_background = dom_lic.firstChildElement("background");
    success = ParseBackground(dom_background);
    if (!success)
    {
        return false;
    }
    QDomElement dom_image = dom_lic.firstChildElement("image");
    success = ParseImage(dom_image);
    if (!success)
    {
        return false;
    }

    // Done
    m_IsValid = true;
    return true;
}



///////////////////////////////////////////////////////////////////////////////
// Vectorfield
bool LIC::ParseVectorfield(QDomElement & mrDomVectorfield)
{
    // Check if DOM element exists
    if (mrDomVectorfield.isNull())
    {
        MessageLogger::Error(METHOD_NAME,
            QString("<lic><vectorfield> does not exist."));
        return false;
    }

    // Read parameters
    static const QRegularExpression format_name("^[a-zA-Z_][a-zA-Z0-9_]*$");
    static const QRegularExpression format_value("^[0-9]+(\\.[0-9]*)?$");
    QDomElement dom_parameters =
        mrDomVectorfield.firstChildElement("parameters");
    if (!dom_parameters.isNull())
    {
        for (QDomElement dom_parameter =
                dom_parameters.firstChildElement("parameter");
             !dom_parameter.isNull();
             dom_parameter = dom_parameter.nextSiblingElement("parameter"))
        {
            const QString name = dom_parameter.attribute("name");
            const QRegularExpressionMatch match_name = format_name.match(name);
            if (!match_name.hasMatch())
            {
                MessageLogger::Error(METHOD_NAME,
                    QString("Parameter name \"%1\" is not a valid name.")
                        .arg(name));
                return false;
            }
            if (m_Parameters.contains(name))
            {
                MessageLogger::Error(METHOD_NAME,
                    QString("Parameter \"%1\" has already been specified.")
                        .arg(name));
                return false;
            }
            if (name == "x" ||
                name == "y")
            {
                MessageLogger::Error(METHOD_NAME,
                    QString("Parameter name \"%1\" cannot be used as it is "
                        "reserved for the coordinates.").arg(name));
                return false;
            }
            const QString value = dom_parameter.attribute("value");
            const QRegularExpressionMatch match_value =
                format_value.match(value);
            if (!match_value.hasMatch())
            {
                MessageLogger::Error(METHOD_NAME,
                    QString("Parameter value \"%1\" is not valid.")
                        .arg(value));
                return false;
            }

            // Save the parameter
            m_Parameters[name] = value.toDouble();
        }
    }

    // Read vectorfield formulas
    QDomElement dom_formulas = mrDomVectorfield.firstChildElement("formulas");

    // Iterations
    // !!! Some checks
    const QString iterations = dom_formulas.attribute("iterations", "1");
    m_Vectorfield_Iterate = iterations.toInt();

    if (!dom_formulas.isNull())
    {
        for (QDomElement dom_child = dom_formulas.firstChildElement();
             !dom_child.isNull();
             dom_child = dom_child.nextSiblingElement())
        {
            const QString coordinate = dom_child.tagName();
            if (coordinate != "x" &&
                coordinate != "y")
            {
                MessageLogger::Error(METHOD_NAME,
                    tr("Invalid coordinate \"%1\" in "
                        "<lic><vectorfield><formulas>. Must be \"x\" or "
                        "\"y\".").arg(coordinate));
                return false;
            }
            if (m_Vectorfield.contains(coordinate))
            {
                MessageLogger::Error(METHOD_NAME,
                    tr("Coordinate \"%1\" in "
                        "<lic><vectorfield><formulas> has already been "
                        "defined.").arg(coordinate));
                return false;
            }
            const QString formula = dom_child.text();
            AbstractFunction * function =
                AbstractFunction::CreateFunction(formula);
            if (!function)
            {
                MessageLogger::Error(METHOD_NAME,
                    tr("Formula \"%1\" for %2 coordinate cannot be parsed.")
                        .arg(formula,
                             coordinate));
                return false;
            }
            m_Vectorfield[coordinate] = function;
        }
    }

    // Both x and y have to be specified
    if (!m_Vectorfield.contains("x") ||
        !m_Vectorfield.contains("y"))
    {
        MessageLogger::Error(METHOD_NAME,
            QString("Both x and y vector fields need to be specified in "
                "<lic><vectorfield><formulas>."));
        return false;
    }

    // Done
    return true;
}



///////////////////////////////////////////////////////////////////////////////
// Background
bool LIC::ParseBackground(QDomElement & mrDomBackground)
{
    // Check if DOM element exists
    if (mrDomBackground.isNull())
    {
        MessageLogger::Error(METHOD_NAME,
            QString("<lic><background> does not exist."));
        return false;
    }

    // Attributes
    m_BackgroundType = mrDomBackground.attribute("type");
    if (m_BackgroundType == "white noise")
    {
        static const QRegularExpression format_value("^[0-9]+(\\.[0-9]*)?$");
        const QString cutoff = mrDomBackground.attribute("cutoff");
        const QRegularExpressionMatch match_value = format_value.match(cutoff);
        if (!match_value.hasMatch())
        {
            MessageLogger::Error(METHOD_NAME,
                QString("Invalid background cutoff parameter \"%1\". Must be "
                    "a value between 0.0 and 1.0.").arg(cutoff));
            return false;
        }
        m_WhiteNoise_Cutoff = cutoff.toDouble();
    } else if (m_BackgroundType == "checkerboard")
    {
        const int width = mrDomBackground.attribute("width", "1").toInt();
        m_Checkerboard_Width = width;
        if (width < 1)
        {
            MessageLogger::Error(METHOD_NAME,
                QString("Invalid background width parameter \"%1\" for "
                    "checkerboard. Must be an integer number >= 1.")
                    .arg(width));
            return false;
        }
    } else if (m_BackgroundType == "gaussian")
    {
        const QString sigma = mrDomBackground.attribute("sigma", "1");
        m_Gaussian_Sigma = sigma.toDouble();
        if (m_Gaussian_Sigma <= 0)
        {
            MessageLogger::Error(METHOD_NAME,
                QString("Invalid value \"%1\" for sigma in the guassian "
                    "distribution. Must be > 0.").arg(sigma));
            return false;
        }
        const QString low = mrDomBackground.attribute("low", "0.0");
        m_Gaussian_Low = low.toDouble();
        if (m_Gaussian_Low < 0 ||
            m_Gaussian_Low > 1.0)
        {
            MessageLogger::Error(METHOD_NAME,
                QString("Invalid value \"%1\" for low value in the guassian "
                    "distribution. Must be >= 0.").arg(low));
            return false;
        }
        const QString high = mrDomBackground.attribute("high", "1.0");
        m_Gaussian_High = high.toDouble();
        if (m_Gaussian_High < 0 ||
            m_Gaussian_High > 1.0)
        {
            MessageLogger::Error(METHOD_NAME,
                QString("Invalid value \"%1\" for high value in the guassian "
                    "distribution. Must be >= 0.").arg(high));
            return false;
        }
        if (m_Gaussian_Low > m_Gaussian_High)
        {
            MessageLogger::Error(METHOD_NAME,
                QString("Low and high values in the guassian distribution are "
                    "in an invalid oder."));
            return false;
        }
    } else
    {
        MessageLogger::Error(METHOD_NAME,
            QString("Invalid background type \"%1\".").arg(m_BackgroundType));
        return false;
    }
    m_BackgroundSeed = mrDomBackground.attribute("seed", "0").toInt();

    // Done
    return true;
}



///////////////////////////////////////////////////////////////////////////////
// Image
bool LIC::ParseImage(QDomElement & mrDomImage)
{
    // Check if DOM element exists
    if (mrDomImage.isNull())
    {
        MessageLogger::Error(METHOD_NAME,
            QString("<lic><image> does not exist."));
        return false;
    }

    // Steps
    m_Steps = mrDomImage.attribute("steps").toInt();
    if (m_Steps < 1)
    {
        MessageLogger::Error(METHOD_NAME,
            QString("Invalid number of steps \"%1\" in <lic><image>< needs to "
                "be at least 1.").arg(m_Steps));
        return false;
    }

    // Ranges
    QDomElement dom_ranges = mrDomImage.firstChildElement("ranges");
    QSet < QString > found;
    if (!dom_ranges.isNull())
    {
        for (QDomElement dom_range = dom_ranges.firstChildElement("range");
             !dom_range.isNull();
             dom_range = dom_range.nextSiblingElement("range"))
        {
            const QString variable = dom_range.attribute("variable");
            if (found.contains(variable))
            {
                MessageLogger::Error(METHOD_NAME,
                    QString("Ranges for %1 have already been "
                        "specified.").arg(variable));
                return false;
            }
            if (variable == "x")
            {
                found += "x";
                m_Image_XMin = dom_range.attribute("min").toDouble();
                m_Image_XMax = dom_range.attribute("max").toDouble();
            } else if (variable == "y")
            {
                found += "y";
                m_Image_YMin = dom_range.attribute("min").toDouble();
                m_Image_YMax = dom_range.attribute("max").toDouble();
            } else
            {
                MessageLogger::Error(METHOD_NAME,
                    QString("<lic><image><ranges> references an unknown "
                        "variable \"%1\".").arg(variable));
                return false;
            }
        }
    }
    if (found.size() != 2)
    {
        MessageLogger::Error(METHOD_NAME,
            QString("Both x and y ranges need to be specified in "
                "<lic><image><ranges>."));
        return false;
    }

    // Resolution
    QDomElement dom_resolution = mrDomImage.firstChildElement("resolution");
    if (dom_resolution.isNull())
    {
        MessageLogger::Error(METHOD_NAME,
            QString("<lic><image> does not appear to have a <resolution> "
                "tag."));
        return false;
    }
    m_Image_Width = dom_resolution.attribute("x").toInt();
    if (m_Image_Width < 10)
    {
        MessageLogger::Error(METHOD_NAME,
            QString("<lic><image><resolution> specifies an invalid width %1 "
                "of the final image.").arg(m_Image_Width));
        return false;
    }
    m_Image_Height = dom_resolution.attribute("y").toInt();
    if (m_Image_Height < 10)
    {
        MessageLogger::Error(METHOD_NAME,
            QString("<lic><image><resolution> specifies an invalid height %1 "
                "of the final image.").arg(m_Image_Width));
        return false;
    }

    // Output
    QDomElement dom_output = mrDomImage.firstChildElement("output");
    if (dom_output.isNull())
    {
        MessageLogger::Error(METHOD_NAME,
            QString("<lic><image> does not appear to have a <output> tag."));
        return false;
    }
    m_OutputFilename = dom_output.attribute("filename");
    if (m_OutputFilename.isEmpty())
    {
        MessageLogger::Error(METHOD_NAME,
            QString("<lic><image><output> specifies an empty file name."));
        return false;
    }

    // Done
    return true;
}



///////////////////////////////////////////////////////////////////////////////
// Create the LIC image
void LIC::Execute()
{
    // Check if parameters are valid
    if (!m_IsValid)
    {
        MessageLogger::Error(METHOD_NAME,
            QString("Set of parameters isn't valid; can't execute."));
        return;
    }

    // Generate underlying noise patters
    GenerateNoise();

    // Generate LIC
    GenerateLIC();

    // Generate Image
    GenerateImage();
}



///////////////////////////////////////////////////////////////////////////////
// Generate underlying noise patterm
void LIC::GenerateNoise()
{
    // Background noise
    m_Noise_R.reserve(m_Image_Width * m_Image_Height);
    m_Noise_G.reserve(m_Image_Width * m_Image_Height);
    m_Noise_B.reserve(m_Image_Width * m_Image_Height);

    // Apply random seed
    srand48(m_BackgroundSeed);

    // This could be generated in a more efficient way, but we like to keep
    // it this way so we can implement other ways more easily.
    for (int ix = 0; ix < m_Image_Width; ix++)
    {
        for (int iy = 0; iy < m_Image_Height; iy++)
        {
            const int idx = ix * m_Image_Height + iy;
            bool is_white = false;

            // Check type of noise
            if (m_BackgroundType == "white noise")
            {
                is_white = (drand48() > m_WhiteNoise_Cutoff);
            }
            if (m_BackgroundType == "checkerboard")
            {
                is_white = (((ix + iy) / m_Checkerboard_Width) % 2 == 0);
            }
            if (m_BackgroundType == "gaussian")
            {
                double x = ix * 1. / (m_Image_Width - 1.);
                x = m_Image_XMin + (m_Image_XMax - m_Image_XMin) * x;
                x = x - 0.5 * (m_Image_XMax + m_Image_XMin);
                double y = iy * 1. / (m_Image_Height - 1.);
                y = m_Image_YMin + (m_Image_YMax - m_Image_YMin) * y;
                y = y - 0.5 * (m_Image_YMax + m_Image_YMin);
                double threshold =
                    exp(-0.5*(x*x+y*y)/pow(m_Gaussian_Sigma,2));
                threshold = m_Gaussian_Low +
                    (m_Gaussian_High-m_Gaussian_Low) * threshold;
                is_white = (drand48() < threshold);
            }

            // Actually pick the color
            if (is_white)
            {
                m_Noise_R[idx] = 255;
                m_Noise_G[idx] = 255;
                m_Noise_B[idx] = 255;
            } else
            {
                m_Noise_R[idx] = 0;
                m_Noise_G[idx] = 0;
                m_Noise_B[idx] = 0;
            }
        }
    }

    // Done
}

#include <QTime>

///////////////////////////////////////////////////////////////////////////////
// Generate LIC
void LIC::GenerateLIC()
{
    // Elapsed/to go time
    QElapsedTimer ti;
    ti.start();

    // Initialize LIC colors
    m_LIC_R.reserve(m_Image_Width * m_Image_Height);
    m_LIC_G.reserve(m_Image_Width * m_Image_Height);
    m_LIC_B.reserve(m_Image_Width * m_Image_Height);
    m_LIC_Strength.reserve(m_Image_Width * m_Image_Height);

    // For conversion from grid to actual x/y values
    const double dx = (m_Image_XMax - m_Image_XMin) / (m_Image_Width - 1.);
    const double dy = (m_Image_YMax - m_Image_YMin) / (m_Image_Height - 1.);

    for (int ix = 0; ix < m_Image_Width; ix++)
    {
        const double elapsed = ti.elapsed() * 0.001;
        const QString elapsed_text = QString("%1:%2:%3")
                .arg(int(elapsed)/3600, 2, 10, QChar('0'))
                .arg(int(elapsed)%3600/60, 2, 10, QChar('0'))
                .arg(int(elapsed)%60, 2, 10, QChar('0'));
        const double remaining = elapsed * (m_Image_Width - 1 - ix)/ ix;
        QString remaining_text = "---";
        if (ix > 1)
        {
            remaining_text = QString("%1:%2:%3")
                .arg(int(remaining)/3600, 2, 10, QChar('0'))
                .arg(int(remaining)%3600/60, 2, 10, QChar('0'))
                .arg(int(remaining)%60, 2, 10, QChar('0'));
        }
        qDebug().noquote() << tr("Step %1/%2 - elapsed %3, remaining %4")
            .arg(QString::number(ix),
                 QString::number(m_Image_Width-1),
                 elapsed_text,
                 remaining_text);
        for (int iy = 0; iy < m_Image_Height; iy++)
        {
            double color_r = 0.;
            double color_g = 0.;
            double color_b = 0.;

            double lic_length = 0;
            for (int direction : {-1, 1})
            {
                // Grid coordinate system
                // We have to remember that the origin of our coordinate system
                // is at the top left corner, not at the bottom left corner
                int grid_x = ix;
                double grid_dx = 0;
                int grid_y = (m_Image_Height - 1) - iy;
                double grid_dy = 0;

                for (int step = 0; step < m_Steps; step++)
                {
                    // Evaluate vector field
                    double x = m_Image_XMin + (grid_x + grid_dx) * dx;
                    double y = m_Image_YMin + (grid_y + grid_dy) * dy;
                    QPair < double, double > v = EvaluateVectorfield(x,y);
                    double vx = direction * v.first;
                    double vy = direction * v.second;

                    // Normalize
                    double r = sqrt(vx*vx + vy*vy);
                    if (r <= 1e-14)
                    {
                        // Avoid singularities; we're not escaping a vanishing
                        // vector field anyway.
                        break;
                    }
                    vx /= r;
                    vy /= r;

                    // Calculate step size in this grid box
                    double sx = 1e10;
                    if (vx > 0)
                    {
                        sx = (1. - grid_dx)/vx;
                    } else if (vx < 0)
                    {
                        if (abs(grid_dx) <= 1e-13)
                        {
                            sx = -1/vx;
                        } else
                        {
                            sx = -grid_dx/vx;
                        }
                    }
                    double sy = 1e10;
                    if (vy > 0)
                    {
                        sy = (1. - grid_dy)/vy;
                    } else if (vy < 0)
                    {
                        if (abs(grid_dy) <= 1e-13)
                        {
                            sy = -1/vy;
                        } else
                        {
                            sy = -grid_dy/vy;
                        }
                    }
                    double s = qMin(sx, sy);

                    // Integrate color
                    int color_grid_x = grid_x % m_Image_Width;
                    if (color_grid_x < 0)
                    {
                        color_grid_x += m_Image_Width;
                    }
                    int color_grid_y = grid_y % m_Image_Height;
                    if (color_grid_y < 0)
                    {
                        color_grid_y += m_Image_Height;
                    }
                    const int idx =
                        (color_grid_x * m_Image_Height + color_grid_y)
                            % (m_Image_Width * m_Image_Height);
                    //if (r > 0.1)
                    double sink_capture = 1/(1/r+1);
                    color_r += sink_capture * s * m_Noise_R[idx];
                    color_g += sink_capture * s * m_Noise_G[idx];
                    color_b += sink_capture * s * m_Noise_B[idx];

                    // Update coordinates
                    grid_dx += s * vx;
                    if (grid_dx < 0)
                    {
                        grid_x -= 1;
                        grid_dx += 1;
                    }
                    if (grid_dx >= 1)
                    {
                        grid_x += 1;
                        grid_dx -= 1;
                    }
                    grid_dy += s * vy;
                    if (grid_dy < 0)
                    {
                        grid_y -= 1;
                        grid_dy += 1;
                    }
                    if (grid_dy >= 1)
                    {
                        grid_y += 1;
                        grid_dy -= 1;
                    }

                    // Add up color
                    lic_length += s;
                }
            }

            // Set point
            color_r /= lic_length;
            color_g /= lic_length;
            color_b /= lic_length;

            // Save information
            const int idx = ix * m_Image_Height + iy;
            m_LIC_R[idx] = color_r;
            m_LIC_G[idx] = color_g;
            m_LIC_B[idx] = color_b;

            // Strength
            double x = m_Image_XMin + ix * dx;
            double y = m_Image_YMin + iy * dy;
            QPair < double, double > v = EvaluateVectorfield(x,y);
            double vx = v.first;
            double vy = v.second;
            const double strength = sqrt(vx*vx + vy*vy);
            m_LIC_Strength[ix * m_Image_Height + iy] = strength;
        }
    }

    // Show strength of the vector field
    // qDebug() << QString("Strength: min=%1, max=%2").arg(min_strength)
    //      .arg(max_strength);
}



///////////////////////////////////////////////////////////////////////////////
// Evaluate vector field
QPair < double, double > LIC::EvaluateVectorfield(double mX, double mY)
{
    QHash < QString, double > values = m_Parameters;
    for (int iteration = 0;
         iteration < m_Vectorfield_Iterate;
         iteration++)
    {
        values["x"] = mX;
        values["y"] = mY;
        mX = m_Vectorfield["x"] -> Evaluate(values);
        mY = m_Vectorfield["y"] -> Evaluate(values);
    }
    return QPair < double, double >(mX, mY);
}



///////////////////////////////////////////////////////////////////////////////
// Generate image
void LIC::GenerateImage()
{
    // === Renormalize

    // Generate output picture
    double min_intensity = 1e10;
    double max_intensity = -1e10;
    for (int idx = 0; idx < m_Image_Width * m_Image_Height; idx++)
    {
        min_intensity = qMin(min_intensity, m_LIC_R[idx]);
        min_intensity = qMin(min_intensity, m_LIC_G[idx]);
        min_intensity = qMin(min_intensity, m_LIC_B[idx]);
        max_intensity = qMax(max_intensity, m_LIC_R[idx]);
        max_intensity = qMax(max_intensity, m_LIC_G[idx]);
        max_intensity = qMax(max_intensity, m_LIC_B[idx]);
    }
    for (int idx = 0; idx < m_Image_Width * m_Image_Height; idx++)
    {
        m_LIC_R[idx] = (m_LIC_R[idx] - min_intensity) /
            (max_intensity - min_intensity) * 255;
        m_LIC_G[idx] = (m_LIC_G[idx] - min_intensity) /
            (max_intensity - min_intensity) * 255;
        m_LIC_B[idx] = (m_LIC_B[idx] - min_intensity) /
            (max_intensity - min_intensity) * 255;
    }

    // === Apply color
    double min_strength = 1e10;
    double max_strength = -1e10;
    for (int idx = 0; idx < m_Image_Width * m_Image_Height; idx++)
    {
        min_strength = qMin(min_strength, m_LIC_Strength[idx]);
        max_strength = qMax(max_strength, m_LIC_Strength[idx]);
    }
    for (int idx = 0; idx < m_Image_Width * m_Image_Height; idx++)
    {
        const double weight = (m_LIC_Strength[idx] - min_strength) /
            (max_strength - min_strength);
        // m_LIC_G[idx] *= pow(weight,.2);
        // m_LIC_B[idx] *= pow(weight,.2);
    }

    // Generate and save image
    QImage lic_image = QPixmap(m_Image_Width, m_Image_Height).toImage();
    for (int ix = 0; ix < m_Image_Width; ix++)
    {
        for (int iy = 0; iy < m_Image_Height; iy++)
        {
            const int idx = ix * m_Image_Height + iy;
            lic_image.setPixelColor(ix, iy,
                QColor(m_LIC_R[idx], m_LIC_G[idx], m_LIC_B[idx]));
        }
    }
    lic_image.save(m_OutputFilename, "png");
}
