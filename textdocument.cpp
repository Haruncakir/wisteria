#include "textdocument.h"
#include "syntaxhighlighter.h"
#include <QFile>
#include <QTextStream>
#include <QTextCursor>
#include <QTextBlock>
#include <QUndoCommand>
#include <QFileInfo>
#include <QDebug>

// Text edit command for undo/redo functionality
class TextDocument::TextEditCommand : public QUndoCommand
{
public:
    TextEditCommand(QTextDocument *document, int position, const QString &textToRemove, const QString &textToInsert, TextDocument *parent)
        : QUndoCommand()
        , m_document(document)
        , m_position(position)
        , m_textToRemove(textToRemove)
        , m_textToInsert(textToInsert)
        , m_parent(parent)
    {
    }

    void undo() override
    {
        QTextCursor cursor(m_document);
        cursor.setPosition(m_position);
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, m_textToInsert.length());
        cursor.removeSelectedText();
        cursor.insertText(m_textToRemove);
        m_parent->contentChanged();
    }

    void redo() override
    {
        QTextCursor cursor(m_document);
        cursor.setPosition(m_position);
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, m_textToRemove.length());
        cursor.removeSelectedText();
        cursor.insertText(m_textToInsert);
        m_parent->contentChanged();
    }

private:
    QTextDocument *m_document;
    int m_position;
    QString m_textToRemove;
    QString m_textToInsert;
    TextDocument *m_parent;
};

TextDocument::TextDocument(QObject *parent)
    : QObject(parent)
    , m_document(new QTextDocument(this))
    , m_undoStack(new QUndoStack(this))
    , m_isDirty(false)
    , m_highlighter(nullptr)
{
    // Connect signal to detect dirty state
    connect(m_undoStack, &QUndoStack::cleanChanged, this, [this](bool clean) {
        markAsDirty(!clean);
    });

    // Set initial state as clean
    m_undoStack->setClean();
}

TextDocument::~TextDocument()
{
    // Cleanup resources
    if (m_highlighter) {
        // m_highlighter is automatically deleted when its parent m_document is deleted
        m_highlighter = nullptr;
    }

    delete m_document;
    delete m_undoStack;
}

bool TextDocument::loadFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream in(&file);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    in.setEncoding(QStringConverter::Utf8);
#else
    in.setCodec("UTF-8");
#endif
    QString fileContent = in.readAll();
    file.close();

    m_filePath = filePath;
    m_originalContent = fileContent;
    m_document->setPlainText(fileContent);

    // Reset undo stack
    resetUndoStack();
    markAsDirty(false);
    updateLineCount();

    // Apply syntax highlighting based on file extension
    QFileInfo fileInfo(filePath);
    applySyntaxHighlighting(fileInfo.suffix());

    emit contentChanged();

    return true;
}

bool TextDocument::saveFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream out(&file);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    out.setEncoding(QStringConverter::Utf8);
#else
    out.setCodec("UTF-8");
#endif
    out << m_document->toPlainText();
    file.close();

    m_filePath = filePath;
    m_originalContent = m_document->toPlainText();

    // Mark as clean
    m_undoStack->setClean();
    markAsDirty(false);

    // If the file extension has changed, update syntax highlighting
    QFileInfo fileInfo(filePath);
    applySyntaxHighlighting(fileInfo.suffix());

    return true;
}

QString TextDocument::content() const
{
    return m_document->toPlainText();
}

void TextDocument::setContent(const QString &content)
{
    if (m_document->toPlainText() != content) {
        // Create an edit command to track this change
        QString oldContent = m_document->toPlainText();

        // Push to undo stack
        m_undoStack->push(new TextEditCommand(m_document, 0, oldContent, content, this));

        // Set the content directly to avoid recursion
        m_document->setPlainText(content);

        updateLineCount();
        emit contentChanged();
    }
}

bool TextDocument::isDirty() const
{
    return m_isDirty;
}

int TextDocument::lineCount() const
{
    return m_document->lineCount();
}

void TextDocument::undo()
{
    m_undoStack->undo();
}

void TextDocument::redo()
{
    m_undoStack->redo();
}

void TextDocument::insertText(int position, const QString &text)
{
    if (position < 0 || position > m_document->characterCount()) {
        return;
    }

    // Create a command for undo/redo
    m_undoStack->push(new TextEditCommand(m_document, position, "", text, this));

    // The actual insertion happens in the command's redo() method
}

void TextDocument::removeText(int position, int length)
{
    if (position < 0 || position + length > m_document->characterCount()) {
        return;
    }

    // Get the text to be removed for undo purposes
    QTextCursor cursor(m_document);
    cursor.setPosition(position);
    cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, length);
    QString textToRemove = cursor.selectedText();

    // Create a command for undo/redo
    m_undoStack->push(new TextEditCommand(m_document, position, textToRemove, "", this));

    // The actual removal happens in the command's redo() method
}

void TextDocument::replaceText(int position, int length, const QString &text)
{
    if (position < 0 || position + length > m_document->characterCount()) {
        return;
    }

    // Get the text to be replaced for undo purposes
    QTextCursor cursor(m_document);
    cursor.setPosition(position);
    cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, length);
    QString textToRemove = cursor.selectedText();

    // Create a command for undo/redo
    m_undoStack->push(new TextEditCommand(m_document, position, textToRemove, text, this));

    // The actual replacement happens in the command's redo() method
}

QString TextDocument::getLine(int lineNumber) const
{
    if (lineNumber < 0 || lineNumber >= m_document->lineCount()) {
        return "";
    }

    QTextBlock block = m_document->findBlockByLineNumber(lineNumber);
    return block.text();
}

int TextDocument::getLineStart(int lineNumber) const
{
    if (lineNumber < 0 || lineNumber >= m_document->lineCount()) {
        return -1;
    }

    QTextBlock block = m_document->findBlockByLineNumber(lineNumber);
    return block.position();
}

int TextDocument::getLineLength(int lineNumber) const
{
    if (lineNumber < 0 || lineNumber >= m_document->lineCount()) {
        return -1;
    }

    QTextBlock block = m_document->findBlockByLineNumber(lineNumber);
    return block.length();
}

void TextDocument::applySyntaxHighlighting(const QString &fileExtension)
{
    // Remove any existing highlighter
    if (m_highlighter) {
        delete m_highlighter;
        m_highlighter = nullptr;
    }

    // Create a new syntax highlighter for the document
    m_highlighter = new SyntaxHighlighter(m_document);

    // Set the language based on file extension
    m_highlighter->setLanguage(fileExtension);
}

void TextDocument::updateLineCount()
{
    static int previousLineCount = 0;
    int currentLineCount = m_document->lineCount();

    if (previousLineCount != currentLineCount) {
        previousLineCount = currentLineCount;
        emit lineCountChanged(currentLineCount);
    }
}

void TextDocument::markAsDirty(bool dirty)
{
    if (m_isDirty != dirty) {
        m_isDirty = dirty;
        emit dirtyChanged(m_isDirty);
    }
}

void TextDocument::resetUndoStack()
{
    m_undoStack->clear();
    m_undoStack->setClean();
}
