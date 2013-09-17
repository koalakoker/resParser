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

private:
    Ui::FunctionWindow *ui;

public slots:
    void functionListPopulate(QStringList list);
};

#endif // FUNCTIONWINDOW_H
