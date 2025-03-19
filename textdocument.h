#ifndef TEXTDOCUMENT_H
#define TEXTDOCUMENT_H
#include <QObject>
#include <QString>
#include <QTextDocument>
#include <QUndoStack>

// Forward declaration of the syntax highlighter
class SyntaxHighlighter;

class TextDocument : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString content READ content WRITE setContent NOTIFY contentChanged)
    Q_PROPERTY(bool isDirty READ isDirty NOTIFY dirtyChanged)
    Q_PROPERTY(int lineCount READ lineCount NOTIFY lineCountChanged)
public:
    explicit TextDocument(QObject *parent = nullptr);
    ~TextDocument();
    // File operations
    bool loadFile(const QString &filePath);
    bool saveFile(const QString &filePath);
    // Content accessors
    QString content() const;
    void setContent(const QString &content);
    // State accessors
    bool isDirty() const;
    int lineCount() const;
    // Editing operations
    Q_INVOKABLE void undo();
    Q_INVOKABLE void redo();
    Q_INVOKABLE void insertText(int position, const QString &text);
    Q_INVOKABLE void removeText(int position, int length);
    Q_INVOKABLE void replaceText(int position, int length, const QString &text);
    // Line operations
    Q_INVOKABLE QString getLine(int lineNumber) const;
    Q_INVOKABLE int getLineStart(int lineNumber) const;
    Q_INVOKABLE int getLineLength(int lineNumber) const;

    // Syntax highlighting
    QTextDocument* document() const { return m_document; }
    void applySyntaxHighlighting(const QString &fileExtension);

signals:
    void contentChanged();
    void dirtyChanged(bool isDirty);
    void lineCountChanged(int lineCount);
private:
    // Internal document representation
    QTextDocument *m_document;
    QUndoStack *m_undoStack;
    // State tracking
    bool m_isDirty;
    QString m_originalContent;
    QString m_filePath;
    // Syntax highlighter
    SyntaxHighlighter *m_highlighter;
    // Helpers
    void updateLineCount();
    void markAsDirty(bool dirty = true);
    void resetUndoStack();
    // Commands for undo/redo
    class TextEditCommand;
};
#endif // TEXTDOCUMENT_H
