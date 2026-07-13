#ifndef WIDGETCONTAINER_H
#define WIDGETCONTAINER_H

#include <QFrame>
#include <QLabel>
#include <QPoint>

class WidgetContainer : public QFrame {
    Q_OBJECT
public:
    explicit WidgetContainer(const QString &labelText, QWidget *parent = nullptr);

signals:
    void dragStarted(WidgetContainer *widget);
    void dragging(WidgetContainer *widget, const QPoint &globalPos);
    void dragFinished(WidgetContainer *widget, const QPoint &globalPos);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QLabel *m_label;
    QPoint m_dragOffset;        // Abstand Klickpunkt <-> Widget-Ecke
    bool m_dragging = false;
    QRect m_originalGeometry;   // zum Zurückspringen bei Release
};

#endif // WIDGETCONTAINER_H
