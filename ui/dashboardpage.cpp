#include "dashboardpage.h"
#include "dashboardcanvas.h"
#include <QScrollArea>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolButton>


DashboardPage::DashboardPage(DashboardService dashboardService, QList<WidgetModel> &widgets, WidgetService &widgetService, bool userIsAdmin, QWidget* parent)
    : QWidget(parent), m_dashboardService(dashboardService), m_userIsAdmin(userIsAdmin)
{
    auto* canvas = new DashboardCanvas(widgets, widgetService, this);

    auto *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(canvas);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

	auto* sidebar = buildSidebar();

    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
    layout->addWidget(sidebar);
    layout->addWidget(scrollArea);
}

DashboardPage::~DashboardPage() = default;

QWidget* DashboardPage::buildSidebar()
{
    {
        auto* sidebar = new QWidget(this);
        sidebar->setFixedWidth(56);
        sidebar->setStyleSheet("background-color: #2b2b2b;");

        auto* sidebarLayout = new QVBoxLayout(sidebar);
        sidebarLayout->setContentsMargins(4, 12, 4, 12);
        sidebarLayout->setSpacing(8);

        auto* addWidgetBtn = buildSidebarButton(":/icons/add_widget.png", "Widget hinzufuegen");
        connect(addWidgetBtn, &QToolButton::clicked, this, &DashboardPage::addWidgetClicked);
        sidebarLayout->addWidget(addWidgetBtn);

        sidebarLayout->addStretch();   // schiebt den Rest ans untere Ende

        if (m_userIsAdmin) {
            auto* adminBtn = buildSidebarButton(":/icons/admin.png", "Adminbereich");
            connect(adminBtn, &QToolButton::clicked, this, &DashboardPage::adminClicked);
            sidebarLayout->addWidget(adminBtn);
        }

        auto* settingsBtn = buildSidebarButton(":/icons/settings.png", "Einstellungen");
        connect(settingsBtn, &QToolButton::clicked, this, &DashboardPage::settingsClicked);
        sidebarLayout->addWidget(settingsBtn);

        auto* logoutBtn = buildSidebarButton(":/icons/logout.png", "Abmelden");
        connect(logoutBtn, &QToolButton::clicked, this, &DashboardPage::logoutClicked);
        sidebarLayout->addWidget(logoutBtn);

        return sidebar;
    }
}

QToolButton* DashboardPage::buildSidebarButton(const QString& iconPath, const QString& tooltip)
{
    auto* btn = new QToolButton(this);
    btn->setIcon(QIcon(iconPath));
    btn->setIconSize(QSize(28, 28));
    btn->setToolTip(tooltip);
    btn->setFixedSize(48, 48);
    btn->setStyleSheet("QToolButton { border: none; border-radius: 8px; } "
        "QToolButton:hover { background-color: #3a3a3a; }");
    return btn;
}
