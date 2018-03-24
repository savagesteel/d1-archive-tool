#include "D1MPQ.h"

D1Mpq::D1Mpq() :
    file( new QFile )
{}

D1Mpq::D1Mpq( QString path ) :
    file( new QFile )
{
    this->load( path );
}

D1Mpq::~D1Mpq()
{
    if( this->file.isOpen() )
        this->file.close();
}

bool D1Mpq::load( QString dirFilePath )
{
/*
    quint32 readDword;

    // Opening EXE file with a QBuffer to load it in RAM
    if( !QFile::exists( exeFilePath ) )
        return false;

    if( this->file.isOpen() )
        this->file.close();

    this->file.setFileName( exeFilePath );

    if( !this->file.open(QIODevice::ReadOnly) )
        return false;


    // Load EXE file in RAM buffer
    this->file.seek(0);
    QByteArray fileData = this->file.readAll();
    QBuffer fileBuffer( &fileData );

    if( !fileBuffer.open(QIODevice::ReadOnly) )
        return false;

    // Read EXE binary data
    QDataStream in( &fileBuffer );
    in.setByteOrder( QDataStream::LittleEndian );

    if( this->version == D1_VERSION::D1_PR1 || this->version == D1_VERSION::D1_PR2 )
    {

    }
    else
    {
        // Prefixes table
        readDword = 0;
        prefixesTableEndOffset = 0;
        for( unsigned int o = this->prefixesTableOffset; o < this->file.size(); o += D1_AFFIX_SIZE )
        {
            if( o + 4 < this->file.size() )
                fileBuffer.seek( o + 4 );
            else
                break;

            in >> readDword;
            if( readDword == 0xFFFFFFFF )
            {
                prefixesTableEndOffset = o;
                break;
            }
        }

        fileBuffer.seek( this->prefixesTableOffset );
        this->prefixesTable.load( fileBuffer.read(
            prefixesTableEndOffset - this->prefixesTableOffset ) );

        // Suffixes table
        readDword = 0;
        suffixesTableEndOffset = 0;
        for( unsigned int o = this->suffixesTableOffset; o < this->file.size(); o += D1_AFFIX_SIZE )
        {
            if( o + 4 < this->file.size() )
                fileBuffer.seek( o + 4 );
            else
                break;

            in >> readDword;
            if( readDword == 0xFFFFFFFF )
            {
                suffixesTableEndOffset = o;
                break;
            }
        }

        fileBuffer.seek( this->suffixesTableOffset );
        this->suffixesTable.load( fileBuffer.read(
            suffixesTableEndOffset - this->suffixesTableOffset ) );

        // Items table
        readDword = 0;
        itemsTableEndOffset = 0;
            for( unsigned int o = this->itemsTableOffset; o < this->file.size(); o += D1_ITEM_SIZE )
        {
            if( o + 16 < this->file.size() )
                fileBuffer.seek( o + 16 );
            else
                break;

            in >> readDword;

            // The 0xE40 condition is needed to skip blank items in Diablo (not Hellfire)
            if( readDword == 0x00000000 && (o-this->itemsTableOffset) > 0xE40 )
            {
                itemsTableEndOffset = o;
                break;
            }
        }

        fileBuffer.seek( this->itemsTableOffset );
        this->itemsTable.load( fileBuffer.read(
            itemsTableEndOffset - this->itemsTableOffset ) );
    }
*/
    return true;
}

QString D1Mpq::getFilePath()
{
    if( this->file.isOpen() )
        return this->file.fileName();
    else
        return QString();
}

bool D1Mpq::isFileOpen()
{
    return this->file.isOpen();
}
