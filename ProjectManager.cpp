#include "ProjectManager.h"
#include <QFile>
#include <QJsonDocument>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>

ProjectManager::ProjectManager(QTreeWidget* tree, const QString& jsonPath, QObject* parent)
    : QObject(parent), treeWidget(tree), projectsJsonPath(jsonPath)
{
    loadProjects();
}

void ProjectManager::addProject(const QString &name, const QString &path)
{
    for (int i = 0; i < projectsArray.size(); ++i) {
        QJsonObject obj = projectsArray[i].toObject();
        if (obj["name"] == name) {
            projectsArray.removeAt(i);
            break;
        }
    }

    QJsonObject newProj;
    newProj["name"] = name;
    newProj["path"] = path;
    projectsArray.append(newProj);

    saveProjects();
    updateTree();
}

void ProjectManager::saveProjects()
{
    QFile file(projectsJsonPath);  
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Cannot write projects.json!";
        return;
    }

    QJsonDocument doc(projectsArray);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
}


void ProjectManager::cleanMissingProjects()
{
    QJsonArray cleanedArray;
    for (const QJsonValue& val : projectsArray) {
        QJsonObject obj = val.toObject();
        QString path = obj["path"].toString();
        if (QDir(path).exists()) {
            cleanedArray.append(obj);
        }
    }
    projectsArray = cleanedArray;
    saveProjects();
}

void ProjectManager::loadProjects()
{
    QFile file(projectsJsonPath);
    if (!file.exists()) return;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray()) return;

    projectsArray = doc.array();

    for (int i = projectsArray.size() - 1; i >= 0; --i) {
        QJsonObject obj = projectsArray[i].toObject();
        QString path = obj["path"].toString();
        if (!QDir(path).exists()) {
            projectsArray.removeAt(i);
        }
    }

    updateTree(); 
}

void ProjectManager::updateTree()
{
    if (!treeWidget) return;

    treeWidget->clear();

    for (const QJsonValue &val : projectsArray)
    {
        QJsonObject obj = val.toObject();
        QString name = obj["name"].toString();
        QString path = obj["path"].toString();

        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, name);
        item->setData(0, Qt::UserRole, path); 
        treeWidget->addTopLevelItem(item);
    }
}
