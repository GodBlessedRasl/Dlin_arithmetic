#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void to_poliz(const std::string& str);
    void add_variables(const std::string& str);
    std::string dbltostr(double x);
    bool Check(std::string s);
private slots:
    void on_ENTER_clicked();
    void on_LOAD_clicked();

    void on_CLEAR_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
