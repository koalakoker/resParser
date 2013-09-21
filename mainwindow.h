#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "functionwindow.h"
#include "commandmngrclass.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_line_input_returnPressed();
    void keyUpPress();
    void keyDownPress();
    void keyOperatorPress();

    void on_line_input_textChanged(const QString &arg1);
    void on_actionFunctions_toggled(bool arg1);
    void on_actionAbout_activated();
    void on_actionAuto_toggled(bool arg1);
    void on_actionFloating_toggled(bool arg1);
    void on_actionScientific_toggled(bool arg1);

    void updateOutputFormatSetting(void);

private:
    Ui::MainWindow *ui;
    FunctionWindow m_funcWin;
    CommandMngrClass m_cmdMngr;
};

#endif // MAINWINDOW_H
