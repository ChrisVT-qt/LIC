# === Where files can be found
INCLUDEPATH += src/
INCLUDEPATH += shared/

# === Where files go
OBJECTS_DIR = build/
MOC_DIR = build/

# === Frameworks and compiler
TEMPLATE = app
DEPENDPATH += .
QT += gui
QT += xml
QT += widgets
CONFIG += c++17
CONFIG += release
CONFIG += silent

# Don't allow deprecated versions of methods (before Qt 6.8)
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060800

# Shared classes
HEADERS += shared/CallTracer.h
SOURCES += shared/CallTracer.cpp
HEADERS += shared/MessageLogger.h
SOURCES += shared/MessageLogger.cpp
HEADERS += shared/StringHelper.h
SOURCES += shared/StringHelper.cpp

# Specific classes
HEADERS += src/AbstractFunction.h
SOURCES += src/AbstractFunction.cpp
HEADERS += src/Deploy.h
HEADERS += src/Function_Constant.h
SOURCES += src/Function_Constant.cpp
HEADERS += src/Function_Cos.h
SOURCES += src/Function_Cos.cpp
HEADERS += src/Function_Difference.h
SOURCES += src/Function_Difference.cpp
HEADERS += src/Function_Exp.h
SOURCES += src/Function_Exp.cpp
HEADERS += src/Function_Exponent.h
SOURCES += src/Function_Exponent.cpp
HEADERS += src/Function_Log.h
SOURCES += src/Function_Log.cpp
HEADERS += src/Function_Product.h
SOURCES += src/Function_Product.cpp
HEADERS += src/Function_Quotient.h
SOURCES += src/Function_Quotient.cpp
HEADERS += src/Function_Sign.h
SOURCES += src/Function_Sign.cpp
HEADERS += src/Function_Sin.h
SOURCES += src/Function_Sin.cpp
HEADERS += src/Function_Sqrt.h
SOURCES += src/Function_Sqrt.cpp
HEADERS += src/Function_Sum.h
SOURCES += src/Function_Sum.cpp
HEADERS += src/Function_Tan.h
SOURCES += src/Function_Tan.cpp
HEADERS += src/Function_Variable.h
SOURCES += src/Function_Variable.cpp
HEADERS += src/LIC.h
SOURCES += src/LIC.cpp
HEADERS += src/Macros.h
SOURCES += src/main.cpp
