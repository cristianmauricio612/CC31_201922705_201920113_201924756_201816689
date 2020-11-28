#ifndef USER_H
#define USER_H

#include <QString>

#include "interaction.h"

class User {

private:

    QString Mail;
    QString Fullname;
    QString RegisterDate;
    QString Password;
    QVector <int> Friends;
    QVector <Interaction*> myinteractions;

public:

    User(QString  Mail = "",QString  Fullname = "",QString  RegisterDate = "",QString Password = "123") : Mail(Mail),
        Fullname(Fullname), RegisterDate(RegisterDate), Password(Password){}

    //MAIL
    QString getMail() { return Mail; }
    void setMail(QString Mail) { this->Mail = Mail; }

    //FULLNAME
    QString getFullname() { return Fullname; }
    void setFullname(QString Fullname) { this->Fullname = Fullname; }

    //REGISTER DATE
    QString getRegisterDate() { return RegisterDate; }
    void setRegisterDate(QString RegisterDate) { this->RegisterDate = RegisterDate; }

    //FRIENDS
    void agregar_amigo(int Friend) {
        Friends.append(Friend);
    }
    int numfollower(){
        return Friends.size();
    }
    int retorn_follow(int i){
        return Friends.at(i);
    }

    //
    QString getPassword() { return Password; }
    void setPassword(QString Password) { this->Password = Password; }

    //MY COMMENTS AND INTERACTIONS
    Interaction* retorn_inter(int i){
       return myinteractions.at(i);
    }

    void add_myinteractions(Interaction *nuevo) {
        myinteractions.append(nuevo);
    }
    int num_interactions() {
        return myinteractions.size();
    }
    void verif_interac(int verificar,bool &algo,QString &exist){
        int cont = 0;
        for(int i=0;i<myinteractions.size();i++){
            if(myinteractions.at(i)->getidP() == verificar){
                algo = myinteractions.at(i)->get_shared();
            }else cont++;
        }
        if(cont == myinteractions.size()){
            exist = "NO EXISTE";
        }else  exist = "EXISTE";
    }

    void edit_interaction(int idP,bool algo){
        for(int i=0;i<myinteractions.size();i++){
            if(myinteractions.at(i)->getidP()== idP){
                myinteractions.at(i)->set_shared(algo);
            }
        }
    }


};

#endif // USER_H
