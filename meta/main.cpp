#include <QDebug>
#include <QFile>
#include <QTextCodec>

const int code = 106;

void skipHeader(QFile& table_file){
    table_file.readLine(); //Discard headers
}

void createIncludes(QTextStream& out){
    out << "#include <QString>\n"
           "#include <QHash>\n"
           "\n";
}

void createEnum(QFile& table_file, QTextStream& out){
    skipHeader(table_file);

    out << "enum NodeType {\n";

    while(!table_file.atEnd()){
        QString line_str = table_file.readLine();
        QList<QString> entries = line_str.split(',');
        QString name = entries.at(0);
        out << ('\t' + name + ",\n").toUtf8();
    }
    out << "};\n\n";

    table_file.reset();
}

void createLabels(QFile& table_file, QTextStream& out){
    skipHeader(table_file);

    out << "static const QHash<NodeType, QString> labels {\n";
    while(!table_file.atEnd()){
        QString line_str = table_file.readLine();
        QList<QString> entries = line_str.split(',');

        QString name = entries.at(0);
        QString label = entries.at(1);
        out << ("\t{" + name + ", \"" + label + "\"},\n");
    }
    out << "};\n";
}

void countEntries(QFile& table_file, QTextStream& out){
    out << "static constexpr int NodeType_Size = " << table_file.readAll().count('\n')-1 << ";\n\n";
    table_file.reset();
}

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

    QFile gen_file("../nodetype.h");
    if(!gen_file.open(QIODevice::WriteOnly)){
        qDebug() << gen_file.errorString();
        return 0;
    }

    QTextStream out(&gen_file);
    out.setCodec(QTextCodec::codecForMib(code));
    createIncludes(out);
    createEnum(table_file, out);
    countEntries(table_file, out);
    createLabels(table_file, out);
    gen_file.close();

    return 0;
}
