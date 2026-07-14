#include "dashboardcanvas.h"

DashboardCanvas::DashboardCanvas(QWidget *parent) : QWidget(parent)
{
    const int minWidth = m_columns * kMinCellWidth + m_spacing * (m_columns + 1);
    setMinimumWidth(minWidth);

    loadData();
}

void DashboardCanvas::loadData()
{
    buildWidgets();

    // get widgetModels of the User (it´s WidgetList)
    for (const WidgetModel &d : m_data) {
        auto *container = new WidgetContainer(d.wName, this);
        container->show();
        m_containers.append(container);

        connect(container, &WidgetContainer::dragFinished,
            this, &DashboardCanvas::onDragFinished);
    }

    reflow();
}

void DashboardCanvas::reflow()
{
    if (m_data.isEmpty()) return;

    const int cellWidth = ((width() - kVScrollbarReserve) - m_spacing * (m_columns + 1)) / m_columns;

    for (int i = 0; i < m_data.size(); ++i) {
        const WidgetModel &model = m_data.at(i);

        const int x = m_spacing + model.wPosX * (cellWidth + m_spacing);
        const int y = m_spacing + model.wPosY * (m_cellHeight + m_spacing);
        const int w = model.wWidth  * cellWidth     + (model.wWidth  - 1) * m_spacing;
        const int h = model.wHeight * m_cellHeight  + (model.wHeight - 1) * m_spacing;

        m_containers.at(i)->setGeometry(x, y, w, h);
    }
}


//Important: changes at reflow must also changed in cellAt!
QPoint DashboardCanvas::cellAt(const QPoint &canvasPos) const
{
    const int cellWidth = (width() - m_spacing * (m_columns + 1)) / m_columns;

    int column = canvasPos.x() / (cellWidth + m_spacing);
    int row    = canvasPos.y() / (m_cellHeight + m_spacing);

    column = qBound(0, column, m_columns - 1);
    row    = qMax(0, row);

    return QPoint(column, row);
}

QRect DashboardCanvas::rectOf(const WidgetModel &m) const
{
    return QRect(m.wPosX, m.wPosY, m.wWidth, m.wHeight);
}

int DashboardCanvas::indexAt(int col, int row) const
{
    for (int i = 0; i < m_data.size(); ++i) {
        if (m_data.at(i).wPosX == col && m_data.at(i).wPosY == row)
            return i;
    }
    return -1; // Zelle ist leer
}

void DashboardCanvas::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    reflow();
}

void DashboardCanvas::onDragFinished(WidgetContainer *widget, const QPoint &globalPos)
{
    const int draggedIndex = m_containers.indexOf(widget);
    if (draggedIndex == -1) return; // sollte nie passieren

    const QPoint localPos = mapFromGlobal(globalPos);
    const QPoint cell = cellAt(localPos);

    const int targetIndex = indexAt(cell.x(), cell.y());

    if (targetIndex != -1 && targetIndex != draggedIndex) {
        std::swap(m_data[draggedIndex].wPosX, m_data[targetIndex].wPosX);
        std::swap(m_data[draggedIndex].wPosY, m_data[targetIndex].wPosY);
    }

    reflow(); // setzt alle Widgets auf ihre (ggf. neuen) Positionen
}


/* Test Func */

void DashboardCanvas::buildWidgets() {
    // Function to fill m_data with users Widgetlist or take usersList - why dublicate given list?

    WidgetModel model1;
    model1.wName = "widget_1";
    model1.wPosX = 0;
    model1.wPosY = 0;
    model1.wWidth = 1;
    model1.wHeight = 1;
    m_data.append(model1);

    WidgetModel model2;
    model2.wName = "widget_2";
    model2.wPosX = 1;
    model2.wPosY = 0;
    model2.wWidth = 1;
    model2.wHeight = 1;
    m_data.append(model2);

    WidgetModel model3;
    model3.wName = "widget_3";
    model3.wPosX = 2;
    model3.wPosY = 0;
    model3.wWidth = 1;
    model3.wHeight = 1;
    m_data.append(model3);

    WidgetModel model4;
    model4.wName = "widget_4";
    model4.wPosX = 0;
    model4.wPosY = 1;
    model4.wWidth = 1;
    model4.wHeight = 12;
    m_data.append(model4);

    WidgetModel model5;
    model5.wName = "widget_5";
    model5.wPosX = 1;
    model5.wPosY = 2;
    model5.wWidth = 2;
    model5.wHeight = 2;
    m_data.append(model5);

}