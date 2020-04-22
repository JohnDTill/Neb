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
        if(parser.err_msg.isEmpty()){
            nodes.push_back(stmt);
        }else{
            QString::size_type index = stmt->data.front().unicode();
            if(errors.isEmpty()){
                QTextCursor c = ui->code_edit->textCursor();
                c.setPosition(index);
                c.setPosition(index + 1, QTextCursor::KeepAnchor);
                ui->code_edit->setTextCursor(c);
                ui->code_edit->setFocus();
            }else{
                errors.append('\n');
            }

            int line = 1 + code.leftRef(index).count('\n');
            errors.append( "Line " + QString::number(line) + " | " + parser.err_msg);
        }
    }

    if(errors.isEmpty()) ui->dot_view->setPlainText(Neb::NodeFunction::toDOT(nodes));
    else ui->dot_view->setPlainText(errors);
    for(Neb::Node* n : nodes) Neb::NodeFunction::deletePostorder(n);
}
