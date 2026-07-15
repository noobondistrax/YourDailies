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
    explicit DashboardCanvas(QList<WidgetModel> &widgets, QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void onDragFinished(WidgetContainer *widget, const QPoint &globalPos);

private:
    void loadData();
    void reflow();
    void updateContentHeight();
    int findCollision(const QRect &rect, int excludeIndex) const;
    QPoint directionOf(const QPoint &from, const QPoint &to) const;
    bool tryPush(int index, const QPoint &direction);
    QPoint findFirstFit(int excludeIndex, int w, int h) const;
    void moveWidgetTo(int draggedIndex, QPoint targetCell);
    void placeNewWidget(WidgetModel&);
    WidgetContainer* createContainerFor(const WidgetModel &model);
    void createContainersFromExistingData();

    QRect rectOf(const WidgetModel &m) const;
    QPoint cellAt(const QPoint &canvasPos) const;
    int indexAt(int col, int row) const;

    QList<WidgetModel> &m_data;
    QList<WidgetContainer*> m_containers;           // visuelle Widgets, gleicher Index wie m_data

    static constexpr int m_columns = 4;             // sum of colums
    static constexpr int m_cellHeight = 100;
    static constexpr int m_spacing = 8;             // space between widgets
    static constexpr int kVScrollbarReserve = 20;   // reserve if a scrollbar appears
    static constexpr int kMinCellWidth = 160;       // Pixel, Design-decision



    void buildWidgets(); // TestFunc
};

#endif // DASHBOARDCANVAS_H
