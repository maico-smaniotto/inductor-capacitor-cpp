#ifndef ELECTRICAL_UTILS_H
#define ELECTRICAL_UTILS_H

#endif // ELECTRICAL_UTILS_H

#include <QStringList>
#include <stdlib.h>

#include <string>

using namespace std;

/* Unit prefixes */
const string PREFIX_KILO  = "k";
const string PREFIX_MEGA  = "M";
const string PREFIX_GIGA  = "G";
const string PREFIX_MILI  = "m";
const string PREFIX_MICRO = "μ";
const string PREFIX_NANO  = "n";

/* Base units */
const string OHM   = "Ω";
const string HENRY = "H";
const string FARAD = "F";
const string HERTZ = "Hz";
//const string BASE_UNITS[] = {OHM, HENRY, FARAD, HERTZ};
const QStringList BASE_UNITS = QStringList() << QString::fromStdString(OHM)
                                             << QString::fromStdString(HENRY)
                                             << QString::fromStdString(FARAD)
                                             << QString::fromStdString(HERTZ);

/* Derived/prefixed units */
const string KILO_OHM    = PREFIX_KILO + OHM;
const string MEGA_OHM    = PREFIX_MEGA + OHM;
const string GIGA_OHM    = PREFIX_GIGA + OHM;

const string MILI_HENRY  = PREFIX_MILI  + HENRY;
const string MICRO_HENRY = PREFIX_MICRO + HENRY;
const string NANO_HENRY  = PREFIX_NANO  + HENRY;

const string MILI_FARAD  = PREFIX_MILI  + FARAD;
const string MICRO_FARAD = PREFIX_MICRO + FARAD;
const string NANO_FARAD  = PREFIX_NANO  + FARAD;

const string KILO_HERTZ = PREFIX_KILO + HERTZ;
const string MEGA_HERTZ = PREFIX_MEGA + HERTZ;
const string GIGA_HERTZ = PREFIX_GIGA + HERTZ;

bool stringExists(const string &aStr, const QStringList &aList);

/* Functions that return the list of units to fill the comboboxes */
QStringList getFrequencyUnits();

QStringList getReactanceUnits();

QStringList getInductanceUnits();

QStringList getCapacitanceUnits();

/* Returns the prefix of the unit, if it is prefixed, otherwise returns an empty string
   Ex: 'kHz', returns 'k' */
string getUnitPrefix(const string &aUnit);

/* Returns the multiple value of the unit accordingly to its prefix
   Ex: 'kHz', prefix k equals 10E3 */
double getUnitMultiple(const string &aUnit);

/* Returns the most appropriate prefix for a given value, using engineering notation */
string getProperPrefix(double aValue);

/* Multiplies a value by the prefix weight of the given unit
   Ex: 5 kHz = 5000 Hz
       multiplyUnit(5, "kHz"); returns 5000 */
double multiplyUnit(double aValue, const string &aUnit);

/* Divides a value by the prefix weight of the given unit
   Ex: 5000 Hz = 5 kHz
       divideUnit(5000, "kHz"); returns 5 */
double divideUnit(double aValue, const string &aUnit);

/* Formats a value to given decimal places */
string formatValue(double aValue, int aDecimalPlaces);

/* Formats output for interchange with simulation softwares
   Replaces comma by period, utilizes only the unit prefix and replaces 'μ' by 'u' */
string formatOutputForInterchange(const string &aValue, const string &aUnit);

/* Conversion functions */
double reactanceToInductance(double aReactance, double aFrequency);
double inductanceToReactance(double aInductance, double aFrequency);
double reactanceToCapacitance(double aReactance, double aFrequency);
double capacitanceToReactance(double aCapacitance, double aFrequency);
