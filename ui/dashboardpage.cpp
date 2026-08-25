#include "dashboardpage.h"
#include "dashboardcanvas.h"
#include <QScrollArea>
#include <QVBoxLayout>

DashboardPage::DashboardPage(QWidget *parent)
    : QWidget(parent)
{
    // buildWidgets(); // only for testing

	auto* canvas = new DashboardCanvas(m_data, this);   // anstelle m_data: m_context.session().widgets() - but session is not available here, so we have to pass the widgets from the session to the DashboardPage constructor

    auto *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(canvas);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(scrollArea);
}

DashboardPage::~DashboardPage() = default;

//FUNKTION ZUM WIDGET ERSTELLEN FÜR TESTS
/*
void DashboardPage::buildWidgets() {
	// Erstellt Widgets zum testen - Funktion eigentlich überflüssig, da die Widgets aus der Datenbank geladen werden sollten
    // Eigentlich: context.session.widgetliste....

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
    model4.wHeight = 4;
    m_data.append(model4);

    WidgetModel model5;
    model5.wName = "widget_5";
    model5.wPosX = 3;
    model5.wPosY = 0;
    model5.wWidth = 1;
    model5.wHeight = 2;
    m_data.append(model5);

    WidgetModel model6;
    model6.wName = "widget_6";
    model6.wPosX = 1;
    model6.wPosY = 2;
    model6.wWidth = 1;
    model6.wHeight = 2;
    m_data.append(model6);

    WidgetModel model7;
    model7.wName = "widget_7";
    model7.wPosX = 2;
    model7.wPosY = 4;
    model7.wWidth = 1;
    model7.wHeight = 1;
    m_data.append(model7);

    WidgetModel model8;
    model8.wName = "widget_8";
    model8.wPosX = 3;
    model8.wPosY = 4;
    model8.wWidth = 1;
    model8.wHeight = 1;
    m_data.append(model8);
}
*/