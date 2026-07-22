#ifndef WIDGETCONTAINER_H
#define WIDGETCONTAINER_H

#include <QFrame>
#include <QLabel>
#include <QPoint>
#include "widgetModel.h"

class WidgetContainer : public QFrame {
    Q_OBJECT
public:
    explicit WidgetContainer(QString widgetName, QWidget *parent = nullptr);

signals:
    void dragStarted(WidgetContainer *widget);
    void dragging(WidgetContainer *widget, const QPoint &globalPos);
    void dragFinished(WidgetContainer *widget, const QPoint &globalPos);
    void resizeFinished(WidgetContainer *widget, const QSize &newSize);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;   // zeichnet den kleinen Grip unten rechts

private:
    QLabel *m_label;
    QPoint m_dragOffset;        // Abstand Klickpunkt <-> Widget-Ecke
    bool m_dragging = false;
    QRect m_originalGeometry;   // zum Zurückspringen bei Release
    QRect gripRect() const;

    bool m_resizing = false;
    QPoint m_resizeStartPos;    // globale Mausposition bei Resize-Start
    QSize m_resizeStartSize;    // Widget-Größe bei Resize-Start

    static constexpr int kGripSize = 14;       // Größe des Grip-Bereichs in Pixeln
    static constexpr int kMinPixelSize = 40;   // Mindestgröße während des freien Ziehens (noch nicht Grid-genau)
};

#endif // WIDGETCONTAINER_H
