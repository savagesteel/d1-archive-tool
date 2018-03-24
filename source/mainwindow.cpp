#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::getSizeString( quint64 size )
{
    QStringList units = { "B", "KB", "MB", "GB", "TB", "PB", "EB" };

    quint8 unitIndex = 0;
    float tmp = size;
    while( tmp > 1024 )
    {
        tmp /= 1024;
        unitIndex++;
    }

    return QString::number( tmp, 'f', 2 ) + " " + units.at(unitIndex);
}

void MainWindow::showDirBinContent()
{
    this->ui->archiveWidget->clear();

    if( this->dirbin.isNull() )
        return;

    QMap< QString, QPair<quint32,quint32> >* files = this->dirbin->getFileEntries();
    QMap< QString, QPair<quint32,quint32> >::iterator it;

    // Creating root tree element
    QStringList archiveRootStrings = {
        this->dirbin->getBinFilePath(),
        this->getSizeString( this->dirbin->getBinFileSize() ),
        "",
        QString::number( this->dirbin->getFileCount() )
    };

    QTreeWidgetItem* archiveRoot = new QTreeWidgetItem( archiveRootStrings );
    archiveRoot->setFlags( Qt::ItemIsEnabled ); // Selection disabled
    archiveRoot->setTextAlignment( 1, Qt::AlignRight );
    archiveRoot->setTextAlignment( 2, Qt::AlignRight );
    archiveRoot->setTextAlignment( 3, Qt::AlignRight );

    // Appending files to root
    //QStringList fileStrings;
    QTreeWidgetItem* fileItem;
    for( it = files->begin(); it != files->end(); ++it )
    {
        QStringList fileStrings = {
            it.key(),
            this->getSizeString( it.value().second ),
            "None"
        };

        fileItem = new QTreeWidgetItem( fileStrings );
        fileItem->setTextAlignment( 1, Qt::AlignRight );
        fileItem->setTextAlignment( 2, Qt::AlignRight );
        fileItem->setTextAlignment( 3, Qt::AlignRight );

        archiveRoot->addChild( fileItem );
    }

    this->ui->archiveWidget->addTopLevelItem( archiveRoot );
    this->ui->archiveWidget->expandItem( archiveRoot );
    this->ui->archiveWidget->sortByColumn( 0, Qt::AscendingOrder );
    this->ui->archiveWidget->resizeColumnToContents(0);
    this->ui->archiveWidget->resizeColumnToContents(1);
    this->ui->archiveWidget->resizeColumnToContents(2);
    this->ui->archiveWidget->resizeColumnToContents(3);
}

void MainWindow::on_actionOpen_triggered()
{
    QString openFilePath = QFileDialog::getOpenFileName(
        this, "Open Diablo/Hellfire Archive", QString(), "Blizzard MPQ v1 Archive (*.mpq *.exe *.sv *.hsv);;EA PlayStation Archive (*.dir *.bin)" );

    if( !openFilePath.isEmpty() )
    {
        // Check file extension
        if( openFilePath.toLower().endsWith( ".dir" ) || openFilePath.toLower().endsWith( ".bin" ) )
        {
            this->on_actionClose_triggered();
            this->dirbin = new D1PS1DirBin;

            QString dirFilePath = openFilePath;
            QString binFilePath = openFilePath;

            // Generating associated BIN/DIR file name
            if( openFilePath.toLower().endsWith( ".dir" ) )
                binFilePath.replace(".dir",".bin").replace(".DIR",".BIN");
            else
                dirFilePath.replace(".bin",".dir").replace(".BIN",".DIR");

            // Checking .DIR file
            if( !QFileInfo::exists( dirFilePath ) )
            {
                QMessageBox::critical( this, "Error", QString("Associated .DIR file is missing.\n")
                    + QString("EA PlayStation Archives consist in one .DIR file and one .BIN file with the same name.") );
                return;
            }

            // Checking .BIN file
            if( !QFileInfo::exists( binFilePath ) )
            {
                QMessageBox::critical( this, "Error", QString("Associated .BIN file is missing.\n")
                    + QString("EA PlayStation Archives consist in one .DIR file and one .BIN file with the same name.") );
                return;
            }

            // Attempt to load the files
            if( !this->dirbin->load( dirFilePath, binFilePath ) )
            {
                QMessageBox::critical( this, "Error", "Failed loading PlayStation Archive." );
                return;
            }

            // Display archive content
            this->showDirBinContent();
        }
        else if( openFilePath.toLower().endsWith( ".mpq" ) || openFilePath.toLower().endsWith( ".exe" )
            || openFilePath.toLower().endsWith( ".sv" ) || openFilePath.toLower().endsWith( ".hsv" ) )
        {

        }
    }
}

void MainWindow::on_actionClose_triggered()
{
    if( this->dirbin )
        delete this->dirbin;

    if( this->mpq )
        delete this->mpq;

    this->ui->archiveWidget->clear();
}

void MainWindow::on_actionQuit_triggered()
{
    qApp->quit();
}

void MainWindow::on_actionSelect_All_triggered()
{
    this->ui->archiveWidget->selectAll();
}

void MainWindow::on_actionAbout_triggered()
{
    QString architecture;
    QString operatingSystem;

#ifdef Q_PROCESSOR_X86_64
    architecture = "(64-bit)";
#endif

#ifdef Q_PROCESSOR_X86_32
    architecture = "(32-bit)";
#endif

#ifdef Q_OS_WIN
    operatingSystem = "Windows";
#endif

#ifdef Q_OS_MAC
    operatingSystem = "Mac OS X";
#endif

#ifdef Q_OS_LINUX
    operatingSystem = "Linux";
#endif

    QMessageBox::about( this, "About", "Diablo 1 Archive Tool "
        + QString( D1_ARCHIVE_TOOL_VERSION ) + " (" + operatingSystem + ") " + architecture );
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt( this, "About Qt" );
}

