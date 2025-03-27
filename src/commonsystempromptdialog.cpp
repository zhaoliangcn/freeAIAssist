#include "commonsystempromptdialog.h"
#include "ui_commonsystempromptdialog.h"
#include <QClipboard>
#include <QMenu>


CommonSystemPromptDialog::CommonSystemPromptDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CommonSystemPromptDialog)
{
    ui->setupUi(this);
    setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "SystemPrompt" );
    ui->tableWidget->setRowCount(5);
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem(u8"你是一个数据安全专家，在密码学方面有很深的造诣，你会根据用户的需求，为用户提供安全的密码建议。"));
    ui->tableWidget->setItem(1, 0, new QTableWidgetItem(u8"你是一个网络安全专家，你会根据用户的需求，为用户提供安全的网络建议。"));   
    ui->tableWidget->setItem(2, 0, new QTableWidgetItem(u8"你是一个操作系统安全专家，你会根据用户的需求，为用户提供安全的操作系统建议。"));   
    ui->tableWidget->setItem(3, 0, new QTableWidgetItem(u8"你是一个系统架构师，你会根据用户的需求，为用户提供系统架构建议。"));
    ui->tableWidget->setItem(4, 0, new QTableWidgetItem(u8"你是一个高级开发工程师，擅长使用C++、Python、Java等编程语言，你能根据代码情况，为用户提供优化建议。"));

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setVisible(false);
    connect(ui->tableWidget, &CommonSystemPromptDialog::customContextMenuRequested, this, [this](const QPoint &pos) {
        QContextMenuEvent event(QContextMenuEvent::Mouse, pos);
        contextMenuEvent(&event);
        this->close();
    });
    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint);
}

CommonSystemPromptDialog::~CommonSystemPromptDialog()
{
    delete ui;
}

void CommonSystemPromptDialog::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu menu(this);
    QAction* copyAction = menu.addAction(u8"复制到剪切板");
    QAction* selectedAction = menu.exec(event->globalPos());

    if (selectedAction == copyAction) {
        QTableWidget* tableWidget = findChild<QTableWidget*>("tableWidget");
        if (tableWidget) {
            QItemSelectionModel* selectionModel = tableWidget->selectionModel();
            if (selectionModel->hasSelection()) {
                QModelIndexList selectedIndexes = selectionModel->selectedIndexes();
                QString text;
                for (const QModelIndex& index : selectedIndexes) {
                    text += tableWidget->item(index.row(), index.column())->text() + "\t";
                }
                QClipboard* clipboard = QGuiApplication::clipboard();
                clipboard->setText(text);
            }
        }
    }
}
