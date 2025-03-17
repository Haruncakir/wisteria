#ifndef THEME_H
#define THEME_H

#include <QObject>
#include <QColor>
#include <QMap>
#include <QSettings>

class Theme : public QObject
{
    Q_OBJECT

    // Core UI colors
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
    Q_PROPERTY(QColor sidebarColor READ sidebarColor WRITE setSidebarColor NOTIFY sidebarColorChanged)
    Q_PROPERTY(QColor explorerColor READ explorerColor WRITE setExplorerColor NOTIFY explorerColorChanged)
    Q_PROPERTY(QColor tabBarColor READ tabBarColor WRITE setTabBarColor NOTIFY tabBarColorChanged)
    Q_PROPERTY(QColor menuBarColor READ menuBarColor WRITE setMenuBarColor NOTIFY menuBarColorChanged)
    Q_PROPERTY(QColor activeTabColor READ activeTabColor WRITE setActiveTabColor NOTIFY activeTabColorChanged)

    // Text colors
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor NOTIFY textColorChanged)
    Q_PROPERTY(QColor menuTextColor READ menuTextColor WRITE setMenuTextColor NOTIFY menuTextColorChanged)
    Q_PROPERTY(QColor lineNumberColor READ lineNumberColor WRITE setLineNumberColor NOTIFY lineNumberColorChanged)

    // Current theme name
    Q_PROPERTY(QString themeName READ themeName WRITE setThemeName NOTIFY themeNameChanged)

    // Available themes
    Q_PROPERTY(QStringList availableThemes READ availableThemes NOTIFY availableThemesChanged)

public:
    explicit Theme(QObject *parent = nullptr);
    ~Theme();

    // Core UI colors getters/setters
    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &color);

    QColor sidebarColor() const;
    void setSidebarColor(const QColor &color);

    QColor explorerColor() const;
    void setExplorerColor(const QColor &color);

    QColor tabBarColor() const;
    void setTabBarColor(const QColor &color);

    QColor menuBarColor() const;
    void setMenuBarColor(const QColor &color);

    QColor activeTabColor() const;
    void setActiveTabColor(const QColor &color);

    // Text colors getters/setters
    QColor textColor() const;
    void setTextColor(const QColor &color);

    QColor menuTextColor() const;
    void setMenuTextColor(const QColor &color);

    QColor lineNumberColor() const;
    void setLineNumberColor(const QColor &color);

    // Theme name getter/setter
    QString themeName() const;
    void setThemeName(const QString &name);

    // Available themes getter
    QStringList availableThemes() const;

    // Methods for theme management
    Q_INVOKABLE bool saveCurrentTheme(const QString &name);
    Q_INVOKABLE bool loadTheme(const QString &name);
    Q_INVOKABLE bool deleteTheme(const QString &name);
    Q_INVOKABLE bool resetToDefault();
    Q_INVOKABLE bool exportTheme(const QString &filePath);
    Q_INVOKABLE bool importTheme(const QString &filePath);

signals:
    // Signals for property changes
    void backgroundColorChanged(const QColor &color);
    void sidebarColorChanged(const QColor &color);
    void explorerColorChanged(const QColor &color);
    void tabBarColorChanged(const QColor &color);
    void menuBarColorChanged(const QColor &color);
    void activeTabColorChanged(const QColor &color);
    void textColorChanged(const QColor &color);
    void menuTextColorChanged(const QColor &color);
    void lineNumberColorChanged(const QColor &color);
    void themeNameChanged(const QString &name);
    void availableThemesChanged();
    void errorOccurred(const QString &error);

private:
    // Private helper methods
    void loadBuiltinThemes();
    void setupDefaultTheme();
    void loadSavedThemes();
    void saveThemes();

    // Theme data
    QString m_themeName;
    QColor m_backgroundColor;
    QColor m_sidebarColor;
    QColor m_explorerColor;
    QColor m_tabBarColor;
    QColor m_menuBarColor;
    QColor m_activeTabColor;
    QColor m_textColor;
    QColor m_menuTextColor;
    QColor m_lineNumberColor;

    // Collection of themes
    QMap<QString, QMap<QString, QVariant>> m_themes;
    QSettings m_settings;
};

#endif // THEME_H
