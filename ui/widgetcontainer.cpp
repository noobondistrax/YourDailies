#include "widgetcontainer.h"
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QPainter>

WidgetContainer::WidgetContainer(QString widgetName, QWidget *parent)
    : QFrame(parent)
{
    m_label = new QLabel(widgetName, this);
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

    // Grip-Bereich hat Vorrang vor dem normalen Verschieben
    if (gripRect().contains(event->pos())) {
        m_resizing = true;
        m_resizeStartPos = event->globalPosition().toPoint();
        m_resizeStartSize = size();

        raise();
        setCursor(Qt::SizeFDiagCursor);

        event->accept();
        return;
    }

    m_dragging = true;
    m_dragOffset = event->pos();          // Point of Click relative to the Widget
    m_originalGeometry = geometry();      // remember for "reset" - original position

    raise();                              // Qt Func - raise the widgit above the other sibling-container
    emit dragStarted(this);

    event->accept();
}

void WidgetContainer::mouseMoveEvent(QMouseEvent *event)
{
    if (m_resizing) {
        // Freies Pixel-Resize für flüssiges Feedback - noch nicht am Grid ausgerichtet.
        // Das Model (WidgetModel) wird hier bewusst NICHT verändert.
        const QPoint delta = event->globalPosition().toPoint() - m_resizeStartPos;

        const int newWidth  = qMax(kMinPixelSize, m_resizeStartSize.width()  + delta.x());
        const int newHeight = qMax(kMinPixelSize, m_resizeStartSize.height() + delta.y());

        resize(newWidth, newHeight);

        event->accept();
        return;
    }

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
    if (m_resizing) {
        m_resizing = false;
        unsetCursor();

        // DashboardCanvas rechnet die Pixelgröße in Zellen zurück und entscheidet über Kollisionen
        emit resizeFinished(this, size());

        event->accept();
        return;
    }

    if (!m_dragging) return;
    m_dragging = false;

    emit dragFinished(this, event->globalPosition().toPoint());
    event->accept();
}

QRect WidgetContainer::gripRect() const
{
    return QRect(width() - kGripSize, height() - kGripSize, kGripSize, kGripSize);
}

void WidgetContainer::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    // kleiner visueller Hinweis auf den Grip-Bereich, damit Nutzer ihn finden
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(QColor("#9e9e9e"), 2));

    const QRect grip = gripRect();
    for (int offset = 3; offset < kGripSize; offset += 4) {
        painter.drawLine(grip.right() - offset, grip.bottom(),
                         grip.right(), grip.bottom() - offset);
    }
}