#ifndef INTERACTION_H
#define INTERACTION_H

#include <QApplication>

class Interaction {

private:

    QString fecha;
    bool shared = false;
    int idP;

public:

    Interaction(QString fecha ="",bool shared = false,int idP = 0) :fecha(fecha),shared(shared),idP(idP) {}

    void set_fecha(QString valor) {
        this->fecha = valor;
    }
    QString get_fecha() {
        return fecha;
    }

    bool get_shared() {
        return shared;
    }
    void set_shared(bool share) {
         this->shared = share;
    }

    void setidP(int idP){
        this->idP=idP;
    }
    int getidP(){
        return this->idP;
    }
};

#endif // INTERACTION_H
