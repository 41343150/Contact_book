#include "mywidget.h"
#include "ui_mywidget.h"

#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QTableWidgetItem>
#include <QDebug>
#include <QHeaderView>

QString defaultFilename = "C:/Users/user/Desktop/1117/file.txt";

void WriteToFile(const QString &filename, const QString &content) {
    QFile file(filename);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        qDebug() << "could not open file for write";
        return;
    }
    QTextStream out(&file);
    out << content;
    file.close();
}

QString ReadFromFile(const QString &filename) {
    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "could not open file for read";
        return "";
    }
    QTextStream in(&file);
    QString text = in.readAll();
    file.close();
    return text;
}

myWidget::myWidget(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::myWidget),
    currentFilename("")
{
    ui->setupUi(this);

    QStringList headers;
    headers << "學號" << "班級" << "姓名" << "電話";

    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    
    // 設置表格外觀
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    // 設置最小列高
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(35);
}

myWidget::~myWidget() {
    delete ui;
}

void myWidget::on_pushButton_2_clicked() {
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(ui->lineEdit->text()));
    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(ui->lineEdit_2->text()));
    ui->tableWidget->setItem(row, 2, new QTableWidgetItem(ui->lineEdit_3->text()));
    ui->tableWidget->setItem(row, 3, new QTableWidgetItem(ui->lineEdit_4->text()));

    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
}

void myWidget::on_pushButton_clicked() {
    QString filename;

    if (!currentFilename.isEmpty()) {
        filename = currentFilename;
    } else {
        filename = QFileDialog::getSaveFileName(
            this,
            "另存新檔",
            defaultFilename,
            "Text Files (*.txt);;All Files (*)"
            );

        if (filename.isEmpty())
            return;

        currentFilename = filename;
    }

    QString content;
    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        for (int j = 0; j < ui->tableWidget->columnCount(); j++) {
            content += (j == 0) ? "" : ",";
            content += ui->tableWidget->item(i, j)->text();
        }
        content += "\n";
    }

    WriteToFile(filename, content);
}

void myWidget::on_pushButton_4_clicked() {
    QString filename;

    if (!currentFilename.isEmpty()) {
        filename = currentFilename;
    } else {
        filename = QFileDialog::getSaveFileName(
            this,
            "另存新檔",
            defaultFilename,
            "Text Files (*.txt);;All Files (*)"
            );

        if (filename.isEmpty())
            return;

        currentFilename = filename;
    }

    QString content;
    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        for (int j = 0; j < ui->tableWidget->columnCount(); j++) {
            content += (j == 0) ? "" : ",";
            content += ui->tableWidget->item(i, j)->text();
        }
        content += "\n";
    }

    WriteToFile(filename, content);
    close();
}

void myWidget::on_pushButton_3_clicked() {
    QString filename = QFileDialog::getOpenFileName(
        this,
        "選擇匯入的檔案",
        defaultFilename,
        "Text Files (*.txt);;All Files (*)"
        );

    if (filename.isEmpty())
        return;

    QString text = ReadFromFile(filename);
    if (text.isEmpty())
        return;

    currentFilename = filename;
    ui->tableWidget->setRowCount(0);

    QStringList lines = text.split("\n", Qt::SkipEmptyParts);

    for (const QString &line : lines) {
        QStringList fields = line.split(",");
        if (fields.size() == 4) {
            int row = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(row);
            for (int i = 0; i < 4; i++)
                ui->tableWidget->setItem(row, i, new QTableWidgetItem(fields[i]));
        }
    }
}
