#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "electrical_utils.h"
#include "aboutwindow.h"
#include "helpwindow.h"
#include <QClipboard>

#define DECIMAL_PLACES 6

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Does not allow resize and disables the Maximize button
    setFixedSize(width(), height());

    //QApplication::setStyle(QString("Fusion"));

    fInductorConversion  = cwReactanceToValue;
    fCapacitorConversion = cwReactanceToValue;
    fInductorResult      = -1.0;
    fCapacitorResult     = -1.0;

    // Validator to allow only numbers (parent = this : delete not needed)
    fInputValidator = new QRegExpValidator(QRegExp("[0-9]*[\\.][0-9]{10}"), this);

    ui->edInductorFrequencyVal->setValidator(fInputValidator);
    ui->edInductorInputVal->setValidator(fInputValidator);
    ui->edCapacitorFrequencyVal->setValidator(fInputValidator);
    ui->edCapacitorInputVal->setValidator(fInputValidator);

    /* Connect the signals "textChanged" (QEditLine) and "currentTextChanged" (QComboBox)
       to the slot/method that triggers the calculation routine.
       Done this way to reuse the same method for both signals */
    // Signals for inductor fields
    connect(ui->edInductorFrequencyVal,   SIGNAL(textChanged(QString)),        this, SLOT(on_inductorInput_changed(QString)));
    connect(ui->cbInductorFrequencyUnit,  SIGNAL(currentTextChanged(QString)), this, SLOT(on_inductorInput_changed(QString)));
    connect(ui->edInductorInputVal,       SIGNAL(textChanged(QString)),        this, SLOT(on_inductorInput_changed(QString)));
    connect(ui->cbInductorInputUnit,      SIGNAL(currentTextChanged(QString)), this, SLOT(on_inductorInput_changed(QString)));
    // Signals for capacitor fields
    connect(ui->edCapacitorFrequencyVal,  SIGNAL(textChanged(QString)),        this, SLOT(on_capacitorInput_changed(QString)));
    connect(ui->cbCapacitorFrequencyUnit, SIGNAL(currentTextChanged(QString)), this, SLOT(on_capacitorInput_changed(QString)));
    connect(ui->edCapacitorInputVal,      SIGNAL(textChanged(QString)),        this, SLOT(on_capacitorInput_changed(QString)));
    connect(ui->cbCapacitorInputUnit,     SIGNAL(currentTextChanged(QString)), this, SLOT(on_capacitorInput_changed(QString)));

    QStringList units = getFrequencyUnits();

    ui->cbInductorFrequencyUnit->addItems(units);
    ui->edInductorFrequencyVal->setText("60");
    ui->cbInductorFrequencyUnit->setCurrentText(QString::fromStdString(HERTZ));

    ui->cbCapacitorFrequencyUnit->addItems(units);
    ui->edCapacitorFrequencyVal->setText("60");
    ui->cbCapacitorFrequencyUnit->setCurrentText(QString::fromStdString(HERTZ));

    setInductorConversion(fInductorConversion);
    setCapacitorConversion(fCapacitorConversion);

    ui->edInductorInputVal->setText("");
    ui->cbInductorInputUnit->setCurrentText(QString::fromStdString(OHM));
    ui->edCapacitorInputVal->setText("");
    ui->cbCapacitorInputUnit->setCurrentText(QString::fromStdString(OHM));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_cbInductorResultUnit_currentTextChanged(const QString &arg1)
{
    displayInductorResult(arg1);
}

void MainWindow::on_cbCapacitorResultUnit_currentTextChanged(const QString &arg1)
{
    displayCapacitorResult(arg1);
}

void MainWindow::on_btInductorAlternate_clicked()
{
    QString resultVal  = ui->edInductorResultVal->text();
    QString resultUnit = ui->cbInductorResultUnit->currentText();

    if (fInductorConversion == cwReactanceToValue)
        setInductorConversion(cwValueToReactance);
    else
        setInductorConversion(cwReactanceToValue);

    ui->edInductorInputVal->setText(resultVal);
    ui->cbInductorInputUnit->setCurrentText(resultUnit);

    calculateInductor();
}

void MainWindow::on_btCapacitorAlternate_clicked()
{
    QString resultVal  = ui->edCapacitorResultVal->text();
    QString resultUnit = ui->cbCapacitorResultUnit->currentText();

    if (fCapacitorConversion == cwReactanceToValue)
        setCapacitorConversion(cwValueToReactance);
    else
        setCapacitorConversion(cwReactanceToValue);

    ui->edCapacitorInputVal->setText(resultVal);
    ui->cbCapacitorInputUnit->setCurrentText(resultUnit);

    calculateCapacitor();
}

void MainWindow::on_btInductorCopy_clicked()
{
    string text = formatOutputForInterchange(ui->edInductorResultVal->text().toStdString(), ui->cbInductorResultUnit->currentText().toStdString());
    QApplication::clipboard()->setText(QString::fromStdString(text));
}

void MainWindow::on_btCapacitorCopy_clicked()
{
    string text = formatOutputForInterchange(ui->edCapacitorResultVal->text().toStdString(), ui->cbCapacitorResultUnit->currentText().toStdString());
    QApplication::clipboard()->setText(QString::fromStdString(text));
}

void MainWindow::MainWindow::on_inductorInput_changed(const QString &arg1)
{
    calculateInductor();
}

void MainWindow::MainWindow::on_capacitorInput_changed(const QString &arg1)
{
    calculateCapacitor();
}

void MainWindow::setInductorConversion(conversion_way_t aValue)
{
    fInductorConversion = aValue;
    switch (aValue) {
        case cwReactanceToValue:
            ui->lbInductorInput->setText(tr("Reatância"));
            ui->cbInductorInputUnit->clear();
            ui->cbInductorInputUnit->addItems(getReactanceUnits());
            ui->lbInductorResult->setText(tr("Indutância"));
            ui->cbInductorResultUnit->clear();
            ui->cbInductorResultUnit->addItems(getInductanceUnits());
            break;
        case cwValueToReactance:
            ui->lbInductorInput->setText(tr("Indutância"));
            ui->cbInductorInputUnit->clear();
            ui->cbInductorInputUnit->addItems(getInductanceUnits());
            ui->lbInductorResult->setText(tr("Reatância"));
            ui->cbInductorResultUnit->clear();
            ui->cbInductorResultUnit->addItems(getReactanceUnits());
    }
}

void MainWindow::setCapacitorConversion(conversion_way_t aValue)
{
    fCapacitorConversion = aValue;
    switch (aValue) {
        case cwReactanceToValue:
            ui->lbCapacitorInput->setText(tr("Reatância"));
            ui->cbCapacitorInputUnit->clear();
            ui->cbCapacitorInputUnit->addItems(getReactanceUnits());
            ui->lbCapacitorResult->setText(tr("Capacitância"));
            ui->cbCapacitorResultUnit->clear();
            ui->cbCapacitorResultUnit->addItems(getCapacitanceUnits());
            break;
        case cwValueToReactance:
            ui->lbCapacitorInput->setText(tr("Capacitância"));
            ui->cbCapacitorInputUnit->clear();
            ui->cbCapacitorInputUnit->addItems(getCapacitanceUnits());
            ui->lbCapacitorResult->setText(tr("Reatância"));
            ui->cbCapacitorResultUnit->clear();
            ui->cbCapacitorResultUnit->addItems(getReactanceUnits());
    }
}

void MainWindow::calculateInductor()
{
    try {
        QString frequencyText = ui->edInductorFrequencyVal->text();
        if (frequencyText == "")
            throw std::invalid_argument("Frequência não informada");

        QString inputText = ui->edInductorInputVal->text();
        if (inputText == "")
            throw std::invalid_argument("Valor não informado");

        double frequencyVal = frequencyText.toDouble();
        frequencyVal        = multiplyUnit(frequencyVal, ui->cbInductorFrequencyUnit->currentText().toStdString());
        double inputVal     = inputText.toDouble();
        inputVal            = multiplyUnit(inputVal, ui->cbInductorInputUnit->currentText().toStdString());

        switch (fInductorConversion) {
            case cwReactanceToValue:
                fInductorResult = reactanceToInductance(inputVal, frequencyVal);
                break;
            case cwValueToReactance:
                fInductorResult = inductanceToReactance(inputVal, frequencyVal);
        }
    } catch (const std::invalid_argument &e) {
        fInductorResult = -1;
    }
    displayInductorResult("");
}

void MainWindow::calculateCapacitor()
{
    try {
        QString frequencyText = ui->edCapacitorFrequencyVal->text();
        if (frequencyText == "")
            throw std::invalid_argument("Frequência não informada");

        QString inputText = ui->edCapacitorInputVal->text();
        if (inputText == "")
            throw std::invalid_argument("Valor não informado");

        double frequencyVal = frequencyText.toDouble();
        frequencyVal        = multiplyUnit(frequencyVal, ui->cbCapacitorFrequencyUnit->currentText().toStdString());
        double inputVal     = inputText.toDouble();
        inputVal            = multiplyUnit(inputVal, ui->cbCapacitorInputUnit->currentText().toStdString());

        switch (fCapacitorConversion) {
            case cwReactanceToValue:
                fCapacitorResult = reactanceToCapacitance(inputVal, frequencyVal);
                break;
            case cwValueToReactance:
                fCapacitorResult = capacitanceToReactance(inputVal, frequencyVal);
        }
    } catch (const std::invalid_argument &e) {
        fCapacitorResult = -1;
    }
    displayCapacitorResult("");
}

void MainWindow::displayInductorResult(QString aUnit)
{
    if (aUnit == "") {
        QStringList units;
        switch (fInductorConversion) {
            case cwReactanceToValue:
                if (fInductorResult > 0) {
                    aUnit = QString::fromStdString(getProperPrefix(fInductorResult) + HENRY);
                    units = getInductanceUnits();
                    // If there is no inductance unit with the returned prefix, then considers the base unit HENRY
                    if (units.indexOf(aUnit) < 0)
                        aUnit = QString::fromStdString(HENRY);
                } else {
                    // Zero or negative value, then shows the base unit HENRY
                    aUnit = QString::fromStdString(HENRY);
                }
                break;
            case cwValueToReactance:
                if (fInductorResult > 0) {
                    aUnit = QString::fromStdString(getProperPrefix(fInductorResult) + OHM);
                    units = getReactanceUnits();
                    // If there is no reactance unit with the returned prefix, then considers the base unit OHM
                    if (units.indexOf(aUnit) < 0)
                        aUnit = QString::fromStdString(OHM);
                } else {
                    // Zero or negative value, then shows the base unit OHM
                    aUnit = QString::fromStdString(OHM);
                }
        }
    }
    ui->cbInductorResultUnit->setCurrentText(aUnit);

    if (fInductorResult >= 0) {
        // Applyes the unit conversion
        double value = divideUnit(fInductorResult, aUnit.toStdString());
        // Format to display limited decimal places
        ui->edInductorResultVal->setText(QString::fromStdString(formatValue(value, DECIMAL_PLACES)));
    } else {
        // Negative value is not shown
        ui->edInductorResultVal->setText("");
    }
}

void MainWindow::displayCapacitorResult(QString aUnit)
{
    if (aUnit == "") {
        QStringList units;
        switch (fCapacitorConversion) {
            case cwReactanceToValue:
                if (fCapacitorResult > 0) {
                    aUnit = QString::fromStdString(getProperPrefix(fCapacitorResult) + FARAD);
                    units = getCapacitanceUnits();
                    // If there is no capacitance unit with the returned prefix, then considers the base unit FARAD
                    if (units.indexOf(aUnit) < 0)
                        aUnit = QString::fromStdString(FARAD);
                } else {
                    // Zero or negative value, then shows the base unit FARAD
                    aUnit = QString::fromStdString(FARAD);
                }
                break;
            case cwValueToReactance:
                if (fCapacitorResult > 0) {
                    aUnit = QString::fromStdString(getProperPrefix(fCapacitorResult) + OHM);
                    units = getReactanceUnits();
                    // If there is no reactance unit with the returned prefix, then considers the base unit OHM
                    if (units.indexOf(aUnit) < 0)
                        aUnit = QString::fromStdString(OHM);
                } else {
                    // Zero or negative value, then shows the base unit OHM
                    aUnit = QString::fromStdString(OHM);
                }
        }
    }
    ui->cbCapacitorResultUnit->setCurrentText(aUnit);

    if (fCapacitorResult >= 0) {
        // Applyes the unit conversion
        double value = divideUnit(fCapacitorResult, aUnit.toStdString());
        // Format to display limited decimal places
        ui->edCapacitorResultVal->setText(QString::fromStdString(formatValue(value, DECIMAL_PLACES)));
    } else {
        // Negative value is not shown
        ui->edCapacitorResultVal->setText("");
    }
}

void MainWindow::on_btAbout_clicked()
{
    AboutWindow *aboutWindow = new AboutWindow();
    aboutWindow->setAttribute(Qt::WA_DeleteOnClose);
    aboutWindow->setModal(true);
    aboutWindow->show();
}

void MainWindow::on_btHelp_clicked()
{
    if (HelpWindow::helpWindow == nullptr) {
        HelpWindow::helpWindow = new HelpWindow();
        HelpWindow::helpWindow->setAttribute(Qt::WA_DeleteOnClose);
    }
    HelpWindow::helpWindow->show();
    HelpWindow::helpWindow->activateWindow();
}
