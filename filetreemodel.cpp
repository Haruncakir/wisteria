#include "filetreemodel.h"
#include <QDir>
#include <QFileInfo>
#include <QMimeData>
#include <QUrl>
#include <QDebug>

FileTreeModel::FileTreeModel(QObject *parent)
    : QAbstractItemModel(parent)
    , m_rootPath("")
{
    m_rootItem = new FileTreeItem("", "", true, nullptr);
}

FileTreeModel::~FileTreeModel()
{
    delete m_rootItem;
}

QString FileTreeModel::rootPath() const
{
    return m_rootPath;
}

void FileTreeModel::setRootPath(const QString &path) {
    QString localPath = path.startsWith("file:///")
        ? QUrl(path).toLocalFile()
        : path;
    if (m_rootPath != localPath) {
        qDebug() << "Setting root path to:" << localPath;
        beginResetModel();
        m_rootPath = localPath;
        delete m_rootItem;
        m_rootItem = new FileTreeItem("", "", true, nullptr);

        qDebug() << "Setting root path to:" << localPath;

        if (!localPath.isEmpty()) {
            QDir dir(localPath);
            qDebug() << "Directory exists?" << dir.exists();
            qDebug() << "Entries found:" << dir.entryInfoList().count();
            if (dir.exists()) {
                dir.setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
                dir.setSorting(QDir::DirsFirst | QDir::Name);
                QFileInfoList fileInfoList = dir.entryInfoList();
                qDebug() << "Found" << fileInfoList.size() << "entries in directory";

                for (const QFileInfo &fileInfo : fileInfoList) {
                    QString absolutePath = fileInfo.absoluteFilePath();
                    QString fileName = fileInfo.fileName();
                    bool isDir = fileInfo.isDir();

                    FileTreeItem *childItem = new FileTreeItem(absolutePath, fileName, isDir, m_rootItem);
                    m_rootItem->appendChild(childItem);

                    // Check if directories have children
                    if (isDir) {
                        QDir childDir(absolutePath);
                        childDir.setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
                        if (!childDir.entryInfoList().isEmpty()) {
                            childItem->setHasChildren(true);
                        }
                    }
                }
            } else {
                qDebug() << "Directory does not exist:" << localPath;
            }
        }

        endResetModel();
        emit rootPathChanged();
    }
}

QModelIndex FileTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    FileTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<FileTreeItem*>(parent.internalPointer());

    FileTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex FileTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    FileTreeItem *childItem = static_cast<FileTreeItem*>(index.internalPointer());
    FileTreeItem *parentItem = childItem->parentItem();

    if (parentItem == m_rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int FileTreeModel::rowCount(const QModelIndex &parent) const
{
    FileTreeItem *parentItem;

    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<FileTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int FileTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1; // We only show the file name column
}

QVariant FileTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    FileTreeItem *item = static_cast<FileTreeItem*>(index.internalPointer());

    switch (role) {
    case FileNameRole:
        return item->fileName();
    case FilePathRole:
        return item->filePath();
    case IsDirRole:
        return item->isDir();
    case HasChildrenRole:
        return item->hasChildren();
    }

    return QVariant();
}

bool FileTreeModel::hasChildren(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return m_rootItem->childCount() > 0;

    FileTreeItem *item = static_cast<FileTreeItem*>(parent.internalPointer());
    return item->hasChildren();
}

bool FileTreeModel::canFetchMore(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return false;

    FileTreeItem *item = static_cast<FileTreeItem*>(parent.internalPointer());
    return item->isDir() && !item->isLoaded();
}

void FileTreeModel::fetchMore(const QModelIndex &parent) {
    if (!parent.isValid())
        return;

    FileTreeItem *item = static_cast<FileTreeItem*>(parent.internalPointer());
    if (!item->isDir() || item->isLoaded())
        return;

    item->setLoaded(true);

    QString dirPath = item->filePath();
    QDir dir(dirPath);

    // Include hidden files/directories in the filter
    dir.setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden);
    dir.setSorting(QDir::DirsFirst | QDir::Name);

    QFileInfoList fileInfoList = dir.entryInfoList();

    if (fileInfoList.isEmpty())
        return;

    qDebug() << "Fetching children for:" << dirPath;
    qDebug() << "Found" << fileInfoList.size() << "items";

    beginInsertRows(parent, 0, fileInfoList.size() - 1);

    for (const QFileInfo &fileInfo : fileInfoList) {
        QString absolutePath = fileInfo.absoluteFilePath();
        QString fileName = fileInfo.fileName();
        bool isDir = fileInfo.isDir();

        FileTreeItem *childItem = new FileTreeItem(absolutePath, fileName, isDir, item);
        item->appendChild(childItem);

        // Set hasChildren for directories
        if (isDir) {
            QDir childDir(absolutePath);
            childDir.setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden);
            childItem->setHasChildren(!childDir.entryInfoList().isEmpty());
        }
    }

    endInsertRows();
}

QHash<int, QByteArray> FileTreeModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[FileNameRole] = "fileName";
    roles[FilePathRole] = "filePath";
    roles[IsDirRole] = "isDir";
    roles[HasChildrenRole] = "hasChildren";
    return roles;
}

// FileTreeItem implementation

FileTreeItem::FileTreeItem(const QString &filePath, const QString &fileName, bool isDir, FileTreeItem *parent)
    : m_filePath(filePath)
    , m_fileName(fileName)
    , m_isDir(isDir)
    , m_isLoaded(false)
    , m_hasChildren(false)
    , m_parentItem(parent)
{
    m_hasChildren = isDir;
}

FileTreeItem::~FileTreeItem()
{
    qDeleteAll(m_childItems);
}

void FileTreeItem::appendChild(FileTreeItem *item)
{
    m_childItems.append(item);
    m_hasChildren = true;
}

FileTreeItem *FileTreeItem::child(int row) const
{
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}

int FileTreeItem::childCount() const
{
    return m_childItems.size();
}

int FileTreeItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<FileTreeItem*>(this));
    return 0;
}

FileTreeItem *FileTreeItem::parentItem() const
{
    return m_parentItem;
}

QString FileTreeItem::fileName() const
{
    return m_fileName;
}

QString FileTreeItem::filePath() const
{
    return m_filePath;
}

bool FileTreeItem::isDir() const
{
    return m_isDir;
}

bool FileTreeItem::isLoaded() const
{
    return m_isLoaded;
}

void FileTreeItem::setLoaded(bool loaded)
{
    m_isLoaded = loaded;
}

bool FileTreeItem::hasChildren() const
{
    return m_hasChildren;
}

void FileTreeItem::setHasChildren(bool hasChildren)
{
    m_hasChildren = hasChildren;
}
