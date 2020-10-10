#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphvizCall.h>
class TypesetEdit;
class QSplitter;
class QTextEdit;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT

private:
    Ui::MainWindow* ui;
    QSplitter* splitter;
    TypesetEdit* code_edit;
    QTextEdit* code_view;
    QGraphvizCall::GraphVizView* graphviz_view;

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_codeEditChange();

    void on_actionShow_MathBran_toggled(bool checked);
    void on_actionShow_Graphviz_toggled(bool checked);
    void on_actionShow_Editor_toggled(bool checked);
};

#endif // MAINWINDOW_H
