#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class LoginDialog;
class MainWindow : public QMainWindow
{
    Q_OBJECT

    LoginDialog*    pLoginDialog;

public:
    static  MainWindow* pWin;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void    save();
    void    restore();

public slots:
    void    onLogout();
    void    onBlock();
    void    onTabChange(int toIndex);

protected:
    void    closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
