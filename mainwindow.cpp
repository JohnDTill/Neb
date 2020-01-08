#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "parser.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);

    //ui->code_edit->setHo
    ui->code_edit->setPlainText("⁜^⏴x⏵⏴*⏵ ∈ ℝ\ny = ⁜^⏴x⏵⏴2⏵");
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_exec_button_clicked(){
    QString code = ui->code_edit->toPlainText();

    std::vector<Parser::Node*> statements = Parser::parse(code);
    QString DOT = Parser::toDOT(statements);

    ui->dot_view->setPlainText(DOT);
}
