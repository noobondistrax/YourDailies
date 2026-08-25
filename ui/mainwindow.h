#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "appcontext.h"
#include "dashboardpage.h"
#include "widgetModel.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(AppContext& appContext, QWidget *parent = nullptr);
    ~MainWindow() override;


private:
    Ui::MainWindow *ui;
    AppContext& m_context;

    void progStart(AppContext& context);
    void showDashboard();

};
#endif // MAINWINDOW_H
