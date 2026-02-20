#ifndef MENUMANAGER_H
#define MENUMANAGER_H

#include <QObject>
#include <QMenuBar>
#include <QMenu>
#include <QAction>

class MenuManager : public QObject
{
    Q_OBJECT
public:
    explicit MenuManager(QMenuBar* menubar, QObject* parent = nullptr);
    void setupMenus();

signals:
    void newProjectTriggered();
    void openFileTriggered();
    void saveTriggered();
    void exitTriggered();

    void undoTriggered();
    void redoTriggered();
    void cutTriggered();
    void copyTriggered();
    void pasteTriggered();

    void fullscreenTriggered();
    void toggleToolbarTriggered();

    void preferencesTriggered();
    void changeThemeTriggered();

    void aboutAppTriggered();
    void aboutQtTriggered();

private:
    QMenuBar* m_menubar;

    void setupFileMenu();
    void setupEditMenu();
    void setupViewMenu();
    void setupSettingsMenu();
    void setupAboutMenu();

private slots:
    void onNewProject();
    void onOpenFile();
    void onSave();
    void onExit();

    void onUndo();
    void onRedo();
    void onCut();
    void onCopy();
    void onPaste();

    void onFullscreen();
    void onToggleToolbar();

    void onPreferences();
    void onChangeTheme();

    void onAboutApp();
    void onAboutQt();
};

#endif // MENUMANAGER_H