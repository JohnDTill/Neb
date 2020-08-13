#ifndef TRIE_H
#define TRIE_H

#include <QDebug>
#include <QTextStream>
#include <vector>

struct Entry{
    QString name;
    QString label;
    bool is_one_to_one;
    bool is_keyword;
    bool is_mathbran;
    bool implicit_mult;
    bool in_use;

    Entry(QString name,
          QString label,
          bool is_one_to_one,
          bool is_keyword,
          bool is_mathbran,
          bool implicit_mult,
          bool in_use)
        : name(name),
          label(label),
          is_one_to_one(is_one_to_one),
          is_keyword(is_keyword),
          is_mathbran(is_mathbran),
          implicit_mult(implicit_mult),
          in_use(in_use) {}
};

static int max_key_len = 0;
static int min_key_len = 999;

struct TrieNode{
    QChar c;
    std::vector<TrieNode> next;
    int branch_count = 0;
    QString token_type;
    TrieNode(QChar c) : c(c){}
};

static std::vector<TrieNode> trie;

inline void addTrieNode(Entry& e){
    QString key = e.label;
    max_key_len = qMax(key.length(), max_key_len);
    min_key_len = qMin(key.length(), min_key_len);

    if(trie.empty() || key.front() != trie.back().c)
        trie.push_back(TrieNode(key.front()));

    TrieNode* curr = &trie.back();
    for(int i = 1; i < key.size(); i++){
        bool found_node = false;
        for(TrieNode& t : curr->next){
            if(t.c == key[i]){
                curr = &t;
                found_node = true;
                break;
            }
        }

        if(!found_node){
            curr->next.push_back(TrieNode(key[i]));
            curr = &curr->next.back();
            curr->token_type = e.name;
        }
    }
}

static int countBranches(TrieNode& t){
    if(t.next.empty()) t.branch_count = 1;
    else for(TrieNode& n : t.next) t.branch_count += countBranches(n);

    return t.branch_count;
}

static QString getWord(const TrieNode& t){
    if(t.next.empty()) return t.c;
    else if(t.next.size() == 1) return t.c + getWord(t.next[0]);

    qDebug() << "getWord() called on branching path";
    return QString();
}

static void indent(QTextStream& out, int level){
    for(int i = 0; i < level; i++) out << "    ";
}

static void writeTrie(QTextStream& out, const TrieNode& t, const QString& word){
    if(t.branch_count == 1 && t.next.empty()){
        indent(out, 1 + 2*word.size());
        out << "return key.size()==" << word.size() << " ? " << t.token_type << " : Identifier; \\\n";
    }else if(t.branch_count == 1){
        indent(out, 1 + 2*word.size());
        QString end = getWord(t.next[0]);
        if(end.size() > 1){
            out << "return key.mid(" << word.size() << ")==\"" << end << "\""
                   " ? " << t.next[0].token_type << " : Identifier; \\\n";
        }else{
            out << "return key.size()>=" << word.size()+1 << " && key[" << word.size() << "]"
                   "=='" << end << "' ? " << t.next[0].token_type << " : Identifier; \\\n";
        }
    }else{
        if(word.size()+1 > min_key_len){
            indent(out, 1 + 2*word.size());
            out << "if(key.size() < " << word.size()+1 << ") return Identifier; \\\n";
        }

        indent(out, 1 + 2*word.size());
        out << "switch(key[" << word.size() << "].unicode()){ \\\n";

        for(const TrieNode& n : t.next){
            indent(out, 2 + 2*word.size());
            out << "case '" << n.c << "': \\\n";
            writeTrie(out, n, word + n.c);
        }

        indent(out, 2 + 2*word.size());
        out << "default: return Identifier; \\\n";

        indent(out, 1 + 2*word.size());
        out << "} \\\n";
    }
}

inline void writeTrie(QTextStream& out){
    //Annotate tree with number of paths
    for(TrieNode& t : trie) countBranches(t);

    //Create keyword lookup function
    out << "#define NEB_DECLARE_KEYWORD_SEARCH \\\n"
           "static TokenType getTextLexemeType(const QStringRef& key){ \\\n"
           "    if(key.size() < " << min_key_len << " || "
           "key.size() > " << max_key_len << ") return Identifier; \\\n"
           "\\\n"
           "    switch(key[0].unicode()){ \\\n";

    for(TrieNode& t : trie){
        out << "        case '" << t.c << "': \\\n";
        writeTrie(out, t, t.c);
    }

    out << "        default: return Identifier; \\\n"
           "    } \\\n"
           "}\n\n";
}

#endif // TRIE_H
