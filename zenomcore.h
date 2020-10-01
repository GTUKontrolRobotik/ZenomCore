#ifndef ZENOMCORE_H
#define ZENOMCORE_H

#include <QObject>
#include <iostream>

class ZenomCore : public QObject
{
    Q_OBJECT
public:
    explicit ZenomCore(QObject *parent = nullptr, int argc = 0, char *argv[] = nullptr);

signals:

public slots:
};

#endif // ZENOMCORE_H
