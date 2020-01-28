#include <QDebug>
#include <QFile>
#include <QTextCodec>

int main(int, char**){
    QFile table_file(":/NodeTypes.csv");
    if(!table_file.open(QIODevice::ReadOnly)){
        qDebug() << table_file.errorString();
        return 0;
    }

    if(table_file.atEnd()){
        qDebug() << "File is empty.";
        return 0;
    }
    table_file.readLine(); //Discard headers

    QFile file("../nodetype.h");
    if(!file.open(QIODevice::WriteOnly)){
        qDebug() << file.errorString();
        return 0;
    }
    file.write("#include <QString>\n"
               "#include <QHash>\n"
               "\n"
               "enum NodeType {\n");

    const int utf8_code = 106;
    int code = utf8_code;

    while(!table_file.atEnd()){
        QByteArray line = table_file.readLine();
        QString line_str = QTextCodec::codecForMib(code)->toUnicode(line);
        QList<QString> entries = line_str.split(',');
        qDebug() << line;

        QString name = entries.at(0);
        file.write(('\t' + name + ",\n").toUtf8());
    }
    file.write("};\n\n");

    table_file.reset();
    table_file.readLine(); //Discard headers

    file.write("static const QHash<NodeType, QString> labels {\n");
    while(!table_file.atEnd()){
        QByteArray line = table_file.readLine();
        QString line_str = QTextCodec::codecForMib(code)->toUnicode(line);
        QList<QString> entries = line_str.split(',');

        QString name = entries.at(0);
        QString label = entries.at(1);
        file.write(("\t{" + name + ", \"" + label + "\"},\n").toUtf8());
    }
    file.write("};\n");

    file.close();

    return 0;
}
