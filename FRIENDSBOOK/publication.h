#ifndef PUBLICATION_H
#define PUBLICATION_H

#include <QListWidget>
#include <QString>

#include "comment.h"
#include "interaction.h"

using namespace std;

class Publication {

private:
    QString title;
    QString desc;
    QString pubdate;
    QVector <Comment*> comments;
    int numlikes;
    int idU;
    int id;
    QVector <Interaction*> interactions;

public:

    Publication(QString  title = "", QString  desc = "", QString  pubdate = "",int numlikes = 0,int idU = 0, int id = 0) : title(title),
        desc(desc), pubdate(pubdate),numlikes(numlikes),idU(idU), id(id) {}

    QString gettitle() { return title; }
    QString getdesc() { return desc; }
    QString getpubdate() { return pubdate; }
    QVector <Comment*> getAllComentarios() {
        return comments;
    }
    int getnum_comments() {
        return comments.size();
    }
    int getnum_interactions() {
        int numshares=0;
        for(int i=0;i<interactions.size();i++){
        if(interactions.at(i)->get_shared()==true){
            numshares++;
        }
    }
        return numshares;
    }
    int getnumlikes() {
        return numlikes;
    }
    void settitle(QString title) { this->title = title; }
    void setdesc(QString desc) { this->desc = desc; }
    void setpubdate(QString pubdate) { this->pubdate = pubdate; }
    void agregar_comments(Comment *nuevo) {
        comments.append(nuevo);
    }
    void agregar_interactions(Interaction *nuevo) {
        interactions.append(nuevo);
    }
    void darlike() {
        numlikes++;
    }
    void quitarlike() {
        numlikes--;
    }
    Comment* getcomment(int pos){
        return comments.at(pos);
    }
    Interaction* getinteraction(int pos){
        return interactions.at(pos);
    }
    void imprimircomments(QListWidget* ejemp){
        QString text;
        QString date;
        for(int i=0;i<comments.size();i++){
        text = comments.at(i)->get_text();
        date = comments.at(i)->get_date();
        QString comm = "Fecha: "+date+"\n"+text;
        ejemp->addItem(comm);
        }
    }
    void editar_interac(int idP,bool share){
        for(int i=0;i<interactions.size();i++){
            if(interactions.at(i)->getidP() == idP){
                interactions.at(i)->set_shared(share);
            }
        }
    }
    int getidU() {return idU;}
    int getId() {return id;}
    void setidU(int idU){this->idU = idU;}

};

#endif // PUBLICATION_H
