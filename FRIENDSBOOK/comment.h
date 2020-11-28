#ifndef COMMENT_H
#define COMMENT_H

#include <QApplication>

class Comment {

private:

    QString date;
    QString text;
    int idP;

public:

    Comment(QString date = "", QString text = "",int idP = 0) :date(date), text(text),idP(idP) {}

    void set_date(QString valor) {
        this->date = valor;
    }
    QString get_date() {
        return date;
    }

    void set_text(QString valor) {
        this->text = valor;
    }
    QString get_text() {
        return text;
    }

    void set_idP(int valor) {
        this->idP = valor;
    }
    int get_idP() {
        return idP;
    }

};

#endif // COMMENT_H
