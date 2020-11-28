#ifndef LIST_H
#define LIST_H

#include <QApplication>
#include <string>

template <typename T>
class Nodo {
public:
    T data;
    Nodo* next;
    Nodo* ant;
    Nodo(T data, Nodo* next = nullptr, Nodo* ant = nullptr) : data(data), next(next), ant(ant) {}
};

template <typename T>
class List {
public:
    Nodo<T>* head = nullptr;

    Nodo<T>* selected = nullptr;

    void agregar(T data) {
        Nodo<T>* nodo = new Nodo<T>(data);

        if (this->head == nullptr)
        {
            this->head = nodo;
            this->selected = head;
            return;
        }

        Nodo<T>* last = this->head;
        while (last->next != nullptr)
        {
            last = last->next;
        }

        last->next = nodo;
        nodo->ant = last;

    }

    void eliminar() {
        Nodo<T>* temp = this->selected;
        //verificar si está vacio
        if (temp == nullptr) return;

        //verificar si es el unico nodo
        if (temp->ant == nullptr and temp->next == nullptr) {
            this->head = nullptr;
            selected = head;
            free(temp);
            return;
        }

        if (temp->ant != nullptr) {
            temp->ant->next = temp->next;
            this->selected = temp->ant;
        }

        if (temp->next != nullptr) {
            temp->next->ant = temp->ant;
            this->selected = temp->next;
            free(temp);
        }

    }

    int tam_list() {
        int count = 0;
        Nodo<T>* current = this->head;
        while (current != nullptr)
        {
            count++;
            current = current->next;
        }
        return count;
    }

    void sgt() {
        if (this->selected->next != nullptr)
        {
            selected = selected->next;
        }
    }

    void ant() {
        if (this->selected->ant != nullptr)
        {
            selected = selected->ant;
        }
    }

    bool verif_vacio() {
        return this->head == nullptr;
    }

    bool find(T* elem) {

        Nodo<T>* aux = head;
        for (int i = 0; i < tam_list(); ++i) {
            if (aux->data == *elem) {
                return true;
            }
            aux = aux->next;
        }
        return false;
    }

    T getPos(int pos) {
        if (pos < 0 || pos >= tam_list()) -.0;
        Nodo<T>* aux = head;
        for (int i = 0; i < pos; ++i) {
            aux = aux->next;
        }
        return aux->data;
    }

    void reemplazar(int pos, T nuevo) {
        if (pos < 0 || pos >= tam_list()) -.0;
        Nodo<T>* aux = head;
        for (int i = 0; i < pos; ++i) {
            aux = aux->next;
        }
        aux->data = nuevo;
    }

    void addTree(T data) {
            if(tam_list()<=50) {
                agregar(data);
            }
        }

};

#endif // LIST_H
