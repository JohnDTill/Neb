#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <neb_parser.h>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);

    ui->code_edit->setPlainText("//The diffusion equation is:\n"
                                "⁜f⏴∂φ(r,t)⏵⏴∂t⏵ = ∇⋅ [D(φ, r) ∇φ(r, t)]");
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_exec_button_clicked(){
    QString code = ui->code_edit->toPlainText();
    Neb::Parser parser(code);
    QString errors;

    std::vector<Neb::Node*> nodes;
    while(Neb::Node* stmt = parser.parseStatement()){
        if(parser.ok()){
            nodes.push_back(stmt);
        }else{
            if(!errors.isEmpty()) errors.append('\n');
            errors.append(parser.err_msg);
        }
    }

    if(errors.isEmpty()) ui->dot_view->setPlainText(Neb::Node::toDOT(nodes));
    else ui->dot_view->setPlainText(errors);
    for(Neb::Node* n : nodes) Neb::Node::deletePostorder(n);
}
