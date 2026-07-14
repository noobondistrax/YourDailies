#include "widgetcontainer.h"
#include <QVBoxLayout>
#include <QMouseEvent>

WidgetContainer::WidgetContainer(const QString &labelText, QWidget *parent)
    : QFrame(parent)
{
    m_label = new QLabel(labelText, this);
    m_label->setAlignment(Qt::AlignCenter);

    setStyleSheet(R"(
        QFrame {
            background-color: #f2f2f2;
            border: 1px solid #bdbdbd;
            border-radius: 12px;
        }
        QLabel {
            background: transparent;
            border: none;
        }
    )");

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(m_label);
}

void WidgetContainer::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) return;

    m_dragging = true;
    m_dragOffset = event->pos();          // Point of Click relative to the Widget
    m_originalGeometry = geometry();      // remember for "reset" - original position

    raise();                              // Qt Func - raise the widgit above the other sibling-container
    emit dragStarted(this);

    event->accept();
}

void WidgetContainer::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_dragging) return;

    // calculate the mouse position relative to the parent Widget (dashboardcanvas)
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

    emit dragFinished(this, event->globalPosition().toPoint());
    event->accept();
}