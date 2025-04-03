#ifndef KNOWLEDGEBASE_H
#define KNOWLEDGEBASE_H

#include <QDialog>
#include <QString>
#include <QMap>

QT_BEGIN_NAMESPACE
namespace Ui {
class KnowledgeBase;
}
QT_END_NAMESPACE

class KnowledgeBase : public QDialog
{
    Q_OBJECT

public:
    explicit KnowledgeBase(QWidget *parent = nullptr);
    ~KnowledgeBase();

    void addDocument(const QString &id, const QString &content);
    QString getDocument(const QString &id) const;
    QMap<QString, QString> searchDocuments(const QString &query) const;
    void removeDocument(const QString &id);
    void AddFile(const QString &filePath);
    void selectDir();

private slots:
    void on_searchButton_clicked();
    void on_selectDir_clicked();
    void on_segmentPreView_clicked();
    void on_importFiles_clicked();
    void on_newKnowledgeBase_clicked();
    void showEvent(QShowEvent *event) override;

private:
    Ui::KnowledgeBase *ui;
    QMap<QString, QString> documents;
    QString knowledgebasename;

    void listAllKnowledgeBase();
};

#endif // KNOWLEDGEBASE_H