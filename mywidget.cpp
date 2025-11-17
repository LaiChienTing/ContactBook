#include "mywidget.h"
#include "ui_mywidget.h"

#include <QFile>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>

QString mFilename = "C:/Users/user/Desktop/EX/ContactBook/myfile.txt";

void Write(QString Filename,QString str)
{
    QFile mFile(Filename);
    if(!mFile.open(QFile::WriteOnly | QFile::Text)){
        qDebug() << "could not open file for write";
        return;
    }

    QTextStream out(&mFile);
    out << str;
    mFile.flush();
    mFile.close();
}

MyWidget::MyWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MyWidget)
{
    ui->setupUi(this);

    QStringList ColTitle;

    ui->tableWidget->setColumnCount(4);
    ColTitle<<QStringLiteral("學號")<<QStringLiteral("班級")<<QStringLiteral("姓名")<<QStringLiteral("電話");
    ui->tableWidget->setHorizontalHeaderLabels(ColTitle);
}

MyWidget::~MyWidget()
{
    delete ui;
}

void MyWidget::on_pushButton_clicked()
{
    QTableWidgetItem *inputRow1,*inputRow2,*inputRow3,*inputRow4;

    inputRow1 = new QTableWidgetItem(QString(ui->lineEdit->text()));
    inputRow2 = new QTableWidgetItem(QString(ui->lineEdit_2->text()));
    inputRow3 = new QTableWidgetItem(QString(ui->lineEdit_3->text()));
    inputRow4 = new QTableWidgetItem(QString(ui->lineEdit_4->text()));

    ui->tableWidget->insertRow(ui->tableWidget->rowCount());

    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 0, inputRow1);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 1, inputRow2);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 2, inputRow3);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 3, inputRow4);
}


void MyWidget::on_pushButton_2_clicked()
{
    QString saveFile = "";   // 存檔內容

    for(int i = 0; i < ui->tableWidget->rowCount(); i++){
        for(int j = 0; j < ui->tableWidget->columnCount(); j++){
            saveFile += ui->tableWidget->item(i, j)->text() + ",";
        }
        saveFile += "\n";
    }

    Write(mFilename, saveFile);  // 呼叫Write
}

// 匯入功能
void MyWidget::on_pushButton_3_clicked()
{
    // 使用 QFileDialog::getOpenFileName 開啟檔案選擇對話框
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    QStringLiteral("選擇要匯入的檔案"),
                                                    "",
                                                    "Text Files (*.txt);;All Files (*)");
    
    if (fileName.isEmpty()) {
        return;  // 使用者取消選擇
    }
    
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, QStringLiteral("錯誤"), QStringLiteral("無法開啟檔案！"));
        return;
    }
    
    // 清空現有資料
    ui->tableWidget->setRowCount(0);
    
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(",");
        
        if (fields.size() >= 4) {  // 確保有足夠的欄位
            int row = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(row);
            
            for (int i = 0; i < 4; i++) {
                QTableWidgetItem *item = new QTableWidgetItem(fields[i]);
                ui->tableWidget->setItem(row, i, item);
            }
        }
    }
    
    file.close();
    QMessageBox::information(this, QStringLiteral("成功"), QStringLiteral("資料匯入成功！"));
}

// 結束功能
void MyWidget::on_pushButton_4_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, QStringLiteral("確認"), 
                                  QStringLiteral("確定要結束程式嗎？"),
                                  QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        QApplication::quit();
    }
}

