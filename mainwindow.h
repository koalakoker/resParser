#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "docklistview.h"
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
    void updateOutputFormatSetting(void);
    void updateOutputPaneAndPreview(void);
    void addStrToLineInput(QString str);

    void on_line_input_textChanged(const QString &arg1);
    void on_actionFunctions_toggled(bool arg1);
    void on_actionAbout_activated();
    void on_actionAuto_toggled(bool arg1);
    void on_actionFloating_toggled(bool arg1);
    void on_actionScientific_toggled(bool arg1);
    void on_actionHexadecimal_toggled(bool arg1);
    void on_action_Save_triggered();
    void on_action_Load_triggered();

    void closeEvent (QCloseEvent* event);

    // Dock
    void dockVisibilityChange(bool visible);
    void on_actionVariables_toggled(bool arg1);
    void on_actionUser_Functions_toggled(bool arg1);

    void variableUpdates(void);
    void userDefinedFunctionUpdates(void);

    void clearSelected(QString str);
    void clearHistorySlot(void);

    void on_actionClear_Hystory_triggered();

    void on_actionClear_All_triggered();

    void on_action_Import_raw_data_triggered();

    void on_action_Wiki_triggered();

private:
    Ui::MainWindow *ui;
    DockListView m_dockListVariables;
    DockListView m_dockListUserDefinedFunctions;
    DockListView m_dockListBuiltinFunctions;
    CommandMngrClass m_cmdMngr;

    bool m_isExtended;
};

#endif // MAINWINDOW_H
