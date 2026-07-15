#include "dashboardcanvas.h"


DashboardCanvas::DashboardCanvas(QList<WidgetModel> &widgets, QWidget *parent)
    : QWidget(parent), m_data(widgets)
{
    const int minWidth = m_columns * kMinCellWidth + m_spacing * (m_columns + 1);
    setMinimumWidth(minWidth);

    createContainersFromExistingData();
    reflow();
}


void DashboardCanvas::reflow()
{
    if (m_data.isEmpty()) return;

    const int cellWidth = (width() - m_spacing * (m_columns + 1)) / m_columns;

    for (int i = 0; i < m_data.size(); ++i) {
        const WidgetModel &model = m_data.at(i);

        const int x = m_spacing + model.wPosX * (cellWidth + m_spacing);
        const int y = m_spacing + model.wPosY * (m_cellHeight + m_spacing);
        const int w = model.wWidth  * cellWidth     + (model.wWidth  - 1) * m_spacing;
        const int h = model.wHeight * m_cellHeight  + (model.wHeight - 1) * m_spacing;

        m_containers.at(i)->setGeometry(x, y, w, h);
    }

    updateContentHeight();
}

// Func to set new program height and commit to Qt to calc if Scrollbar is needed
void DashboardCanvas::updateContentHeight()
{
    int maxBottomRow = 0;
    for (const WidgetModel &model : m_data) {
        const int bottomRow = model.wPosY + model.wHeight;
        maxBottomRow = qMax(maxBottomRow, bottomRow);
    }

    const int requiredHeight = m_spacing + maxBottomRow * (m_cellHeight + m_spacing) + m_spacing + m_cellHeight;
    setMinimumHeight(requiredHeight);
}

int DashboardCanvas::findCollision(const QRect &rect, int excludeIndex) const
{
    for (int i = 0; i < m_data.size(); ++i) {
        if (i == excludeIndex) continue;
        if (rectOf(m_data.at(i)).intersects(rect))
            return i;
    }
    return -1;
}

QPoint DashboardCanvas::directionOf(const QPoint &from, const QPoint &to) const
{
    const int dx = to.x() - from.x();
    const int dy = to.y() - from.y();

    if (qAbs(dx) >= qAbs(dy))
        return QPoint(dx >= 0 ? -1 : 1, 0);
    else
        return QPoint(0, dy >= 0 ? -1 : 1);
}

bool DashboardCanvas::tryPush(int index, const QPoint &direction)
{
    WidgetModel &model = m_data[index];
    QPoint pos(model.wPosX, model.wPosY);

    for (int step = 0; step < m_columns + 50; ++step) {
        pos += direction;

        if (pos.x() < 0 || pos.x() + model.wWidth > m_columns || pos.y() < 0)
            return false;

        const QRect candidate(pos.x(), pos.y(), model.wWidth, model.wHeight);
        if (findCollision(candidate, index) == -1) {
            model.wPosX = pos.x();
            model.wPosY = pos.y();
            return true;
        }
    }
    return false;
}

QPoint DashboardCanvas::findFirstFit(int excludeIndex, int w, int h) const
{
    for (int row = 0; ; ++row) {
        for (int col = 0; col <= m_columns - w; ++col) {
            const QRect candidate(col, row, w, h);
            if (findCollision(candidate, excludeIndex) == -1)
                return QPoint(col, row);
        }
    }
}

void DashboardCanvas::moveWidgetTo(int draggedIndex, QPoint targetCell)
{
    WidgetModel &dragged = m_data[draggedIndex];

    targetCell.setX(qBound(0, targetCell.x(), m_columns - dragged.wWidth));

    const QPoint oldPos(dragged.wPosX, dragged.wPosY);
    const QPoint direction = directionOf(oldPos, targetCell);
    const QRect targetRect(targetCell.x(), targetCell.y(), dragged.wWidth, dragged.wHeight);

    QList<int> blockers;
    for (int i = 0; i < m_data.size(); ++i) {
        if (i == draggedIndex) continue;
        if (rectOf(m_data.at(i)).intersects(targetRect))
            blockers.append(i);
    }

    dragged.wPosX = targetCell.x();
    dragged.wPosY = targetCell.y();

    for (int blockerIndex : blockers) {
        if (!tryPush(blockerIndex, direction)) {
            const WidgetModel &blocker = m_data.at(blockerIndex);
            const QPoint fallback = findFirstFit(blockerIndex, blocker.wWidth, blocker.wHeight);
            m_data[blockerIndex].wPosX = fallback.x();
            m_data[blockerIndex].wPosY = fallback.y();
        }
    }

    reflow();
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
    if (draggedIndex == -1) return;

    const QPoint localPos = mapFromGlobal(globalPos);
    const QPoint targetCell = cellAt(localPos);

    moveWidgetTo(draggedIndex, targetCell);
}

WidgetContainer* DashboardCanvas::createContainerFor(const WidgetModel &model)
{
    auto *container = new WidgetContainer(model.wName, this);
    container->show();
    m_containers.append(container);

    connect(container, &WidgetContainer::dragFinished,
            this, &DashboardCanvas::onDragFinished);

    return container;
}

void DashboardCanvas::createContainersFromExistingData()
{
    for (const WidgetModel &model : m_data) {
        createContainerFor(model);
    }
}

void DashboardCanvas::placeNewWidget(WidgetModel &model)
{
    const QPoint newWidgetCoords = findFirstFit(-1, model.wWidth, model.wHeight);
    model.wPosX = newWidgetCoords.x();
    model.wPosY = newWidgetCoords.y();

    m_data.append(model);
    createContainerFor(model);

    reflow();
}
