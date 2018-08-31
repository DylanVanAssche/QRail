/*
 *   This file is part of QRail.
 *
 *   QRail is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   QRail is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with QRail.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef LINKEDCONNECTIONFACTORY_H
#define LINKEDCONNECTIONFACTORY_H

#include <QtCore/QDebug>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonParseError>
#include <QtCore/QObject>
#include <QtCore/QRegularExpression>

#include "fragments/fragmentsdispatcher.h"
#include "fragments/fragmentsfragment.h"
#include "fragments/fragmentspage.h"
#include "network/networkmanager.h"
#include "qrail.h"

#define BASE_URL "https://graph.irail.be/sncb/connections"

// Factory pattern to generate Linked Connections fragments on the fly
namespace QRail {
namespace Fragments {
class Factory : public QObject {
  Q_OBJECT
public:
  static QRail::Fragments::Factory *getInstance();
  void getPage(const QUrl &uri, QObject *caller);
  void getPage(const QDateTime &departureTime, QObject *caller);
  QRail::Fragments::Dispatcher *dispatcher() const;

protected:
  virtual void customEvent(QEvent *event);

signals:
  void pageReady(QRail::Fragments::Page *page);
  void getResource(const QUrl &uri, QObject *caller);
  void error(const QString &message);

private:
  static QRail::Fragments::Factory *m_instance;
  QRail::Network::Manager *m_http;
  QRail::Fragments::Dispatcher *m_dispatcher;
  void getPageByURIFromNetworkManager(const QUrl &uri);
  QRail::Fragments::Fragment *generateFragmentFromJSON(const QJsonObject &data);
  bool validateData(const QJsonObject &data, const QStringList &properties);
  void processHTTPReply(QNetworkReply *reply);
  QRail::Network::Manager *http() const;
  void setHttp(QRail::Network::Manager *http);
  void setDispatcher(QRail::Fragments::Dispatcher *dispatcher);
  explicit Factory(QObject *parent = nullptr);
};
} // namespace Fragments
} // namespace QRail

#endif // LINKEDCONNECTIONFACTORY_H
