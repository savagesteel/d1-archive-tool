#ifndef D1MPQ_H
#define D1MPQ_H

#include <QString>
#include <QObject>
#include <QPointer>
#include <QFile>
#include <QBuffer>
#include <QDataStream>
#include <QCryptographicHash>

class D1Mpq : public QObject
{
    Q_OBJECT

public:
    D1Mpq();
    D1Mpq( QString );
    ~D1Mpq();

    bool load( QString );

    QString getFilePath();
    bool isFileOpen();

private:
    QFile file;

};

#endif // D1MPQ_H
