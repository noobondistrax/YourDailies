#ifndef DASHBOARDCANVAS_H
#define DASHBOARDCANVAS_H

#include <QWidget>
#include <QLabel>
#include <QList>
#include "widgetModel.h"
#include "widgetcontainer.h"

class DashboardCanvas : public QWidget {
    Q_OBJECT
public:
    explicit DashboardCanvas(QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void onDragFinished(WidgetContainer *widget, const QPoint &globalPos);

private:
    void loadData();
    void reflow();

    QList<WidgetModel> m_data;             // Positions-/Größenlogik
    QList<WidgetContainer*> m_containers; // visuelle Widgets, gleicher Index wie m_data

    int m_columns = 4;
    int m_cellHeight = 100;
    int m_spacing = 8;
};

#endif // DASHBOARDCANVAS_H
