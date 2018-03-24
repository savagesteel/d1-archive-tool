#include "d1ps1dirbin.h"

D1PS1DirBin::D1PS1DirBin() :
    dirFile( new QFile ),
    binFile( new QFile ),
    fileCount( 0 )
{}

D1PS1DirBin::D1PS1DirBin( QString dirPath, QString binPath ) :
    dirFile( new QFile ),
    binFile( new QFile ),
    fileCount( 0 )
{
    this->load( dirPath, binPath );
}

D1PS1DirBin::~D1PS1DirBin()
{
    if( this->dirFile.isOpen() )
        this->dirFile.close();

    if( this->binFile.isOpen() )
        this->binFile.close();
}

bool D1PS1DirBin::load( QString dirFilePath, QString binFilePath )
{
    quint32 readDword;

    if( !QFile::exists( dirFilePath ) )
        return false;
    if( !QFile::exists( binFilePath ) )
        return false;

    if( this->dirFile.isOpen() )
        this->dirFile.close();
    if( this->binFile.isOpen() )
        this->binFile.close();

    this->dirFile.setFileName( dirFilePath );
    this->binFile.setFileName( binFilePath );

    if( !this->dirFile.open(QIODevice::ReadOnly) )
        return false;
    if( !this->binFile.open(QIODevice::ReadOnly) )
        return false;

    // DIR file minimum size (one entry) is 0x1C (28)
    if( this->dirFile.size() < 0x1C )
        return false;
    // BIN file minimum size is one 0x800 (2048) block
    if( this->binFile.size() < 0x800 )
        return false;

    // Load DIR file in RAM buffer
    // BIN file not completely loaded in RAM
    // because these files have usually a high file size
    this->dirFile.seek(0);
    QByteArray dirFileData = this->dirFile.readAll();
    QBuffer dirFileBuffer( &dirFileData );

    if( !dirFileBuffer.open(QIODevice::ReadOnly) )
        return false;

    // Read DIR binary data
    QDataStream in( &dirFileBuffer );
    in.setByteOrder( QDataStream::LittleEndian );

    // Checking DIR file signature (LDIR)
    in >> readDword;
    if( readDword != 0x5249444C )
        return false;

    // Reading file count
    in >> readDword;
    if( this->dirFile.size() != readDword*20 + 8 )
        return false;
    else
        this->fileCount = readDword;

    // Checking BIN file size
    if( this->binFile.size() < this->fileCount * 0x800 )
        return false;

    // Reading file entries
    for( unsigned int i = 0; i < this->fileCount; i++ )
    {
        // First DWORD is file offset
        // Second DWORD is file size
        QPair<quint32,quint32> offsetSize;

        in >> offsetSize.first;
        in >> offsetSize.second;

        QByteArray fileNameData = dirFileBuffer.read( 12 );
        QString fileName = QString::fromLatin1( fileNameData );

        if( fileName.isEmpty() )
            fileName = "UNKNOWN."
                + QString("%1").arg( i+1, 3, 10, QChar('0') );

        this->fileEntries[fileName] = offsetSize;
    }

    return true;
}

quint32 D1PS1DirBin::get32bitChecksum( QByteArray &data )
{
    QBuffer buffer( &data );
    buffer.open( QIODevice::ReadOnly );
    QDataStream in( &buffer );

    quint8 byte = 0;
    quint32 checksum = 0;
    while( !in.atEnd() )
    {
        in >> byte;
        checksum += byte;
    }

    return checksum;
}

QString D1PS1DirBin::getDirFilePath()
{
    if( this->dirFile.isOpen() )
        return this->dirFile.fileName();
    else
        return QString();
}

QString D1PS1DirBin::getBinFilePath()
{
    if( this->binFile.isOpen() )
        return this->binFile.fileName();
    else
        return QString();
}

bool D1PS1DirBin::isDirFileOpen()
{
    return this->dirFile.isOpen();
}

bool D1PS1DirBin::isBinFileOpen()
{
    return this->binFile.isOpen();
}

quint32 D1PS1DirBin::getBinFileSize()
{
    if( this->binFile.isOpen() )
        return this->binFile.size();
    else
        return 0;
}

quint32 D1PS1DirBin::getFileCount()
{
    return this->fileCount;
}

QMap< QString, QPair<quint32,quint32> >* D1PS1DirBin::getFileEntries()
{
    return &this->fileEntries;
}
