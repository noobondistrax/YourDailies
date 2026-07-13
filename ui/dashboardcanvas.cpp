#include "dashboardcanvas.h"

DashboardCanvas::DashboardCanvas(QWidget *parent) : QWidget(parent)
{
    loadData();
}

void DashboardCanvas::loadData()
{

    // Function to fill m_data with users Widgetlist or take usersList - why dublicate given list?
    for (int i = 0 ; i < 5 ; i++) {
        WidgetModel model;
        model.wName = "widget_" + QString::number(i);
        model.wPosX = 0;
        model.wPosY = i;
        model.wWidth = 1;
        model.wHeight = 1;
        m_data.append(model);
    }

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

    const int totalSpacing = m_spacing * (m_columns + 1);
    const int cellWidth = (width() - totalSpacing) / m_columns;

    for (int i = 0; i < m_data.size(); ++i) {
        const WidgetModel &d = m_data.at(i);

        int x = m_spacing + d.wPosX * (cellWidth + m_spacing);                  // position x of the Widgeet
        int y = m_spacing + d.wPosY * (m_cellHeight + m_spacing);               // position y of the Widget
        int w = d.wWidth * cellWidth + (d.wWidth - 1) * m_spacing;              // width of the widget (in "span" - fields of the grid
        int h = d.wHeight * m_cellHeight + (d.wHeight - 1) * m_spacing;         // height of the widget (in "span" - fields of the grid

        m_containers.at(i)->setGeometry(x, y, w, h);
    }
}

void DashboardCanvas::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    reflow();
}

void DashboardCanvas::onDragFinished(WidgetContainer *widget, const QPoint &globalPos)
{
    Q_UNUSED(widget);
    Q_UNUSED(globalPos);
    // Schritt 2: noch nichts zu tun — Widget hat sich in mouseReleaseEvent
    // schon selbst zurückgesetzt. Zielzellen-Logik folgt in Schritt 3.
}