#ifndef FUNCTIONWINDOW_H
#define FUNCTIONWINDOW_H

#include <QMainWindow>

namespace Ui {
class FunctionWindow;
}

class FunctionWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit FunctionWindow(QWidget *parent = 0);
    ~FunctionWindow();

    void functionListPopulate(QStringList list);
    
private:
    Ui::FunctionWindow *ui;
};

#endif // FUNCTIONWINDOW_H
