#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_bFtpStop(false)
{
    ui->setupUi(this);
    init();
    InitUiElements();
    CreateActions();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    QTreeWidgetItem *top = new QTreeWidgetItem(ui->twLocalFile , QStringList(QString(tr("我的电脑"))));
    m_twLocalroot.append(top);
    ui->twLocalFile->insertTopLevelItems(0 , m_twLocalroot);
}

//! 初始化UI配置
void MainWindow::InitUiElements()
{
    ui->lblServer->setText(tr("Server Address"));
    ui->lblPort->setText(tr("Port"));
    ui->btConnect->setText(tr("Connect"));
    ui->btStop->setText(tr("Stop"));
    ui->lblUsername->setText(tr("Username"));
    ui->lblPassword->setText(tr("Password"));
    ui->lnePassword->setEchoMode(QLineEdit::Password);
    ui->textMsg->setText(tr(""));
    ui->btConnect->setText(tr("Connect"));
    ui->lblSatus->setText(tr(""));
    ui->lblProcessInfo->setText(QObject::tr("").toStdString().c_str());
    ui->lblSatus->setHidden(true);
    ui->lblProcessInfo->setHidden(true);
    ui->textMsg->setReadOnly(true);
    ui->barProcess->setValue(0);
    ui->barProcess->setHidden(true);
    ui->btStop->setHidden(true);

    m_localMenu =new QMenu(ui->twLocalFile);//定义本地路径一个右键弹出菜单
    m_remoteMenu =new QMenu(ui->twRemoteFile);//定义远程路径一个右键弹出菜单

    //! 账户密码
    QString strServerIP = "ftp.vsochina.com";
    QString strUsername = "gaoyuan123";
    QString strPassword = "dell_456";
    QString nPort = "21";

    ui->lneServer->setText(strServerIP);
    ui->lneUsername->setText(strUsername);
    ui->lnePassword->setText(strPassword);
    ui->lnePort->setText(nPort);
    //!
    QStringList sltHeader;
    sltHeader.append(tr("Name"));
    sltHeader.append(tr("Size"));
    sltHeader.append(tr("Modify Time"));
    sltHeader.append(tr("Owner"));
    ui->twRemoteFile->setHeaderLabels(sltHeader);
    ui->twLocalFile->setHeaderLabels(sltHeader);
}

void MainWindow::CreateActions()
{
    connect(ui->twRemoteFile, SIGNAL(itemActivated(QTreeWidgetItem*,int)), this, SLOT(ProcessItem(QTreeWidgetItem*,int)));
    connect(ui->btConnect, SIGNAL(clicked(bool)), this, SLOT(TriggerConnectClicked()));
    connect(ui->btStop, SIGNAL(clicked(bool)), this, SLOT(HandleFtpInterruput()));

    dirScan = new DirScan();

    //! 绑定扫DirScan扫描完的信号，发送过来添加进树
    if(!connect(dirScan , SIGNAL(ItemScaned(QString,QFileInfo,int)) , this , SLOT(AddItem(QString,QFileInfo,int)))){
        qDebug("--error:1--\n");
    }

    //! 绑定本地树控件的单击事件，经过处理后发送出去，参数为点击的path
    if(!connect(ui->twLocalFile , SIGNAL(itemActivated(QTreeWidgetItem*,int)) , this , SLOT(selectItem(QTreeWidgetItem* , int)))){
        qDebug("--error:2--\n");
    }

    //! 绑定发送出去的事件，即发送给DirScan的事件
    if(!connect(this , SIGNAL(sendToDirScan(QString)) , dirScan , SLOT(Scan(QString)))){
        qDebug("--error:3--\n");
    }

    connect(ui->twRemoteFile, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(HandleTwRemoteCustomContextMenuRequested(QPoint)));
    connect(ui->twLocalFile, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(HandleTwLocalCustomContextMenuRequested(QPoint)));

    connect(m_localMenu, SIGNAL(triggered(QAction*)), this, SLOT(HandleLocalMenuTrigger(QAction*)));
    connect(m_remoteMenu, SIGNAL(triggered(QAction*)), this, SLOT(HandleRemoteMenuTrigger(QAction*)));
}

//! 初始化连接
void MainWindow::TriggerConnectClicked()
{
    ui->twRemoteFile->clear();
    m_strCurrentRemotePath.clear();
    m_ftp = new QFtp(this);

    connect(m_ftp, SIGNAL(commandStarted(int)), this, SLOT(FTPCommandStart(int)));
    connect(m_ftp, SIGNAL(commandFinished(int, bool)), this, SLOT(FTPCommandFinished(int,bool)));
    connect(m_ftp, SIGNAL(listInfo(QUrlInfo)), this, SLOT(AddToList(QUrlInfo)));
    connect(m_ftp, SIGNAL(dataTransferProgress(qint64,qint64)), this, SLOT(updateDataTransferProgress(qint64,qint64)));

    QString strServerIP = ui->lneServer->text();
    QString strUsername = ui->lneUsername->text();
    QString strPassword = ui->lnePassword->text();
    quint16 nPort = ui->lnePort->text().toUInt();


    m_ftp->connectToHost(strServerIP, nPort);
    m_ftp->login(strUsername, strPassword);
}

//! 将目录内文件加入列表
void MainWindow::AddToList(const QUrlInfo &urlInfo)
{
    qDebug() << FromFTPEncoding(urlInfo.name());


    if (ui->twRemoteFile->topLevelItemCount() == 0)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem;
        item->setText(0, "..");
        item->setIcon(0, QIcon(":/dir.png"));
        m_mapDirctory[".."] = true;
        ui->twRemoteFile->addTopLevelItem(item);

    }

    QTreeWidgetItem *item = new QTreeWidgetItem;
    item->setText(0, urlInfo.name());
    //item->setText(0, FromFTPEncoding(urlInfo.name()));
    item->setText(1, QString::number(urlInfo.size()));
    item->setText(2, urlInfo.lastModified().toString("yyyy-MM-dd  hh:mm:ss"));
    item->setText(3, urlInfo.owner());
    item->setIcon(0, urlInfo.isDir() ? QIcon(":/dir.png") : QIcon(":/file.png"));
    m_mapDirctory[urlInfo.name()] = urlInfo.isDir();

    ui->twRemoteFile->addTopLevelItem(item);
    if (!ui->twRemoteFile->currentItem())
    {
        ui->twRemoteFile->setCurrentItem(ui->twRemoteFile->topLevelItem(0));
        ui->twRemoteFile->setEnabled(true);
    }
}

void MainWindow::AddItem(const QString &strRootPath, const QFileInfo &ItemInfo, const int k)
{
    if(ItemInfo.isDir())
    {
        QString fullPath = ItemInfo.absolutePath().replace("/" , "\\") ;

        qDebug("(fullPath: %s)\n" , fullPath.toLatin1().data());

        // 这个只是第一次走，剩下的肯定都是包含的，因为盘符是在最外面的
        if(!m_StoreDirItem.contains(fullPath))
        {
            QString showname = ( k ? ItemInfo.fileName() : ItemInfo.filePath() );
            qDebug("---not----contains----k = %d ---------showname = %s.\n" , k , showname.toLatin1().data());
            QTreeWidgetItem *item = new QTreeWidgetItem(ui->twLocalFile->findItems(QString(tr("我的电脑")) , 0 , 0).at(0) , QStringList(showname));
            m_StoreDirItem.insert(fullPath , item);

        }
        else
        {
            qDebug("---------contains---------\n");
            QTreeWidgetItem *item = m_StoreDirItem.value(fullPath);
            QTreeWidgetItem *item_1 = new QTreeWidgetItem(QStringList(ItemInfo.fileName()));
            item_1->setIcon(0, ItemInfo.isDir() ? QIcon(":/dir.png") : QIcon(":/file.png"));

            int j ;
            for(j=0 ; j < item->childCount() ; j++)
            {
                if(!QString::compare(ItemInfo.fileName() , item->child(j)->text(0))){
                    break ;
                }
            }

            if(j == item->childCount())
            {
                item->addChild(item_1);
                m_StoreDirItem.insert(ItemInfo.absoluteFilePath().replace("/","\\") , item_1);
            }
        }
    }
    else if(ItemInfo.isFile())
    {
        qDebug(":::::::I am a file.\n");

        QString fileFullPath = ItemInfo.filePath();
        QString filename = ItemInfo.fileName();

        QString file_path ;

        file_path = ItemInfo.absolutePath().replace("/","\\");

        qDebug("::::::file_path = %s\n" , file_path.toLatin1().data());

        if(!m_StoreDirItem.contains(file_path))
        {
            qDebug("----not contains----\n");
            QTreeWidgetItem *item = new QTreeWidgetItem(ui->twLocalFile , QStringList(file_path));
            QTreeWidgetItem *item_1 = new QTreeWidgetItem(item , QStringList(filename));
            item->addChild(item_1);

            m_StoreDirItem.insert(file_path , item);
        }
        else
        {

            qDebug("---contains---\n");
            QTreeWidgetItem *item = m_StoreDirItem.value(file_path);
            QTreeWidgetItem *item_1 = new QTreeWidgetItem(QStringList(filename));
            item_1->setIcon(0, ItemInfo.isDir() ? QIcon(":/dir.png") : QIcon(":/file.png"));
            item_1->setText(1, QString::number(ItemInfo.size()));
            item_1->setText(2, ItemInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss"));
            item_1->setText(3, ItemInfo.owner());

            int j ;
            for( j = 0 ; j < item->childCount() ; j++)
            {
                if(!QString::compare(filename , item->child(j)->text(0)))
                {
                    break ;
                }
            }

            qDebug("---j = %d and childCount = %d \n" , j , item->childCount());
            if(j == item->childCount())
            {
                item->addChild(item_1);
            }
        }
    }

    return ;
}

void MainWindow::selectItem(QTreeWidgetItem *item, int)
{
    const QString itemFullPath = getItemFullPath(item);
    m_strCurrentLocalPath = itemFullPath;
    QFileInfo localFileInfo(m_strCurrentLocalPath);
    if (localFileInfo.isDir() || localFileInfo.completeBaseName().compare(QObject::tr("我的电脑")) == 0)
    {

        qDebug("send signal and itemFullPath = %s ... \n" , itemFullPath.toLatin1().data());
        emit sendToDirScan(itemFullPath);
    }
    else if (localFileInfo.isFile())//! 文件上传
    {
        QString strFileName = localFileInfo.fileName();
        m_strCurrentLocalPath = localFileInfo.absolutePath();
        m_strCurrentLocalFile = itemFullPath;
        m_strCurrentRemoteFile = m_strCurrentRemotePath + "/" + strFileName;
        UploadFile(ToFTPEncoding(m_strCurrentLocalFile), m_strCurrentRemoteFile, false);
    }

    return ;
}

//! 更新进度条
void MainWindow::updateDataTransferProgress(qint64 nReadBytes, qint64 nTotalBytes)
{
    ui->barProcess->setMaximum(nTotalBytes);
    ui->barProcess->setValue(nReadBytes);
}

void MainWindow::FTPCommandStart(int error)
{
    switch (m_ftp->currentCommand())
    {
    case QFtp::ConnectToHost:
        ui->textMsg->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                                 .append("--")
                                 .append(QObject::tr("Connectint to Host!")));
        break;
    case QFtp::LoggedIn:
        ui->textMsg->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                                 .append("--")
                                 .append(QObject::tr("Logging to Host!")));
        break;
    case QFtp::Get:
        ui->textMsg->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                                 .append("--")
                                 .append(QObject::tr("Ready to download...!")));
        break;
    case QFtp::Put:
        ui->textMsg->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                                 .append("--")
                                 .append(QObject::tr("Ready to upload...!")));
        break;
    case QFtp::Remove:
        ui->textMsg->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                                 .append("--")
                                 .append(QObject::tr("Ready to delete!")));
        break;
    default:
        break;
    }
}

//! 处理FTP命令
void MainWindow::FTPCommandFinished(int nCommand, bool error)
{
    qDebug() << m_ftp->currentCommand();
    switch (m_ftp->currentCommand())
    {
    case QFtp::ConnectToHost:
        if (error)
        {
            ui->textMsg->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                                .append(QString(tr("Connect Error %1"))
                                .arg(m_ftp->errorString())));
        }
        break;
    case QFtp::LoggedIn:
        if (error)
        {
            ui->textMsg->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                                .append(QString(tr("Login Error %1"))
                                .arg(m_ftp->errorString())));
        }
        else
        {
            ui->textMsg->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                                     .append("--")
                                     .append(QObject::tr("Login success!")));
            ui->lneRemotePath->setText("/");
            m_ftp->list();
        }
        break;
    case QFtp::Get:
        m_nFtpTransType = 1;
        if (error)
        {
            if (m_bFtpStop)
            {
                ui->textMsg->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                                         .append("--")
                                         .append(QObject::tr("Stop transmission!")));
            }
            else
            {
                ui->textMsg->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                                         .append("--")
                                         .append(QObject::tr("Download fail %1")
                                                 .arg(m_ftp->errorString())));
            }
        }
        else
        {
            ui->textMsg->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                                     .append("--")
                                     .append(QObject::tr("Download success!")));
            m_file->close();
            ui->lblSatus->setHidden(true);
            ui->barProcess->setHidden(true);
            ui->lblProcessInfo->setHidden(true);
            ui->btStop->setHidden(true);
        }
        break;
    case QFtp::Put:
        m_nFtpTransType = 0;
        if (error)
        {
            ui->textMsg->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                                .append(QString(tr("Upload Error %1"))
                                .arg(m_ftp->errorString())));
        }
        else if (m_bFtpStop)
        {
            ui->textMsg->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                                     .append("--")
                                     .append(QObject::tr("Stop transmission!")));
        }
        else
        {
            ui->textMsg->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                                     .append("--")
                                     .append(QObject::tr("Upload success!")));
            m_file->close();
            ui->lblSatus->setHidden(true);
            ui->barProcess->setHidden(true);
            ui->lblProcessInfo->setHidden(true);
            ui->btStop->setHidden(true);
        }
        break;
    case QFtp::Remove:
        if (error)
        {
            ui->textMsg->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                                .append(QString(tr("Delete fail %1"))
                                .arg(m_ftp->errorString())));
        }
        else
        {
            ui->textMsg->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                                     .append("--")
                                     .append(QObject::tr("Delete success!")));
        }
        break;
    default:
        break;
    }
}

void MainWindow::ProcessItem(QTreeWidgetItem *item, int)
{
    //! 打开一个目录
    QString strName = ToFTPEncoding(item->text(0));
    qDebug() << FromFTPEncoding(strName);
    if (m_mapDirctory.value(strName) && strName.compare("..") != 0)
    {
        ui->twRemoteFile->clear();
        m_mapDirctory.clear();
        if (m_strCurrentRemotePath.compare("/") != 0)
            m_strCurrentRemotePath += "/";
        m_strCurrentRemotePath += strName;
        m_ftp->cd(strName);
        ui->lneRemotePath->setText(FromFTPEncoding(m_strCurrentRemotePath));
        m_ftp->list();
    }
    //! 返回上级目录
    else if (strName.compare("..") == 0)
    {
        m_mapDirctory.clear();
        ui->twRemoteFile->clear();
        m_ftp->cd(strName);
        m_strCurrentRemotePath = m_strCurrentRemotePath.left(m_strCurrentRemotePath.lastIndexOf("/"));
        qDebug() << m_strCurrentRemotePath;
        if (m_strCurrentRemotePath.isEmpty())
        {
            m_strCurrentRemotePath += "/";
        }
        ui->lneRemotePath->setText(FromFTPEncoding(m_strCurrentRemotePath));
        m_ftp->list();
    }
    //! 如果是文件双击下载
    else
    {
        m_strCurrentRemoteFile = strName;
        m_strCurrentLocalFile = m_strCurrentLocalPath+"\\"+FromFTPEncoding(strName);
        DownloadFile(m_strCurrentRemoteFile, m_strCurrentLocalFile, false);
    }
}

void MainWindow::HandleFtpInterruput()
{
    m_bFtpStop = !m_bFtpStop;
    if (m_bFtpStop)
    {
        ui->btStop->setText(tr("Start"));
        ui->lblProcessInfo->setText(tr("Stop..."));
        m_ftp->abort();
    }
    else
    {
        ui->btStop->setText(tr("Stop"));
        ui->textMsg->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                                 .append("--")
                                 .append(QObject::tr("Resume transmission!")));
        switch (m_nFtpTransType)
        {
        case 0:
            ui->lblProcessInfo->setText(tr("ReseUpload..."));
            UploadFile(ToFTPEncoding(m_strCurrentLocalFile), m_strCurrentRemoteFile, true);
            break;
        case 1:
            ui->lblProcessInfo->setText(tr("ReseDownload..."));
            DownloadFile(m_strCurrentRemoteFile, m_strCurrentLocalFile, true);
            break;
        default:
            break;
        }
    }

}

void MainWindow::HandleTwLocalCustomContextMenuRequested(QPoint pos)
{
    QTreeWidgetItem* curItem=ui->twLocalFile->itemAt(pos);  //获取当前被点击的节点

    const QString strFilePath = getItemFullPath(curItem);
    m_strLocalSelectFile = strFilePath;
    qDebug() << strFilePath;

    curItem->setData(0, Qt::UserRole, QFileInfo(m_strLocalSelectFile).isDir() ? 0 : 1);

    if(curItem==NULL)return;           //! 这种情况是右键的位置不在treeItem的范围内，即在空白位置右击
    QVariant var = curItem->data(0,Qt::UserRole);

    m_localMenu->clear();
    if(0 == var)      //! data(...)返回的data已经在之前建立节点时用setdata()设置好
    {
       m_localMenu->addAction(new QAction("Open", this));
       m_localMenu->exec(QCursor::pos());//! 弹出右键菜单，菜单位置为光标位置
    }
    else if (1 == var)
    {
        m_localMenu->addAction(new QAction("ReseUpload", this));
        m_localMenu->addAction(new QAction("Flush", this));
        m_localMenu->exec(QCursor::pos());//! 弹出右键菜单，菜单位置为光标位置
    }
}

void MainWindow::HandleLocalMenuTrigger(QAction *action)
{
    QString strFileName = QFileInfo(m_strLocalSelectFile).fileName();
    m_strCurrentRemoteFile = m_strCurrentRemotePath+"/"+strFileName;
    m_strCurrentLocalFile = m_strLocalSelectFile;

    if (action->iconText().compare("ReseUpload", Qt::CaseSensitive) == 0)
    {
        qDebug() << "ReseUpload";
        UploadFile(ToFTPEncoding(m_strCurrentLocalFile), m_strCurrentRemoteFile, true);
    }
    else if (action->iconText().compare("Open", Qt::CaseSensitive) == 0)
    {
        qDebug() << "Open";
        emit sendToDirScan(m_strLocalSelectFile);
    }
}

void MainWindow::HandleTwRemoteCustomContextMenuRequested(QPoint pos)
{
    QTreeWidgetItem* curItem=ui->twRemoteFile->itemAt(pos);  //获取当前被点击的节点
    m_strRemoteSelectFile = m_strCurrentRemotePath + "/" + getItemFullPath(curItem);
    qDebug() << m_strRemoteSelectFile;
    if (curItem==NULL)return;           //这种情况是右键的位置不在treeItem的范围内，即在空白位置右击

    curItem->setData(0, Qt::UserRole, QFileInfo(m_strLocalSelectFile).isDir() ? 0 : 1);

    QVariant var = curItem->data(0,Qt::UserRole);

    m_remoteMenu->clear();
    if (0 == var)      //data(...)返回的data已经在之前建立节点时用setdata()设置好
    {
       m_remoteMenu->addAction(new QAction("Entry the folder", this));
       m_remoteMenu->exec(QCursor::pos());//弹出右键菜单，菜单位置为光标位置
    }
    else if (1 == var)
    {
        m_remoteMenu->addAction(new QAction("ReseDownload", this));//往菜单内添加QAction   该action在前面用设计器定义了
        m_remoteMenu->addAction(new QAction("Flush", this));
        m_remoteMenu->addAction(new QAction("Delete", this));
        m_remoteMenu->exec(QCursor::pos());//弹出右键菜单，菜单位置为光标位置
    }
}

void MainWindow::HandleRemoteMenuTrigger(QAction *action)
{

    if (action->iconText().compare("Flush", Qt::CaseSensitive) == 0)
    {
        m_mapDirctory.clear();
        ui->twRemoteFile->clear();
        m_ftp->list(m_strCurrentRemotePath);
    }
    else if (action->iconText().compare("Delete", Qt::CaseSensitive) == 0)
    {
        m_ftp->remove(m_strRemoteSelectFile);
        m_mapDirctory.clear();
        ui->twRemoteFile->clear();
        m_ftp->list(m_strCurrentRemotePath);
    }
    else if (action->iconText().compare("ReseDownload", Qt::CaseSensitive) == 0)
    {
        m_strCurrentLocalFile = m_strCurrentLocalPath + "\\" +QFileInfo(m_strRemoteSelectFile).fileName();
        m_strCurrentRemoteFile = ToFTPEncoding(m_strRemoteSelectFile);
        DownloadFile(m_strCurrentRemoteFile, m_strCurrentLocalFile, true);
    }

    return;
}

//! 下载文件
void MainWindow::DownloadFile(QString strRemoteFile, QString strLocalFile, bool isRese)
{
    ui->lblSatus->setHidden(false);
    ui->barProcess->setHidden(false);
    ui->btStop->setHidden(false);
    ui->lblProcessInfo->setHidden(false);
    ui->lblSatus->setText(FromFTPEncoding(strRemoteFile) + "      ==>      " + strLocalFile);

    m_file = new QFile(strLocalFile);
    if (!m_file->open(QIODevice::ReadWrite))
    {
        qDebug() << "open fail";
    }
    if (isRese)
    {
        ui->lblProcessInfo->setText(QObject::tr("ReseDownloading...").toStdString().c_str());
        m_ftp->rawCommand(tr("REST %1").arg(m_file->size()));
        m_ftp->m_isConLoad = true;
        m_ftp->get(strRemoteFile, m_file, QFtp::Binary);
    }
    else
    {
        ui->lblProcessInfo->setText(QObject::tr("Downloading...").toStdString().c_str());
        m_ftp->get(strRemoteFile, m_file, QFtp::Binary);
    }
}

//! 上传
//! isRese 续传
void MainWindow::UploadFile(QString strLocalFile, QString strRemoteFile, bool isRese)
{
    ui->lblSatus->setHidden(false);
    ui->barProcess->setHidden(false);
    ui->btStop->setHidden(false);
    ui->lblProcessInfo->setHidden(false);
    ui->lblSatus->setText(strLocalFile + "       ==>      " + FromFTPEncoding(strRemoteFile));
    m_file = new QFile(strLocalFile);
    if (!m_file->open(QIODevice::ReadWrite))
    {
        qDebug() << "open fail";
        return;
    }
    if (isRese)
    {
        ui->lblProcessInfo->setText(QObject::tr("ReseUploading...").toStdString().c_str());
        m_ftp->conPut(m_file, ToFTPEncoding(strRemoteFile),QFtp::Binary);
    }
    else
    {
        ui->lblProcessInfo->setText(QObject::tr("Uploading...").toStdString().c_str());
        m_ftp->put(m_file, ToFTPEncoding(strRemoteFile),QFtp::Binary);
    }
}

//! FTP编码转本地编码
QString MainWindow::FromFTPEncoding(const QString &strInput)
{
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    if (codec)
    {
        return codec->toUnicode(strInput.toLatin1());
    }
    else
    {
        return QString("");
    }
}

//! 本地编码转FTP编码
QString MainWindow::ToFTPEncoding(const QString &strOutput)
{
    QTextCodec *codec= QTextCodec::codecForName("utf-8");
    if (codec)
    {
        return QString::fromLatin1(codec->fromUnicode(strOutput));
    }
    else
    {
        return QString("");
    }
}

QString MainWindow::getItemFullPath(QTreeWidgetItem *item)
{
    if (item == NULL)
    {
        return "";
    }
    QString itemFullPath = item->text(0);

    while(item->parent() != NULL && QString::compare(item->parent()->text(0) , QString(tr("我的电脑")))){
        qDebug("------parentPath = %s\n" , item->parent()->text(0).toLatin1().data());
        itemFullPath = item->parent()->text(0).replace("/","") + "\\" + itemFullPath ;
        item = item->parent();
    }

    if(!QString::compare(item->text(0) , QString(tr("我的电脑")))){
        return QString(tr("我的电脑"));
    }

    return itemFullPath ;
}
