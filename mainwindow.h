#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QValidator>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/* Defines the direction/way of conversion, used for both the capacitor and the inductor:
   cwReactanceToValue -> To convert an Reactance value to Capacitance/Inductance
   cwValueToReactance -> To convert Capacitance/Inductance to an Reactance value */
enum conversion_way_t {cwReactanceToValue, cwValueToReactance};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_cbInductorResultUnit_currentTextChanged(const QString &arg1);
    void on_cbCapacitorResultUnit_currentTextChanged(const QString &arg1);
    void on_btInductorAlternate_clicked();
    void on_btCapacitorAlternate_clicked();
    void on_btInductorCopy_clicked();
    void on_btCapacitorCopy_clicked();
    /* The following slots are connected to the input fields signals in the MainWindow constructor */
    void on_inductorInput_changed(const QString &arg1);
    void on_capacitorInput_changed(const QString &arg1);
    /* */
    void on_btAbout_clicked();

    void on_btHelp_clicked();

private:
    /* Result of the Inductor conversion */
    double fInductorResult;
    /* Actual direction/way of conversion (Reactance to Inductance OR Inductance to Reactance) */
    conversion_way_t fInductorConversion;
    /* Result of the Capacitor conversion */
    double fCapacitorResult;
    /* Actual direction/way of conversion (Reactance to Capacitance OR Capacitance to Reactance) */
    conversion_way_t fCapacitorConversion;
    /* Validator for input fields */
    QRegExpValidator *fInputValidator;
    /* Methods to arrange the widgets on screen according to the conversion way */
    void setInductorConversion(conversion_way_t aValue);
    void setCapacitorConversion(conversion_way_t aValue);
    /* */
    /* Conversion routines */
    void calculateInductor();
    void calculateCapacitor();
    void displayInductorResult(QString aUnit);
    void displayCapacitorResult(QString aUnit);
    /* */
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
