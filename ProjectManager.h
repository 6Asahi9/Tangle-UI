#pragma once
#include <QTreeWidget>
#include <QString>
#include <QJsonObject>
#include <QJsonArray>

class ProjectManager : public QObject
{
    Q_OBJECT
public:
    ProjectManager(QTreeWidget* tree, const QString& jsonPath, QObject* parent = nullptr);

    void addProject(const QString& name, const QString& path);
    void loadProjects();
    void saveProjects();
    void cleanMissingProjects();

private:
    QTreeWidget* treeWidget;
    QString projectsJsonPath;
    QJsonArray projectsArray;
    void updateTree();
};
