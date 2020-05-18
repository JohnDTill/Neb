#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]){
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}

/*

int main(int argc, char *argv[]){
    std::vector<Parser::Node*> statements = Parser::parse("⁜^⏴x⏵⏴*⏵ ∈ ℝ\n y = ⁜^⏴x⏵⏴2⏵");
    QString DOT = Parser::toDOT(statements);

    QApplication a(argc, argv);
    QLabel l(DOT);
    l.setTextInteractionFlags(Qt::TextInteractionFlag::TextBrowserInteraction);
    l.show();

    return a.exec();
}
*/
