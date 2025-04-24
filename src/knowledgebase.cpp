#include "KnowledgeBase.h"
#include "ui_KnowledgeBase.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QListWidget>
#include <QLineEdit>
#include <QTextBrowser>
#include <QMessageBox>

KnowledgeBase::KnowledgeBase(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KnowledgeBase)
{
    ui->setupUi(this);
}
void KnowledgeBase::listAllKnowledgeBase()
{
    QDir dir("KnowledgeBase");
    QStringList dirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    ui->comboBoxKB->clear();
    ui->comboBoxKB->addItems(dirs);

}
void KnowledgeBase::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    listAllKnowledgeBase();
}

KnowledgeBase::~KnowledgeBase()
{
    delete ui;
}


void KnowledgeBase::addDocument(const QString &id, const QString &content)
{
    documents[id] = content;
}

QString KnowledgeBase::getDocument(const QString &id) const
{
    return documents.value(id, QString());
}

QMap<QString, QString> KnowledgeBase::searchDocuments(const QString &query) const
{
    QMap<QString, QString> results;
    for (auto it = documents.constBegin(); it != documents.constEnd(); ++it) {
        if (it.value().contains(query)) {
            results[it.key()] = it.value();
        }
    }
    return results;
}

void KnowledgeBase::removeDocument(const QString &id)
{
    documents.remove(id);
}

void KnowledgeBase::on_selectDir_clicked()
{
    QString dirPath = QFileDialog::getExistingDirectory(this, tr("选择目录"), QDir::homePath());
    if (!dirPath.isEmpty()) {
        // 这里可以添加处理目录路径的逻辑
        ui->searchInput->setText(dirPath);
    }
}

void KnowledgeBase::on_searchButton_clicked()
{
    QString searchDir = ui->searchInput->text();

    if (searchDir.isEmpty()) {
        return;
    }

    QDir directory(searchDir);
    QStringList files = directory.entryList(QStringList() << "*.txt" << "*.md" << "*.log" << "*.py", QDir::Files);
    ui->documentList->clear();

    foreach(QString file, files) {
        QString filePath = directory.absoluteFilePath(file);
        QListWidgetItem* item = new QListWidgetItem(file);
                item->setData(Qt::UserRole, filePath);
                ui->documentList->addItem(item);
    }

    if (ui->documentList->count() == 0) {
        QMessageBox::information(this, tr("Search Result"), tr("No matching files found."));
    }
}
void KnowledgeBase::on_segmentPreView_clicked() {
    QListWidgetItem* selectedItem = ui->documentList->currentItem();
    if (selectedItem) {
        QString filePath = selectedItem->data(Qt::UserRole).toString();
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QString content = file.readAll();
            QStringList segments = content.split("\n\n");
            for (int i = 0; i < segments.size(); ++i) {
                segments[i] = QString("## Segment %1\n\n%2").arg(i+1).arg(segments[i]);
            }
            ui->contentView->setPlainText(segments.join("\n---\n"));
            file.close();
        } 
    } 
}
void KnowledgeBase::on_importFiles_clicked() {
    if(knowledgebasename.isEmpty()){
        QMessageBox::information(this, tr("Error"), tr("Please input a name for the knowledge base."));
        return; 
    }
    // 从 documentList 中获取所有文件路径列表
    QList<QListWidgetItem*> items = ui->documentList->findItems("*", Qt::MatchWildcard);
    QStringList filePaths      = QStringList();
    foreach(QListWidgetItem* item, items) {
        filePaths.append(item->data(Qt::UserRole).toString());        
    }
    //全部导入
    foreach(QString filePath, filePaths) {
        AddFile(filePath);  
    }

}
void KnowledgeBase::on_newKnowledgeBase_clicked()
{
    knowledgebasename = ui->comboBoxKB->currentText();
    if(knowledgebasename.isEmpty()){
        QMessageBox::information(this, tr("Error"), tr("Please input a name for the knowledge base."));
        return; 
    }
    QString destination = QCoreApplication::applicationDirPath() + "/KnowledgeBase/" +knowledgebasename+"/";
    //确保目标目录存在
    QDir dir = QFileInfo(destination).absoluteDir();
    if (!dir.exists()) {
        dir.mkpath(dir.absolutePath());
    }

    listAllKnowledgeBase();
}
void KnowledgeBase::AddFile(const QString &filePath) {
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString content = file.readAll();
        file.close();

        QString id = QFileInfo(filePath).fileName();
        addDocument(id, content);

        QString destination = QCoreApplication::applicationDirPath() + "/KnowledgeBase/" +knowledgebasename+"/"+ id;
        //确保目标目录存在
        QDir dir = QFileInfo(destination).absoluteDir();
        if (!dir.exists()) {
            dir.mkpath(dir.absolutePath());
        }
        QFile::copy(filePath, destination);
    }
}