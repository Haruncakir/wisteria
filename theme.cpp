#include "theme.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>

Theme::Theme(QObject *parent)
    : QObject(parent)
    , m_themeName("Default")
    , m_settings("Wisteria", "WisteriaEditor")
{
    // Load built-in and saved themes
    loadBuiltinThemes();
    loadSavedThemes();

    // Initialize with saved or default theme
    QString savedTheme = m_settings.value("currentTheme", "Default").toString();
    if (!loadTheme(savedTheme)) {
        setupDefaultTheme();
    }
}

Theme::~Theme()
{
    // Save current settings
    m_settings.setValue("currentTheme", m_themeName);
    saveThemes();
}

QColor Theme::backgroundColor() const
{
    return m_backgroundColor;
}

void Theme::setBackgroundColor(const QColor &color)
{
    if (m_backgroundColor != color) {
        m_backgroundColor = color;
        emit backgroundColorChanged(color);
    }
}

QColor Theme::sidebarColor() const
{
    return m_sidebarColor;
}

void Theme::setSidebarColor(const QColor &color)
{
    if (m_sidebarColor != color) {
        m_sidebarColor = color;
        emit sidebarColorChanged(color);
    }
}

QColor Theme::explorerColor() const
{
    return m_explorerColor;
}

void Theme::setExplorerColor(const QColor &color)
{
    if (m_explorerColor != color) {
        m_explorerColor = color;
        emit explorerColorChanged(color);
    }
}

QColor Theme::tabBarColor() const
{
    return m_tabBarColor;
}

void Theme::setTabBarColor(const QColor &color)
{
    if (m_tabBarColor != color) {
        m_tabBarColor = color;
        emit tabBarColorChanged(color);
    }
}

QColor Theme::menuBarColor() const
{
    return m_menuBarColor;
}

void Theme::setMenuBarColor(const QColor &color)
{
    if (m_menuBarColor != color) {
        m_menuBarColor = color;
        emit menuBarColorChanged(color);
    }
}

QColor Theme::activeTabColor() const
{
    return m_activeTabColor;
}

void Theme::setActiveTabColor(const QColor &color)
{
    if (m_activeTabColor != color) {
        m_activeTabColor = color;
        emit activeTabColorChanged(color);
    }
}

QColor Theme::textColor() const
{
    return m_textColor;
}

void Theme::setTextColor(const QColor &color)
{
    if (m_textColor != color) {
        m_textColor = color;
        emit textColorChanged(color);
    }
}

QColor Theme::menuTextColor() const
{
    return m_menuTextColor;
}

void Theme::setMenuTextColor(const QColor &color)
{
    if (m_menuTextColor != color) {
        m_menuTextColor = color;
        emit menuTextColorChanged(color);
    }
}

QColor Theme::lineNumberColor() const
{
    return m_lineNumberColor;
}

void Theme::setLineNumberColor(const QColor &color)
{
    if (m_lineNumberColor != color) {
        m_lineNumberColor = color;
        emit lineNumberColorChanged(color);
    }
}

QString Theme::themeName() const
{
    return m_themeName;
}

void Theme::setThemeName(const QString &name)
{
    if (m_themeName != name) {
        m_themeName = name;
        emit themeNameChanged(name);
    }
}

QStringList Theme::availableThemes() const
{
    return m_themes.keys();
}

bool Theme::saveCurrentTheme(const QString &name)
{
    // Don't allow overwriting built-in themes
    if (name == "Default" || name == "Light" || name == "Dracula") {
        emit errorOccurred(tr("Cannot overwrite built-in theme: %1").arg(name));
        return false;
    }

    // Save current properties to theme map
    QMap<QString, QVariant> themeData;
    themeData["backgroundColor"] = m_backgroundColor;
    themeData["sidebarColor"] = m_sidebarColor;
    themeData["explorerColor"] = m_explorerColor;
    themeData["tabBarColor"] = m_tabBarColor;
    themeData["menuBarColor"] = m_menuBarColor;
    themeData["activeTabColor"] = m_activeTabColor;
    themeData["textColor"] = m_textColor;
    themeData["menuTextColor"] = m_menuTextColor;
    themeData["lineNumberColor"] = m_lineNumberColor;

    // Add or update the theme
    m_themes[name] = themeData;

    // Update current theme name
    setThemeName(name);

    // Save themes to settings
    saveThemes();

    emit availableThemesChanged();
    return true;
}

bool Theme::loadTheme(const QString &name)
{
    if (!m_themes.contains(name)) {
        emit errorOccurred(tr("Theme not found: %1").arg(name));
        return false;
    }

    QMap<QString, QVariant> themeData = m_themes[name];

    setBackgroundColor(themeData["backgroundColor"].value<QColor>());
    setSidebarColor(themeData["sidebarColor"].value<QColor>());
    setExplorerColor(themeData["explorerColor"].value<QColor>());
    setTabBarColor(themeData["tabBarColor"].value<QColor>());
    setMenuBarColor(themeData["menuBarColor"].value<QColor>());
    setActiveTabColor(themeData["activeTabColor"].value<QColor>());
    setTextColor(themeData["textColor"].value<QColor>());
    setMenuTextColor(themeData["menuTextColor"].value<QColor>());
    setLineNumberColor(themeData["lineNumberColor"].value<QColor>());

    setThemeName(name);
    m_settings.setValue("currentTheme", name);

    return true;
}

bool Theme::deleteTheme(const QString &name)
{
    // Don't allow deleting built-in themes
    if (name == "Default" || name == "Light" || name == "Dracula") {
        emit errorOccurred(tr("Cannot delete built-in theme: %1").arg(name));
        return false;
    }

    if (!m_themes.contains(name)) {
        emit errorOccurred(tr("Theme not found: %1").arg(name));
        return false;
    }

    m_themes.remove(name);

    // If we deleted the current theme, switch to Default
    if (m_themeName == name) {
        loadTheme("Default");
    }

    saveThemes();
    emit availableThemesChanged();
    return true;
}

bool Theme::resetToDefault()
{
    return loadTheme("Default");
}

bool Theme::exportTheme(const QString &filePath)
{
    QMap<QString, QVariant> themeData = m_themes[m_themeName];

    QJsonObject jsonTheme;
    jsonTheme["name"] = m_themeName;
    jsonTheme["backgroundColor"] = m_backgroundColor.name();
    jsonTheme["sidebarColor"] = m_sidebarColor.name();
    jsonTheme["explorerColor"] = m_explorerColor.name();
    jsonTheme["tabBarColor"] = m_tabBarColor.name();
    jsonTheme["menuBarColor"] = m_menuBarColor.name();
    jsonTheme["activeTabColor"] = m_activeTabColor.name();
    jsonTheme["textColor"] = m_textColor.name();
    jsonTheme["menuTextColor"] = m_menuTextColor.name();
    jsonTheme["lineNumberColor"] = m_lineNumberColor.name();

    QJsonDocument doc(jsonTheme);
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly)) {
        emit errorOccurred(tr("Could not write to file: %1").arg(filePath));
        return false;
    }

    file.write(doc.toJson());
    file.close();

    return true;
}

bool Theme::importTheme(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        emit errorOccurred(tr("Could not read file: %1").arg(filePath));
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        emit errorOccurred(tr("Invalid theme file format"));
        return false;
    }

    QJsonObject jsonTheme = doc.object();

    QString name = jsonTheme["name"].toString();
    if (name.isEmpty()) {
        name = QFileInfo(filePath).baseName();
    }

    // Create theme data
    QMap<QString, QVariant> themeData;
    themeData["backgroundColor"] = QColor(jsonTheme["backgroundColor"].toString());
    themeData["sidebarColor"] = QColor(jsonTheme["sidebarColor"].toString());
    themeData["explorerColor"] = QColor(jsonTheme["explorerColor"].toString());
    themeData["tabBarColor"] = QColor(jsonTheme["tabBarColor"].toString());
    themeData["menuBarColor"] = QColor(jsonTheme["menuBarColor"].toString());
    themeData["activeTabColor"] = QColor(jsonTheme["activeTabColor"].toString());
    themeData["textColor"] = QColor(jsonTheme["textColor"].toString());
    themeData["menuTextColor"] = QColor(jsonTheme["menuTextColor"].toString());
    themeData["lineNumberColor"] = QColor(jsonTheme["lineNumberColor"].toString());

    // Add to themes
    m_themes[name] = themeData;

    // Apply the theme
    loadTheme(name);

    saveThemes();
    emit availableThemesChanged();

    return true;
}

void Theme::loadBuiltinThemes()
{
    // Default theme (Dark)
    QMap<QString, QVariant> defaultTheme;
    defaultTheme["backgroundColor"] = QColor("#1e1e2e");
    defaultTheme["sidebarColor"] = QColor("#2f2f3f");
    defaultTheme["explorerColor"] = QColor("#252535");
    defaultTheme["tabBarColor"] = QColor("#252535");
    defaultTheme["menuBarColor"] = QColor("#2d2d3a");
    defaultTheme["activeTabColor"] = QColor("#1e1e2e");
    defaultTheme["textColor"] = QColor("#ffffff");
    defaultTheme["menuTextColor"] = QColor("#d9d9d9");
    defaultTheme["lineNumberColor"] = QColor("#737373");
    m_themes["Default"] = defaultTheme;

    // Light theme
    QMap<QString, QVariant> lightTheme;
    lightTheme["backgroundColor"] = QColor("#ffffff");
    lightTheme["sidebarColor"] = QColor("#f0f0f0");
    lightTheme["explorerColor"] = QColor("#f5f5f5");
    lightTheme["tabBarColor"] = QColor("#e5e5e5");
    lightTheme["menuBarColor"] = QColor("#e0e0e0");
    lightTheme["activeTabColor"] = QColor("#ffffff");
    lightTheme["textColor"] = QColor("#000000");
    lightTheme["menuTextColor"] = QColor("#333333");
    lightTheme["lineNumberColor"] = QColor("#999999");
    m_themes["Light"] = lightTheme;

    // Dracula theme
    QMap<QString, QVariant> draculaTheme;
    draculaTheme["backgroundColor"] = QColor("#282a36");
    draculaTheme["sidebarColor"] = QColor("#343746");
    draculaTheme["explorerColor"] = QColor("#303240");
    draculaTheme["tabBarColor"] = QColor("#343746");
    draculaTheme["menuBarColor"] = QColor("#343746");
    draculaTheme["activeTabColor"] = QColor("#282a36");
    draculaTheme["textColor"] = QColor("#f8f8f2");
    draculaTheme["menuTextColor"] = QColor("#f8f8f2");
    draculaTheme["lineNumberColor"] = QColor("#6272a4");
    m_themes["Dracula"] = draculaTheme;
}

void Theme::setupDefaultTheme()
{
    loadTheme("Default");
}

void Theme::loadSavedThemes()
{
    int count = m_settings.beginReadArray("themes");

    for (int i = 0; i < count; i++) {
        m_settings.setArrayIndex(i);

        QString name = m_settings.value("name").toString();

        // Skip built-in themes (they're already loaded)
        if (name == "Default" || name == "Light" || name == "Dracula") {
            continue;
        }

        QMap<QString, QVariant> themeData;
        themeData["backgroundColor"] = m_settings.value("backgroundColor");
        themeData["sidebarColor"] = m_settings.value("sidebarColor");
        themeData["explorerColor"] = m_settings.value("explorerColor");
        themeData["tabBarColor"] = m_settings.value("tabBarColor");
        themeData["menuBarColor"] = m_settings.value("menuBarColor");
        themeData["activeTabColor"] = m_settings.value("activeTabColor");
        themeData["textColor"] = m_settings.value("textColor");
        themeData["menuTextColor"] = m_settings.value("menuTextColor");
        themeData["lineNumberColor"] = m_settings.value("lineNumberColor");

        m_themes[name] = themeData;
    }

    m_settings.endArray();
}

void Theme::saveThemes()
{
    m_settings.beginWriteArray("themes");

    int index = 0;
    QMapIterator<QString, QMap<QString, QVariant>> i(m_themes);
    while (i.hasNext()) {
        i.next();

        // Skip built-in themes
        if (i.key() == "Default" || i.key() == "Light" || i.key() == "Dracula") {
            continue;
        }

        m_settings.setArrayIndex(index++);

        QMap<QString, QVariant> themeData = i.value();
        m_settings.setValue("name", i.key());
        m_settings.setValue("backgroundColor", themeData["backgroundColor"]);
        m_settings.setValue("sidebarColor", themeData["sidebarColor"]);
        m_settings.setValue("explorerColor", themeData["explorerColor"]);
        m_settings.setValue("tabBarColor", themeData["tabBarColor"]);
        m_settings.setValue("menuBarColor", themeData["menuBarColor"]);
        m_settings.setValue("activeTabColor", themeData["activeTabColor"]);
        m_settings.setValue("textColor", themeData["textColor"]);
        m_settings.setValue("menuTextColor", themeData["menuTextColor"]);
        m_settings.setValue("lineNumberColor", themeData["lineNumberColor"]);
    }

    m_settings.endArray();
}
