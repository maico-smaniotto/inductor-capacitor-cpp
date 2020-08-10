#include <QStringList>
#include <string>
#include <sstream>
#include <iomanip>

#include "electrical_utils.h"

using namespace std;

const double PI = std::atan(1.0) * 4;

bool stringExists(const string &aStr, const QStringList &aList) {
    return aList.contains(QString::fromStdString(aStr));
}

QStringList getFrequencyUnits() {
    QStringList units;
    units.append(QString::fromStdString(HERTZ));
    units.append(QString::fromStdString(KILO_HERTZ));
    units.append(QString::fromStdString(MEGA_HERTZ));
    units.append(QString::fromStdString(GIGA_HERTZ));
    return units;
}

QStringList getReactanceUnits() {
    QStringList units;
    units.append(QString::fromStdString(OHM));
    units.append(QString::fromStdString(KILO_OHM));
    units.append(QString::fromStdString(MEGA_OHM));
    units.append(QString::fromStdString(GIGA_OHM));
    return units;
}

QStringList getInductanceUnits() {
    QStringList units;
    units.append(QString::fromStdString(HENRY));
    units.append(QString::fromStdString(MILI_HENRY));
    units.append(QString::fromStdString(MICRO_HENRY));
    units.append(QString::fromStdString(NANO_HENRY));
    return units;
}

QStringList getCapacitanceUnits() {
    QStringList units;
    units.append(QString::fromStdString(FARAD));
    units.append(QString::fromStdString(MILI_FARAD));
    units.append(QString::fromStdString(MICRO_FARAD));
    units.append(QString::fromStdString(NANO_FARAD));
    return units;
}

string getUnitPrefix(const string &aUnit) {
    // If it is not a base unit, then it is a prefixed unit
    if ((aUnit != "") && !stringExists(aUnit, BASE_UNITS)) {
        // Converts to wstring because symbols like "micro" and "omega" occupy more than one character in ANSI encoding
        wstring str = QString::fromStdString(aUnit).toStdWString();
        // wstring uses wchar_t, now each special character will occupy only one string position
        // Gets the first character/symbol
        str = str.substr(0, 1);

        return QString::fromStdWString(str).toStdString();
    }
    // Unit without prefix
    return "";
}

double getUnitMultiple(const string &aUnit) {
    string prefix = getUnitPrefix(aUnit);
    if (prefix != "") {
        // Prefixed unit
        if (prefix == PREFIX_MILI) {
            return 1E-3;
        } else if (prefix == PREFIX_MICRO) {
            return 1E-6;
        } else if (prefix == PREFIX_NANO) {
            return 1E-9;
        } else if (prefix == PREFIX_KILO) {
            return 1E3;
        } else if (prefix == PREFIX_MEGA) {
            return 1E6;
        } else if (prefix == PREFIX_GIGA) {
            return 1E9;
        } else {
            // Unrecognized prefix
            throw "Prefixo '" + prefix + "' não reconhecido.";
            return 0;
        }
    }
    // Unit without prefix
    return 1;
}

string getProperPrefix(double aValue) {
    if (aValue == 0) return "";

    string result = PREFIX_NANO;
    if (aValue > 1E-6)
        result = PREFIX_MICRO;
    if (aValue > 1E-3)
        result = PREFIX_MILI;
    if (aValue > 1)
        result = "";
    if (aValue > 1E3)
        result = PREFIX_KILO;
    if (aValue > 1E6)
        result = PREFIX_MEGA;
    if (aValue > 1E9)
        result = PREFIX_GIGA;

    return result;
}

double multiplyUnit(double aValue, const string &aUnit) {
    return aValue * getUnitMultiple(aUnit);
}

double divideUnit(double aValue, const string &aUnit) {
    return aValue / getUnitMultiple(aUnit);
}

string formatValue(double aValue, int aDecimalPlaces) {
    ostringstream strBuffer;

    if (aValue < 1000000) {
        // Defines precision to fixed at given decimal places and converts the value to string
        strBuffer << fixed << setprecision(aDecimalPlaces) << aValue;
        string valText = strBuffer.str();

        // Gets the last non-zero digit and crops the number there
        size_t end = valText.find_last_not_of('0') + 1;
        valText = valText.erase(end);

        // Removes the last character if it's a period
        end = valText.length() - 1;
        if (valText.at(end) == '.') {
            valText = valText.erase(end);
        }
        return valText;
    } else {
        // Greater than or equal 1 000 000, output in scientific notation
        strBuffer << scientific << setprecision(aDecimalPlaces) << aValue ;
        return strBuffer.str();
    }
}

string formatOutputForInterchange(const string &aValue, const string &aUnit) {
    string prefix = getUnitPrefix(aUnit);
    if (prefix == "μ") {
        prefix = "u";
    }
    return aValue + prefix;
}

double reactanceToInductance(double aReactance, double aFrequency) {
    return aReactance / (2 * PI * aFrequency);
}

double inductanceToReactance(double aInductance, double aFrequency) {
    return 2 * PI * aFrequency * aInductance;
}

double reactanceToCapacitance(double aReactance, double aFrequency) {
    return 1 / (2 * PI * aFrequency * aReactance);
}

double capacitanceToReactance(double aCapacitance, double aFrequency) {
    return 1 / (2 * PI * aFrequency * aCapacitance);
}
