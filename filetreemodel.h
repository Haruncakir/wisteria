#ifndef FILETREEMODEL_H
#define FILETREEMODEL_H

#include <QAbstractItemModel>
#include <QList>

// Forward declarations
class FileTreeItem;

class FileTreeModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_PROPERTY(QString rootPath READ rootPath WRITE setRootPath NOTIFY rootPathChanged)

public:
    enum Roles {
        FileNameRole = Qt::UserRole + 1,
        FilePathRole,
        IsDirRole,
        HasChildrenRole
    };

    explicit FileTreeModel(QObject *parent = nullptr);
    ~FileTreeModel();

    // Property accessor methods
    QString rootPath() const;
    Q_INVOKABLE void setRootPath(const QString &path);

    // QAbstractItemModel implementation
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;
    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;
    QHash<int, QByteArray> roleNames() const override;

signals:
    void rootPathChanged();

private:
    void loadDirectory(FileTreeItem *parentItem, const QString &path);

    QString m_rootPath;
    FileTreeItem *m_rootItem;
};

// Helper class to represent items in the tree
class FileTreeItem
{
public:
    explicit FileTreeItem(const QString &filePath, const QString &fileName, bool isDir, FileTreeItem *parent);
    ~FileTreeItem();

    void appendChild(FileTreeItem *child);
    FileTreeItem *child(int row) const;
    int childCount() const;
    int row() const;
    FileTreeItem *parentItem() const;

    QString fileName() const;
    QString filePath() const;
    bool isDir() const;
    bool isLoaded() const;
    void setLoaded(bool loaded);
    bool hasChildren() const;
    void setHasChildren(bool hasChildren);

private:
    QString m_filePath;
    QString m_fileName;
    bool m_isDir;
    bool m_isLoaded;
    bool m_hasChildren;
    FileTreeItem *m_parentItem;
    QList<FileTreeItem*> m_childItems;
};

#endif // FILETREEMODEL_H
