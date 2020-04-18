#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <neb_parser.h>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);

    ui->code_edit->setPlainText("x⁜^⏴*⏵ ∈ ℝ\ny = x⁜^⏴2⏵");
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_exec_button_clicked(){
    QString code = ui->code_edit->toPlainText();
    Neb::Parser parser(code);

    std::vector<Neb::Node*> nodes;
    while(Neb::Node* stmt = parser.parseStatement()){
        nodes.push_back(stmt);
        qDebug() << "PARSED";
        if(!parser.err_msg.isEmpty()){
            ui->dot_view->setPlainText(parser.err_msg);
            QTextCursor c = ui->code_edit->textCursor();
            //DO THIS: recover error location
            //c.setPosition(parser.errorStart());
            //c.setPosition(parser.errorEnd(), QTextCursor::KeepAnchor);
            ui->code_edit->setTextCursor(c);
            ui->code_edit->setFocus();
            for(Neb::Node* n : nodes) Neb::NodeFunction::deletePostorder(n);
            return;
        }
    }

    ui->dot_view->setPlainText(Neb::NodeFunction::toDOT(nodes, code));
    for(Neb::Node* n : nodes) Neb::NodeFunction::deletePostorder(n);
}
