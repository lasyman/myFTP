#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QFile>
#include <QNetworkAccessManager>
#include <QTreeWidget>
#include <QUrl>
#include <QtGui>
#include <QDebug>
#include "qftp.h"
#include "qurlinfo.h"
#include <QIcon>
#include "dirscan.h"
#include <QHash>
#include "dirscan.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString getItemFullPath(QTreeWidgetItem* item);


private:
    Ui::MainWindow *ui;
    void init();
    void InitUiElements();
    void DownloadFile(QString strRemoteFile, QString strLocalFile);
    void UploadFile(QString strLocalFile, QString strRemoteFile);
    QString FromFTPEncoding(const QString &strInput);
    QString ToFTPEncoding(const QString &strOutput);

    QString     m_strCurrentRemotePath;
    QString     m_strCurrentLocalPath;
    QFtp        *m_ftp;
    QMap<QString,bool> m_mapDirctory;
    QFile       *m_file;
    DirScan     *dirScan;
    QList<QTreeWidgetItem *> root;
    QString rootPath;
    QHash<QString/*path*/, QTreeWidgetItem *> m_StoreDirItem;
    QMenu       *m_localMenu;
    QAction     *m_conUpload;
    QString     m_strLocalSelectFile;
    QString     m_strRemoteSelectFile;
    QMenu       *m_remoteMenu;
    QAction     *m_reDownload;
    QAction     *m_flush;
    QAction     *m_delete;


    void CreateActions();
    
private slots:
    void TriggerConnectClicked();
    void AddToList(const QUrlInfo &urlInfo);
    void AddItem(const QString &strRootPath, const QFileInfo &ItemInfo , const int k );
    void selectItem(QTreeWidgetItem * item, int);
    void updateDataTransferProgress(qint64 nReadBytes, qint64 nTotalBytes);
    void FTPCommandFinished(int nCommand, bool error);
    void ProcessItem(QTreeWidgetItem *item, int);
    void HandleTwRemoteCustomContextMenuRequested(QPoint pos);
    void HandleTwLocalCustomContextMenuRequested(QPoint pos);
    void HandleLocalMenuTrigger(QAction *action);
    void HandleRemoteMenuTrigger(QAction *action);

signals:
    void sendToDirScan(const QString &selectedItem );
};

#endif // MAINWINDOW_H
