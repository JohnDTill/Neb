#ifndef SCRIPT_TOKENS_H
#define SCRIPT_TOKENS_H

#include "trie.h"
#include <QDebug>
#include <QFile>
#include <QTextCodec>
#include <QtMath>

static ushort u(QString s){
    return s.front().unicode();
}

int processTokens(){
    //Settings
    const QString interval_delimiter = "Comma";
    const QString element_delimiter = "Bar";
    Q_ASSERT(interval_delimiter != element_delimiter);

    //Open table file
    QFile table(":/TokenTable.csv");
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
    std::vector<Entry> rows;
    table.readLine(); //Discard headers
    unsigned line = 1;
    while(!table.atEnd()){
        line++;
        QString line_str = table.readLine();
        if(line_str.contains(' ')){
            qDebug() << "Warning: line " << line << " contains a space.";
            continue;
        }else if(!line_str.contains("¦")){
            qDebug() << "Error: TokenTable.csv is not deliminated by '¦'";
            return 0;
        }
        QList<QString> entries = line_str.split("¦");

        QString name = entries.at(0);
        QString label = entries.at(1);
        bool is_one_to_one = entries.at(2) == "y";
        bool is_keyword = entries.at(3) == "y";
        bool is_mathbran = entries.at(4) == "y";
        bool implicit_mult = entries.at(5) == "y";
        bool in_use = parser_code.contains(name);

        rows.push_back(
            Entry(name, label, is_one_to_one, is_keyword, is_mathbran, implicit_mult, in_use)
        );

        if(!in_use) qDebug() << "TokenType" << name << "is not in use";
    }

    //Open gen file for writing
    QFile file("../neb_tokentype.h");
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
           "#ifndef NEB_TOKENTYPE_H\n"
           "#define NEB_TOKENTYPE_H\n"
           "\n"
           "namespace Neb{\n"
           "\n";

    //Enum definition
    out << "enum TokenType{\n";
    for(Entry e : rows) out << '\t' << e.name << ",\n";
    out << "};\n\n";

    //Labels
    out << "#define NEB_DECLARE_TOKEN_LABELS \\\n"
           "static const QString token_labels[" << rows.size() << "] { \\\n";
    for(Entry e : rows){
        out << "\t\"";
        if(e.is_mathbran) out << QChar(8284);
        else if(e.label == "\\") out << "\\";
        out << e.label << "\", \\\n";
    }
    out << "};\n\n";

    //Build trie of keywords, assuming keywords are sorted alphabetically
    std::vector<TrieNode> trie;
    for(Entry& e : rows) if(e.is_keyword) addTrieNode(e);
    writeTrie(out);

    //One-to-one tokens
    constexpr ushort USHORT_MAX = 55349;
    out << "#define NEB_ONE_TO_ONE_CASES_MACRO_EXPANSION";
    for(Entry e : rows)
        if(e.is_one_to_one && e.label.front() != USHORT_MAX){
            if(e.label == "\\n"){
                out << "\\\n\t case " << QChar('\n').unicode() << ": return createToken(" << e.name << ");";
            }else{
                out << "\\\n\t case " << e.label.front().unicode() << ": return createToken(" << e.name << ");";
            }
        }
    out << "\n\n";

    //Big tokens
    out << "#define NEB_BIG_ONE_TO_ONE_CASES_MACRO_EXPANSION";
    for(Entry e : rows)
        if(e.is_one_to_one && e.label.front() == USHORT_MAX)
            out << "\\\n\t case " << e.label.back().unicode() << ": return Token(" << e.name << ", line, curr-2, 2);";
    out << "\n\n";

    //MathBran
    out << "#define NEB_MATHBRAN_CASES_MACRO_EXPANSION";
    for(Entry e : rows)
        if(e.is_mathbran)
            out << "\\\n\t case " << e.label.front().unicode() << ": return Token(" << e.name << ", line, curr-2, 2);";
    out << "\n\n";

    //Numbers
    out << "#define NEB_NUMBER_CASES_MACRO_EXPANSION";
    out << "\\\n\t case '0': return scanZeroNumber();";
    for(int i = 1; i <= 9; i++)
        out << "\\\n\t case '" << i << "': return scanNonzeroNumber();";
    out << "\n\n";

    //Unicode script numbers
    out << "#define NEB_SCRIPTED_NUMBER_CASES_MACRO_EXPANSION";
    out << "\\\n\t case 8304: return scanSuperscriptZeroNumber();"; //Superscript numbers NOT ordered
    ushort sup[9] = {u("¹"), u("²"), u("³"), u("⁴"), u("⁵"), u("⁶"), u("⁷"), u("⁸"), u("⁹")};
    for(int i = 0; i < 9; i++)
        out << "\\\n\t case " << sup[i] << ": return scanSuperscriptNonzeroNumber();";

    ushort sub = QString("₀").front().unicode(); //Subscript numbers are ordered
    out << "\\\n\t case " << sub << ": return scanSubscriptZeroNumber();";
    for(int i = 1; i <= 9; i++)
        out << "\\\n\t case " << sub+i << ": return scanSubscriptNonzeroNumber();";
    out << "\n\n";

    //Size of enum
    out << "#define NEB_NUM_TOKENTYPES " << rows.size() << "\n\n";

    //Implicit mult signal
    int num_implicit_mult = 0;
    out << "#define NEB_IMPLICIT_MULT_MACRO_EXPANSION {";
    for(Entry e : rows)
        if(e.implicit_mult){
            num_implicit_mult++;
            out << "\\\n\t" << e.name << ",";
        }
    out << "\\\n}\n\n";
    out << "#define NEB_NUM_IMPLICIT_MULT " << num_implicit_mult << "\n\n";
    out << "}\n\n";

    //Cleanup
    out << "#endif\n";
    file.close();

    return 0;
}

#endif // SCRIPT_TOKENS_H
