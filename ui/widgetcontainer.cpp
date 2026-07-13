#include "widgetcontainer.h"
#include <QVBoxLayout>
#include <QMouseEvent>

WidgetContainer::WidgetContainer(const QString &labelText, QWidget *parent)
    : QFrame(parent)
{
    setFrameShape(QFrame::Box);
    m_label = new QLabel(labelText, this);
    m_label->setAlignment(Qt::AlignCenter);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(m_label);
}

void WidgetContainer::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) return;

    m_dragging = true;
    m_dragOffset = event->pos();          // Klickpunkt relativ zum Widget
    m_originalGeometry = geometry();      // für Rücksprung merken

    raise();                              // über die anderen Widgets legen
    emit dragStarted(this);

    event->accept();
}

void WidgetContainer::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_dragging) return;

    // Mausposition relativ zum Parent (DashboardCanvas) berechnen
    QPoint parentPos = parentWidget()->mapFromGlobal(event->globalPosition().toPoint());
    QPoint newTopLeft = parentPos - m_dragOffset;

    move(newTopLeft);

    emit dragging(this, event->globalPosition().toPoint());
    event->accept();
}

void WidgetContainer::mouseReleaseEvent(QMouseEvent *event)
{
    if (!m_dragging) return;
    m_dragging = false;

    // Schritt 2: noch keine Zielzellen-Logik -> immer zurückspringen
    setGeometry(m_originalGeometry);

    emit dragFinished(this, event->globalPosition().toPoint());
    event->accept();
}