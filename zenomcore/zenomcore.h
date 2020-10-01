#ifndef ZENOMCORE_H
#define ZENOMCORE_H

#include <QObject>
#include <QStringList>
#include <QDebug>
#include <iostream>

class ZenomCore : public QObject
{
    Q_OBJECT
public:
    explicit ZenomCore(QObject *parent = nullptr, QStringList args = QStringList() );

signals:

public slots:
};

#endif // ZENOMCORE_H
