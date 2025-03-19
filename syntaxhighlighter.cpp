#include "syntaxhighlighter.h"

SyntaxHighlighter::SyntaxHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    // Initialize the highlighting formats with default colors

    // Keywords format (orange-brown)
    keywordFormat.setForeground(QColor("#CC7832"));
    keywordFormat.setFontWeight(QFont::Bold);

    // Class name format (light blue)
    classFormat.setForeground(QColor("#A9B7C6"));
    classFormat.setFontWeight(QFont::Bold);

    // Function format (light orange)
    functionFormat.setForeground(QColor("#FFC66D"));

    // Number format (blue)
    numberFormat.setForeground(QColor("#6897BB"));

    // String format (green)
    stringFormat.setForeground(QColor("#6A8759"));

    // Comment format (grey)
    commentFormat.setForeground(QColor("#808080"));
    commentFormat.setFontItalic(true);

    // Preprocessor format (yellow)
    preprocessorFormat.setForeground(QColor("#BBB529"));

    // Multi-line comment format (same as single-line comments)
    multiLineCommentFormat = commentFormat;

    // Set default language to generic
    setupGenericRules();
}

void SyntaxHighlighter::setLanguage(const QString &language)
{
    // Clear existing rules
    highlightingRules.clear();

    // Set new rules based on language
    if (language.toLower() == "cpp" || language.toLower() == "c" ||
        language.toLower() == "h" || language.toLower() == "hpp") {
        setupCppRules();
    }
    else if (language.toLower() == "js") {
        setupJavaScriptRules();
    }
    else if (language.toLower() == "py") {
        setupPythonRules();
    }
    else if (language.toLower() == "qml") {
        setupQmlRules();
    }
    else {
        setupGenericRules();
    }

    // Force rehighlighting of the entire document
    if (document()) {
        rehighlight();
    }
}

void SyntaxHighlighter::highlightBlock(const QString &text)
{
    // Apply normal highlighting rules
    for (const HighlightingRule &rule : qAsConst(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    // Handle multi-line comments
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1) {
        startIndex = text.indexOf(commentStartExpression);
    }

    while (startIndex >= 0) {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;

        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex + match.capturedLength();
        }

        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}

void SyntaxHighlighter::setupCppRules()
{
    HighlightingRule rule;

    // C++ keywords
    QStringList keywordPatterns = {
        "\\bauto\\b", "\\bbreak\\b", "\\bcase\\b", "\\bcatch\\b", "\\bclass\\b",
        "\\bconst\\b", "\\bconstexpr\\b", "\\bcontinue\\b", "\\bdefault\\b",
        "\\bdelete\\b", "\\bdo\\b", "\\bdouble\\b", "\\belse\\b", "\\benum\\b",
        "\\bexplicit\\b", "\\bexport\\b", "\\bextern\\b", "\\bfalse\\b", "\\bfloat\\b",
        "\\bfor\\b", "\\bfriend\\b", "\\bgoto\\b", "\\bif\\b", "\\binline\\b",
        "\\bint\\b", "\\blong\\b", "\\bmutable\\b", "\\bnamespace\\b", "\\bnew\\b",
        "\\bnoexcept\\b", "\\bnullptr\\b", "\\boperator\\b", "\\bprivate\\b",
        "\\bprotected\\b", "\\bpublic\\b", "\\bregister\\b", "\\breinterpret_cast\\b",
        "\\breturn\\b", "\\bshort\\b", "\\bsigned\\b", "\\bsizeof\\b", "\\bstatic\\b",
        "\\bstatic_assert\\b", "\\bstatic_cast\\b", "\\bstruct\\b", "\\bswitch\\b",
        "\\btemplate\\b", "\\bthis\\b", "\\bthrow\\b", "\\btrue\\b", "\\btry\\b",
        "\\btypedef\\b", "\\btypeid\\b", "\\btypename\\b", "\\bunion\\b",
        "\\bunsigned\\b", "\\busing\\b", "\\bvirtual\\b", "\\bvoid\\b",
        "\\bvolatile\\b", "\\bwhile\\b"
    };

    for (const QString &pattern : keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    // Class names (simplified - matches words after class, struct, or typename)
    rule.pattern = QRegularExpression("\\b(class|struct|typename)\\s+(\\w+)");
    rule.format = classFormat;
    highlightingRules.append(rule);

    // Function names
    rule.pattern = QRegularExpression("\\b(\\w+)\\s*\\(");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    // Preprocessor directives
    rule.pattern = QRegularExpression("^\\s*#\\s*\\w+");
    rule.format = preprocessorFormat;
    highlightingRules.append(rule);

    // Quotation
    rule.pattern = QRegularExpression("\".*\"");
    rule.format = stringFormat;
    highlightingRules.append(rule);

    // Single-line comments
    rule.pattern = QRegularExpression("//[^\n]*");
    rule.format = commentFormat;
    highlightingRules.append(rule);

    // Numbers
    rule.pattern = QRegularExpression("\\b\\d+\\b");
    rule.format = numberFormat;
    highlightingRules.append(rule);

    // Multi-line comments
    commentStartExpression = QRegularExpression("/\\*");
    commentEndExpression = QRegularExpression("\\*/");
}

void SyntaxHighlighter::setupJavaScriptRules()
{
    HighlightingRule rule;

    // JavaScript keywords
    QStringList keywordPatterns = {
        "\\bbreak\\b", "\\bcase\\b", "\\bcatch\\b", "\\bclass\\b", "\\bconst\\b",
        "\\bcontinue\\b", "\\bdebugger\\b", "\\bdefault\\b", "\\bdelete\\b",
        "\\bdo\\b", "\\belse\\b", "\\bexport\\b", "\\bextends\\b", "\\bfalse\\b",
        "\\bfinally\\b", "\\bfor\\b", "\\bfunction\\b", "\\bif\\b", "\\bimport\\b",
        "\\bin\\b", "\\binstanceof\\b", "\\bnew\\b", "\\bnull\\b", "\\breturn\\b",
        "\\bsuper\\b", "\\bswitch\\b", "\\bthis\\b", "\\bthrow\\b", "\\btrue\\b",
        "\\btry\\b", "\\btypeof\\b", "\\bvar\\b", "\\bvoid\\b", "\\bwhile\\b",
        "\\bwith\\b", "\\blet\\b", "\\bstatic\\b", "\\byield\\b", "\\basync\\b",
        "\\bawait\\b", "\\bconsole\\b"
    };

    for (const QString &pattern : keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    // Function names
    rule.pattern = QRegularExpression("\\b(\\w+)\\s*\\(");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    // Double-quoted strings
    rule.pattern = QRegularExpression("\".*?\"");
    rule.format = stringFormat;
    highlightingRules.append(rule);

    // Single-quoted strings
    rule.pattern = QRegularExpression("'.*?'");
    rule.format = stringFormat;
    highlightingRules.append(rule);

    // Single-line comments
    rule.pattern = QRegularExpression("//[^\n]*");
    rule.format = commentFormat;
    highlightingRules.append(rule);

    // Numbers
    rule.pattern = QRegularExpression("\\b\\d+\\b");
    rule.format = numberFormat;
    highlightingRules.append(rule);

    // Multi-line comments
    commentStartExpression = QRegularExpression("/\\*");
    commentEndExpression = QRegularExpression("\\*/");
}

void SyntaxHighlighter::setupPythonRules()
{
    HighlightingRule rule;

    // Python keywords
    QStringList keywordPatterns = {
        "\\band\\b", "\\bas\\b", "\\bassert\\b", "\\bbreak\\b", "\\bclass\\b",
        "\\bcontinue\\b", "\\bdef\\b", "\\bdel\\b", "\\belif\\b", "\\belse\\b",
        "\\bexcept\\b", "\\bFalse\\b", "\\bfinally\\b", "\\bfor\\b", "\\bfrom\\b",
        "\\bglobal\\b", "\\bif\\b", "\\bimport\\b", "\\bin\\b", "\\bis\\b",
        "\\blambda\\b", "\\bNone\\b", "\\bnonlocal\\b", "\\bnot\\b", "\\bor\\b",
        "\\bpass\\b", "\\braise\\b", "\\breturn\\b", "\\bTrue\\b", "\\btry\\b",
        "\\bwhile\\b", "\\bwith\\b", "\\byield\\b"
    };

    for (const QString &pattern : keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    // Function definitions
    rule.pattern = QRegularExpression("\\bdef\\s+(\\w+)\\s*\\(");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    // Class definitions
    rule.pattern = QRegularExpression("\\bclass\\s+(\\w+)\\s*[:\\(]");
    rule.format = classFormat;
    highlightingRules.append(rule);

    // Double-quoted strings
    rule.pattern = QRegularExpression("\".*?\"");
    rule.format = stringFormat;
    highlightingRules.append(rule);

    // Single-quoted strings
    rule.pattern = QRegularExpression("'.*?'");
    rule.format = stringFormat;
    highlightingRules.append(rule);

    // Single-line comments
    rule.pattern = QRegularExpression("#[^\n]*");
    rule.format = commentFormat;
    highlightingRules.append(rule);

    // Numbers
    rule.pattern = QRegularExpression("\\b\\d+\\b");
    rule.format = numberFormat;
    highlightingRules.append(rule);

    // Triple-quoted strings (simplified)
    commentStartExpression = QRegularExpression("\"\"\"");
    commentEndExpression = QRegularExpression("\"\"\"");
}

void SyntaxHighlighter::setupQmlRules()
{
    // QML includes JavaScript rules
    setupJavaScriptRules();

    HighlightingRule rule;

    // QML-specific keywords
    QStringList qmlKeywords = {
        "\\bproperty\\b", "\\bsignal\\b", "\\bItem\\b", "\\bRectangle\\b",
        "\\bText\\b", "\\bImage\\b", "\\bRow\\b", "\\bColumn\\b", "\\bGrid\\b",
        "\\bFlow\\b", "\\bMouseArea\\b", "\\bComponent\\b", "\\balias\\b",
        "\\banchors\\b", "\\bwidth\\b", "\\bheight\\b", "\\bcolor\\b",
        "\\bfont\\b", "\\bmargin\\b", "\\bpadding\\b", "\\bfocus\\b",
        "\\benabled\\b", "\\bvisible\\b", "\\bparent\\b", "\\bchildren\\b",
        "\\bmodel\\b", "\\bdelegate\\b", "\\brepeater\\b", "\\btransition\\b",
        "\\bstate\\b", "\\bstates\\b", "\\bscale\\b", "\\brotation\\b",
        "\\bopacity\\b", "\\bimplicitWidth\\b", "\\bimplicitHeight\\b"
    };

    for (const QString &keyword : qmlKeywords) {
        rule.pattern = QRegularExpression(keyword);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    // QML id property
    rule.pattern = QRegularExpression("\\bid\\s*:\\s*\\w+");
    rule.format = classFormat;
    highlightingRules.append(rule);
}

void SyntaxHighlighter::setupGenericRules()
{
    HighlightingRule rule;

    // Double-quoted strings
    rule.pattern = QRegularExpression("\".*?\"");
    rule.format = stringFormat;
    highlightingRules.append(rule);

    // Single-quoted strings
    rule.pattern = QRegularExpression("'.*?'");
    rule.format = stringFormat;
    highlightingRules.append(rule);

    // Numbers
    rule.pattern = QRegularExpression("\\b\\d+\\b");
    rule.format = numberFormat;
    highlightingRules.append(rule);

    // Multi-line comments for C-style languages
    commentStartExpression = QRegularExpression("/\\*");
    commentEndExpression = QRegularExpression("\\*/");
}
