#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <neb_parser.h>
#include <QGraphvizCall.h>
#include <QSplitter>
#include <QTextEdit>
#include <YAWYSIWYGEE>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);

    splitter = new QSplitter(this);
    setCentralWidget(splitter);

    code_edit = new TypesetEdit(this);
    code_edit->zoomOut(0.6);
    code_edit->setMathBran("//The diffusion equation is:\n"
                           "⁜f⏴∂φ(r,t)⏵⏴∂t⏵ = ∇⋅ [D(φ, r) ∇φ(r, t)]");
    splitter->addWidget(code_edit);

    code_view = new QTextEdit(this);
    code_view->setReadOnly(true);
    code_view->setWordWrapMode(QTextOption::WrapMode::NoWrap);
    splitter->addWidget(code_view);

    graphviz_view = new QGraphvizCall::GraphVizView(this);
    graphviz_view->scale(0.25, 0.25);
    splitter->addWidget(graphviz_view);

    connect(code_edit, SIGNAL(contentsChanged()), this, SLOT(on_codeEditChange()));
    on_codeEditChange();
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_codeEditChange(){
    const QString code = code_edit->toMathBran();
    code_edit->clearErrors();
    code_view->setPlainText(code);

    Neb::Parser parser(code);
    QString errors;

    std::vector<Neb::Node*> nodes;
    while(Neb::Node* stmt = parser.parseStatement()){
        if(parser.ok()){
            nodes.push_back(stmt);
        }else{
            if(!errors.isEmpty()) errors.append("<br>");
            int start_line = 1 + code.leftRef(parser.err_start).count('\n');
            errors.append("Line " + QString::number(start_line) + " | " + parser.err_msg);

            QTextCursor c = code_view->textCursor();
            c.setPosition(parser.err_start);
            c.setPosition(parser.err_end, QTextCursor::KeepAnchor);
            c.insertHtml("<span style=\"color:#ff0000;\"><u>" + c.selectedText() + "</u></span>");

            code_edit->reportError(code, parser.err_start, parser.err_end-parser.err_start, parser.err_msg);
        }
    }

    if(!errors.isEmpty()) code_view->append("<span style=\"color:#ff0000;\">" + errors + "</span>");

    const QString dot_code = Neb::Node::toDOT(nodes, true);
    for(Neb::Node* n : nodes) Neb::Node::deletePostorder(n);
    graphviz_view->update(dot_code);
}

void MainWindow::on_actionShow_MathBran_toggled(bool checked){
    code_view->setVisible(checked);
}

void MainWindow::on_actionShow_Graphviz_toggled(bool checked){
    graphviz_view->setVisible(checked);
}

void MainWindow::on_actionShow_Editor_toggled(bool checked){
    code_edit->setVisible(checked);
}
