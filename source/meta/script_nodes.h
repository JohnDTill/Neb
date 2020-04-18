#ifndef SCRIPT_NODES_H
#define SCRIPT_NODES_H

#include <QDebug>
#include <QFile>
#include <QTextCodec>

struct NodeEntry{
    QString name;
    QString label;

    NodeEntry(QString name, QString label)
        : name(name),
          label(label) {}
};

int processNodes(){
    //Open table file
    QFile table(":/NodeTable.csv");
    if(!table.open(QIODevice::ReadOnly)){
        qDebug() << table.errorString();
        return 0;
    }

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

        rows.push_back(NodeEntry(name, label));
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
           "#include <QString>\n"
           "#include <QHash>\n"
           "\n";

    //Enum definition
    out << "enum NodeType{\n";
    for(NodeEntry e : rows) out << '\t' << e.name << ",\n";
    out << "};\n\n";

    //Labels
    out << "#ifndef Neb_NDebug\n"
            "static const QHash<NodeType, QString> labels {\n";
    for(NodeEntry e : rows){
        out << "\t{" << e.name << ", \"";
        out << e.label << "\"},\n";
    }
    out << "};\n#endif\n\n";

    //Size of enum
    out << "#define NEB_NUM_NODETYPES " << rows.size() << "\n\n";

    //Cleanup
    out << "#endif\n";
    file.close();

    return 0;
}

#endif // SCRIPT_NODES_H
