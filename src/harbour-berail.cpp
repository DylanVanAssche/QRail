/*
*   This file is part of BeRail.
*
*   BeRail is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   BeRail is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with BeRail.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <sailfishapp.h>
#include <QtCore/QScopedPointer>
#include <QtCore/QString>
#include <QtCore/QTranslator>
#include <QtGui/QGuiApplication>
#include <QtQuick/QQuickView>
#include <QtQml/QQmlEngine>

#include "logger.h"
#include "os.h"
#include "../tests/network/networkmanagertest.h"
#include "../tests/database/databasemanagertest.h"
#include "../tests/fragments/fragmentsfragmenttest.h"
#include "../tests/fragments/fragmentspagetest.h"
#include "../tests/fragments/fragmentsfactorytest.h"
#include "../tests/csa/csaplannertest.h"

// Add toString() method to all custom method

int main(int argc, char *argv[])
{
    // Set up qml engine.
    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    QScopedPointer<QQuickView> view(SailfishApp::createView());
    qApp->setApplicationVersion(QString(APP_VERSION));

    // Set application version and enable logging
#ifndef QT_QML_DEBUG
    enableLogger(true);
#endif

    // Enable default translations
    QTranslator *translator = new QTranslator(qApp);
    QString trPath = SailfishApp::pathTo(QStringLiteral("translations")).toLocalFile();
    QString appName = app->applicationName();
    // Check if translations have been already loaded
    if (!translator->load(QLocale::system(), appName, "-", trPath))
    {
        // Load default translations if not
        translator->load(appName, trPath);
        app->installTranslator(translator);
    }
    else
    {
        translator->deleteLater();
    }

    // Register custom QML modules
    qmlRegisterUncreatableType<Liveboard>("Harbour.BeRail.Models", 1, 0, "Liveboard", "read only");
    qmlRegisterUncreatableType<Vehicle>("Harbour.BeRail.Models", 1, 0, "Vehicle", "read only");
    qmlRegisterUncreatableType<Connection>("Harbour.BeRail.Models", 1, 0, "Connection", "read only");
    qmlRegisterUncreatableType<Station>("Harbour.BeRail.Models", 1, 0, "Station", "read only");
    qmlRegisterUncreatableType<Stop>("Harbour.BeRail.Models", 1, 0, "Stop", "read only");
    qmlRegisterUncreatableType<Via>("Harbour.BeRail.Models", 1, 0, "Via", "read only");
    qmlRegisterUncreatableType<Disturbances>("Harbour.BeRail.Models", 1, 0, "Disturbances", "read only");
    qmlRegisterUncreatableType<Alert>("Harbour.BeRail.Models", 1, 0, "Alert", "read only");
    qmlRegisterUncreatableType<IRail>("Harbour.BeRail.Models", 1, 0, "IRail", "read only");
    qmlRegisterType<API>("Harbour.BeRail.API", 1, 0, "API");
    qmlRegisterType<OS>("Harbour.BeRail.SFOS", 1, 0, "SFOS");

    // Start the application.
    view->setSource(SailfishApp::pathTo("qml/harbour-berail.qml"));
    view->show();

    return app->exec();
}
