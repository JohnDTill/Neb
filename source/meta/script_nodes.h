#ifndef SCRIPT_NODES_H
#define SCRIPT_NODES_H

#include <QDebug>
#include <QFile>
#include <QTextCodec>

struct NodeEntry{
    QString name;
    QString label;
    bool in_use;

    NodeEntry(QString name, QString label, bool in_use)
        : name(name), label(label), in_use(in_use) {}
};

int processNodes(){
    //Open table file
    QFile table(":/NodeTable.csv");
    if(!table.open(QIODevice::ReadOnly)){
        qDebug() << table.errorString();
        return 0;
    }

    //Open parser definition file
    QFile parser("../neb_parser.cpp");
    if(!parser.open(QIODevice::ReadOnly)){
        qDebug() << parser.errorString();
        return 0;
    }
    QString parser_code = parser.readAll();

    //Process table file
    std::vector<NodeEntry> rows;
    table.readLine(); //Discard headers
    unsigned line = 1;
    while(!table.atEnd()){
        line++;
        QString line_str = table.readLine();
        QList<QString> entries = line_str.split("¦");

        QString name = entries.at(0);
        QString label = entries.at(1);
        bool in_use = parser_code.contains(name);

        rows.push_back(NodeEntry(name, label, in_use));
        if(!in_use) qDebug() << "NodeType" << name << "is not in use";
    }

    //Open gen file for writing
    QFile file("../neb_nodetype.h");
    if(!file.open(QIODevice::WriteOnly)){
        qDebug() << file.errorString();
        return 0;
    }

    QTextStream out(&file);
    out.setCodec(QTextCodec::codecForMib(106)); //UTF8

    //Front matter
    out << "//CODE-GEN FILE\n"
           "//This file is generated from subfolder \"meta\".\n"
           "//Changes to this file must be made in the meta project.\n"
           "\n"
           "#ifndef NEB_NODETYPE_H\n"
           "#define NEB_NODETYPE_H\n"
           "\n"
           "namespace Neb{\n"
           "\n";

    //Enum definition
    out << "enum NodeType{\n";
    for(NodeEntry e : rows) out << "    " << e.name << ",\n";
    out << "};\n\n";

    //Labels
    out << "#define NEB_DECLARE_NODE_LABELS \\\n"
            "static const QString labels[" << rows.size() << "] { \\\n";
    for(NodeEntry e : rows) out << "    \"" << e.label << "\", \\\n";
    out << "};\n\n";

    //Cleanup
    out << "}\n\n#endif\n";
    file.close();

    return 0;
}

#endif // SCRIPT_NODES_H
