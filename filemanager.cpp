#include "filemanager.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QTextStream>
#include <QDebug>

FileManager::FileManager(QObject *parent)
    : QObject(parent)
    , m_activeFileIndex(-1)
    , m_explorerVisible(false)
    , m_currentFolder("")
{
    // Connect signals for automatic updates
}

FileManager::~FileManager()
{
    // Clean up resources if needed
}

bool FileManager::openFile(const QString &filePath)
{
    // Check if the file exists
    if (!fileExists(filePath)) {
        emit errorOccurred(tr("File does not exist: %1").arg(filePath));
        return false;
    }

    // Check if the file is already open
    int existingIndex = findFileIndex(filePath);
    if (existingIndex != -1) {
        setActiveFileIndex(existingIndex);
        return true;
    }

    // Create a new TextDocument
    QSharedPointer<TextDocument> document = QSharedPointer<TextDocument>::create(this);
    if (!document->loadFile(filePath)) {
        emit errorOccurred(tr("Failed to load file: %1").arg(filePath));
        return false;
    }

    // Connect document signals
    connect(document.data(), &TextDocument::contentChanged, [this, document]() {
        int index = m_documents.indexOf(document);
        if (index != -1) {
            handleDocumentChange(index);
        }
    });

    connect(document.data(), &TextDocument::dirtyChanged, [this, document](bool isDirty) {
        int index = m_documents.indexOf(document);
        if (index != -1) {
            emit fileDirtyChanged(index, isDirty);
        }
    });

    // Add to our collections
    m_documents.append(document);
    m_filePaths.append(filePath);
    m_fileNames.append(extractFileName(filePath));

    // Set as active file
    setActiveFileIndex(m_documents.size() - 1);

    // Notify of changes
    emit openFilesChanged();

    return true;
}

bool FileManager::closeFile(int index)
{
    // Validate index
    if (index < 0 || index >= m_documents.size()) {
        emit errorOccurred(tr("Invalid file index: %1").arg(index));
        return false;
    }

    // Check if file has unsaved changes
    if (m_documents[index]->isDirty()) {
        // In a real application, you would show a dialog to the user
        // For this example, we'll just return false
        emit errorOccurred(tr("File has unsaved changes: %1").arg(m_fileNames[index]));
        return false;
    }

    // Update active index if needed
    if (m_activeFileIndex == index) {
        if (m_documents.size() > 1) {
            // Set to previous tab or the first one
            m_activeFileIndex = qMin(index, m_documents.size() - 2);
        } else {
            // No tabs left
            m_activeFileIndex = -1;
        }
        emit activeFileIndexChanged();
    } else if (m_activeFileIndex > index) {
        // Active tab is after the one we're closing
        m_activeFileIndex--;
        emit activeFileIndexChanged();
    }

    // Remove from collections
    m_documents.removeAt(index);
    m_filePaths.removeAt(index);
    m_fileNames.removeAt(index);

    // Notify of changes
    emit openFilesChanged();

    return true;
}

QString FileManager::getFileContent(int index) const
{
    // Validate index
    if (index < 0 || index >= m_documents.size()) {
        return "";
    }

    return m_documents[index]->content();
}

bool FileManager::saveFile(int index)
{
    // Validate index
    if (index < 0 || index >= m_documents.size()) {
        emit errorOccurred(tr("Invalid file index: %1").arg(index));
        return false;
    }

    // Save the file
    if (!m_documents[index]->saveFile(m_filePaths[index])) {
        emit errorOccurred(tr("Failed to save file: %1").arg(m_filePaths[index]));
        return false;
    }

    return true;
}

bool FileManager::saveFileAs(int index, const QString &filePath)
{
    // Validate index
    if (index < 0 || index >= m_documents.size()) {
        emit errorOccurred(tr("Invalid file index: %1").arg(index));
        return false;
    }

    // Save the file
    if (!m_documents[index]->saveFile(filePath)) {
        emit errorOccurred(tr("Failed to save file: %1").arg(filePath));
        return false;
    }

    // Update file path and name
    m_filePaths[index] = filePath;
    m_fileNames[index] = extractFileName(filePath);
    emit openFilesChanged();

    return true;
}

bool FileManager::isFileDirty(int index) const
{
    // Validate index
    if (index < 0 || index >= m_documents.size()) {
        return false;
    }

    return m_documents[index]->isDirty();
}

QString FileManager::getFileName(int index) const
{
    // Validate index
    if (index < 0 || index >= m_fileNames.size()) {
        return "";
    }

    return m_fileNames[index];
}

QString FileManager::getFilePath(int index) const
{
    // Validate index
    if (index < 0 || index >= m_filePaths.size()) {
        return "";
    }

    return m_filePaths[index];
}

void FileManager::setFileContent(int index, const QString &content)
{
    // Validate index
    if (index < 0 || index >= m_documents.size()) {
        emit errorOccurred(tr("Invalid file index: %1").arg(index));
        return;
    }

    m_documents[index]->setContent(content);
}

bool FileManager::createNewFile(const QString &fileName)
{
    // Create a full path
    QString filePath = m_currentFolder + "/" + fileName;

    // Check if file already exists
    if (fileExists(filePath)) {
        emit errorOccurred(tr("File already exists: %1").arg(filePath));
        return false;
    }

    // Create empty file
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        emit errorOccurred(tr("Failed to create file: %1").arg(filePath));
        return false;
    }
    file.close();

    // Open the newly created file
    return openFile(filePath);
}

QStringList FileManager::openFiles() const
{
    return m_fileNames;
}

int FileManager::activeFileIndex() const
{
    return m_activeFileIndex;
}

void FileManager::setActiveFileIndex(int index)
{
    if (m_activeFileIndex != index && index >= -1 && index < m_documents.size()) {
        m_activeFileIndex = index;
        emit activeFileIndexChanged();
    }
}

bool FileManager::explorerVisible() const
{
    return m_explorerVisible;
}

void FileManager::setExplorerVisible(bool visible)
{
    if (m_explorerVisible != visible) {
        m_explorerVisible = visible;
        emit explorerVisibleChanged();
    }
}

QString FileManager::currentFolder() const
{
    return m_currentFolder;
}

void FileManager::setCurrentFolder(const QString &folder)
{
    if (m_currentFolder != folder) {
        m_currentFolder = folder;
        emit currentFolderChanged();
    }
}

QString FileManager::extractFileName(const QString &filePath) const
{
    return QFileInfo(filePath).fileName();
}

bool FileManager::fileExists(const QString &filePath) const
{
    return QFile::exists(filePath);
}

int FileManager::findFileIndex(const QString &filePath) const
{
    return m_filePaths.indexOf(filePath);
}

bool FileManager::handleDocumentChange(int index)
{
    if (index >= 0 && index < m_documents.size()) {
        emit fileContentChanged(index);
        return true;
    }
    return false;
}
