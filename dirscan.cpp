#include "dirscan.h"

int DirScan::k = 0;
DirScan::DirScan(QObject *parent) : QObject(parent)
{

}
void DirScan::AsncScan(const QString strPath)
{
    QtConcurrent::run(this, &DirScan::Scan, strPath);
}

void DirScan::Scan(const QString strPath)
{
    //! 第一次肯定是显示磁盘驱动器
    if(k == 0 && !QString::compare(strPath , QString(tr("我的电脑"))))
    {
        QFileInfoList drivers = QDir::drives();
        foreach (QFileInfo driverInfo, drivers)
        {
            if (driverInfo.isReadable())
            {
                qDebug("-----sendBack:%s\n" , driverInfo.filePath().toLatin1().data());
                emit ItemScaned(strPath , driverInfo , k);
            }
        }
        k++ ;

    //! 其它就是盘符下面的内容
    }
    else
    {
        QDir dir(strPath);
        dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
        qDebug() << "+++++++++++++++++" << strPath;
        if(dir.exists())
        {
            k ++ ;
            QFileInfoList fileList = dir.entryInfoList();
            foreach (QFileInfo fileInfo, fileList)
            {
                qDebug() << "Scan==============" << fileInfo.filePath();
                emit ItemScaned(strPath, fileInfo , k);
            }
        }
    }

    return ;
}
