#include <QDebug>
#include <QFile>
#include <QTextCodec>

void skipHeader(QFile& table_file){
    table_file.readLine(); //Discard headers
}

void createIncludes(QTextStream& out){
    out << "//CODE-GEN FILE\n"
           "//This file is generated from subfolder \"meta\".\n"
           "//Changes to this file must be made in the meta project.\n"
           "\n"
           "#include <QString>\n"
           "#include <QHash>\n"
           "\n";
}

void createEnum(QFile& table_file, QTextStream& out, const QString& enum_name){
    skipHeader(table_file);

    out << "enum " + enum_name + " {\n";

    while(!table_file.atEnd()){
        QString line_str = table_file.readLine();
        QList<QString> entries = line_str.split(',');
        QString name = entries.at(0);
        out << ('\t' + name + ",\n").toUtf8();
    }
    out << "};\n\n";

    table_file.reset();
}

void createLabels(QFile& table_file, QTextStream& out, const QString& map_name, const QString& enum_name){
    skipHeader(table_file);

    out << "static const QHash<" + enum_name + ", QString> " + map_name + " {\n";
    while(!table_file.atEnd()){
        QString line_str = table_file.readLine();
        QList<QString> entries = line_str.split(',');

        QString name = entries.at(0);
        QString label = entries.at(1);
        if(label=='"') label = ',';
        out << ("\t{" + name + ", \"" + label + "\"},\n");
    }
    out << "};\n";

    table_file.reset();
}

void countEntries(QFile& table_file, QTextStream& out){
    out << "static constexpr int NodeType_Size = " << table_file.readAll().count('\n')-1 << ";\n\n";
    table_file.reset();
}

int main(int, char**){
    QFile node_table(":/NodeTypes.csv");
    if(!node_table.open(QIODevice::ReadOnly)){
        qDebug() << node_table.errorString();
        return 0;
    }

    QFile node_type_file("../nodetype.h");
    if(!node_type_file.open(QIODevice::WriteOnly)){
        qDebug() << node_type_file.errorString();
        return 0;
    }

    QFile token_table(":/TokenTypes.csv");
    if(!token_table.open(QIODevice::ReadOnly)){
        qDebug() << token_table.errorString();
        return 0;
    }

    QFile token_type_file("../tokentype.h");
    if(!token_type_file.open(QIODevice::WriteOnly)){
        qDebug() << token_type_file.errorString();
        return 0;
    }

    QTextStream out_node(&node_type_file);
    out_node.setCodec(QTextCodec::codecForMib(106)); //UTF8

    QTextStream out_token(&token_type_file);
    out_token.setCodec(QTextCodec::codecForMib(106)); //UTF8

    createIncludes(out_node);
    createEnum(node_table, out_node, "NodeType");
    countEntries(node_table, out_node);
    createLabels(node_table, out_node, "labels", "NodeType");
    node_type_file.close();

    createIncludes(out_token);
    createEnum(token_table, out_token, "TokenType");
    createLabels(token_table, out_token, "token_names", "TokenType");
    token_type_file.close();

    return 0;
}
