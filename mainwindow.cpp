#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "parser.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);

    ui->code_edit->setPlainText("⁜^⏴x⏵⏴*⏵ ∈ ℝ\ny = ⁜^⏴x⏵⏴2⏵");
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_exec_button_clicked(){
    QString code = ui->code_edit->toPlainText();
    Neb::Parser parser(code);

    try{
        std::vector<Neb::Node*> statements = parser.parse();
        QString DOT = Neb::Parser::toDOT(statements);
        ui->dot_view->setPlainText(DOT);
    }catch(int code){
        if(code == 646){
            ui->dot_view->setPlainText(parser.getErrorMessage());
            Neb::Token t = parser.lastExaminedToken();
            QTextCursor c = ui->code_edit->textCursor();
            c.setPosition(t.start);
            c.setPosition(t.end, QTextCursor::KeepAnchor);
            ui->code_edit->setTextCursor(c);
            ui->code_edit->setFocus();
        }else throw code;
    }
}
