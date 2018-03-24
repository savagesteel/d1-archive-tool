#ifndef D1PS1DirBin_H
#define D1PS1DirBin_H

#include <QString>
#include <QObject>
#include <QPointer>
#include <QFile>
#include <QBuffer>
#include <QDataStream>
#include <QCryptographicHash>
#include <QPair>
#include <QList>
#include <QMap>

class D1PS1DirBin : public QObject
{
    Q_OBJECT

public:
    D1PS1DirBin();
    D1PS1DirBin( QString, QString );
    ~D1PS1DirBin();

    bool load( QString, QString );
    quint32 get32bitChecksum( QByteArray & );
    void verifyFileIntegrity( QString );
    void verifyAllFilesIntegrity();
    bool extractFile( QString, QString );
    bool extractAllFiles( QString );

    QString getDirFilePath();
    QString getBinFilePath();
    bool isDirFileOpen();
    bool isBinFileOpen();
    quint32 getBinFileSize();
    quint32 getFileCount();
    QMap< QString, QPair<quint32,quint32> >* getFileEntries();

private:
    QFile dirFile;
    QFile binFile;

    quint32 fileCount;
    QMap< QString, QPair<quint32,quint32> > fileEntries;
};

#endif // D1PS1DirBin_H
