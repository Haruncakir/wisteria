#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QStringList>
#include <QSharedPointer>
#include <QMap>
#include "textdocument.h"

class FileManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList openFiles READ openFiles NOTIFY openFilesChanged)
    Q_PROPERTY(int activeFileIndex READ activeFileIndex WRITE setActiveFileIndex NOTIFY activeFileIndexChanged)
    Q_PROPERTY(bool explorerVisible READ explorerVisible WRITE setExplorerVisible NOTIFY explorerVisibleChanged)
    Q_PROPERTY(QString currentFolder READ currentFolder WRITE setCurrentFolder NOTIFY currentFolderChanged)

public:
    explicit FileManager(QObject *parent = nullptr);
    ~FileManager();

    // QML invokable methods
    Q_INVOKABLE bool openFile(const QString &filePath);
    Q_INVOKABLE bool closeFile(int index);
    Q_INVOKABLE QString getFileContent(int index) const;
    Q_INVOKABLE bool saveFile(int index);
    Q_INVOKABLE bool saveFileAs(int index, const QString &filePath);
    Q_INVOKABLE bool isFileDirty(int index) const;
    Q_INVOKABLE QString getFileName(int index) const;
    Q_INVOKABLE QString getFilePath(int index) const;
    Q_INVOKABLE void setFileContent(int index, const QString &content);
    Q_INVOKABLE bool createNewFile(const QString &fileName);

    // Property accessors
    QStringList openFiles() const;
    int activeFileIndex() const;
    void setActiveFileIndex(int index);
    bool explorerVisible() const;
    void setExplorerVisible(bool visible);
    QString currentFolder() const;
    void setCurrentFolder(const QString &folder);

signals:
    void openFilesChanged();
    void activeFileIndexChanged();
    void explorerVisibleChanged();
    void currentFolderChanged();
    void fileContentChanged(int index);
    void fileDirtyChanged(int index, bool isDirty);
    void errorOccurred(const QString &error);

private:
    QStringList m_fileNames;
    QStringList m_filePaths;
    QList<QSharedPointer<TextDocument>> m_documents;
    int m_activeFileIndex;
    bool m_explorerVisible;
    QString m_currentFolder;

    // Helper methods
    QString extractFileName(const QString &filePath) const;
    bool fileExists(const QString &filePath) const;
    int findFileIndex(const QString &filePath) const;
    bool handleDocumentChange(int index);
};

#endif // FILEMANAGER_H
