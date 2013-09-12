#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "parserclass.h"
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

private:
    Ui::MainWindow *ui;

    ParserClass parser;
    CommandMngrClass cmdMngr;
};

#endif // MAINWINDOW_H
