#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "avltree.h"
#include "list.h"
#include "publication.h"
#include "user.h"

#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    QVector<User*> lstUsuarios;
    QVector<Publication*> globalPub;
    QVector<Comment*> globalComm;
    User *prueba;
    Comment aux2;
    QList <QString> lstCorreos;

    //ARBOLES
    AvlTree<Publication*,int> *avlGlobalLikes;
    AvlTree<Publication*,QString> *avlGlobalFecha;
    AvlTree<Publication*,int> *avlGlobalRelevantes;

    AvlTree<Publication*,int> *avlGlobalLikesPerfil;
    AvlTree<Publication*,QString> *avlGlobalFechaPerfil;
    AvlTree<Publication*,int> *avlGlobalRelevantesPerfil;
    //LISTAS ORDENADAS
    List<Publication*> orden;
    List<Publication*> ordenTitulo;
    List<Publication*> ordenPerfil2;
    List<Publication*> ordenPerfil;
    int tipoFiltradoGeneral = 0; //0: Cualquier tipo 1: Por titulo
    int tipoFiltradoOrden = 0; //0: Like, 1: más reciente, 2: más antiguo, 3: más relevante
    bool perfil = true; //T: Perfiles, F1:Inicio
    QVBoxLayout* layout;
    vector<QLabel> vLabelComment;

    Publication *aux1;
    QString correo;
    int contpubs = 0;
    int contcomments = 0;

    //id del usuario que ingresa
    int idU;

    //id del perfil que quiero ver
    int perfriend=0;
    int idF;
    int pag;

    // PUBLICACIONES
    QVector<QLabel*> vLabelNameDate;
    QVector<QLabel*> vLabelTitleDesc;
    QVector<QLabel*> vLabelPhoto;
    QVector<QPushButton*> vBttnLikes;
    QVector<QPushButton*> vBttnComments;

    ~MainWindow();

public slots:
    void load();

private:
    Ui::MainWindow *ui;

private slots:
    void verComentariosP(int idP);
    void agregarConnects();
    void agregarPub();
    void agregarFollow();
    void agregarWidgets();
    void buscFri();
    void addLike();
    void iniciarSesion();
    void irRegistrar();
    void registrar();
    void regresarIniciarSesion();
    void salir();
    void verPerfil();
    void verPubGeneral();
    void verPubGeneralConFiltradoAnterior();
    void on_newComment_returnPressed();;
    void filtro();
    void verFriend();
    void verComentarios(int idP);
    void verComentariosGeneral(int idF);
    void volverFollows();
    void verFollw2();
    void seguir();
    void regresarPerfil();
    void filtradoLikes();
    void filtradoRecientes();
    void filtradoAntiguos();
    void filtradoRelevantes();
    void filtradoLikesPerfil();
    void filtradoRecientesPerfil();
    void filtradoAntiguosPerfil();
    void filtradoRelevantesPerfil();
    void llenarLabelsButtonsTotal();
    void llenarLabelsButtonsParcial();
    void QPushButtonChange(QPushButton* p1);
    void QPushButtonOriginal(QPushButton* p1, QPushButton* p2, QPushButton* p3);

    void bttnComment_1();
    void bttnComment_2();
    void bttnComment_3();
    void bttnComment_4();
    void bttnComment_5();
    void bttnComment_6();
    void bttnComment_7();
    void bttnComment_8();
    void bttnComment_9();
    void bttnComment_10();
    void bttnComment_11();
    void bttnComment_12();
    void bttnComment_13();
    void bttnComment_14();
    void bttnComment_15();
    void bttnComment_16();
    void bttnComment_17();
    void bttnComment_18();
    void bttnComment_19();
    void bttnComment_20();
    void bttnComment_21();
    void bttnComment_22();
    void bttnComment_23();
    void bttnComment_24();
    void bttnComment_25();
    void bttnComment_26();
    void bttnComment_27();
    void bttnComment_28();
    void bttnComment_29();
    void bttnComment_30();
    void bttnComment_31();
    void bttnComment_32();
    void bttnComment_33();
    void bttnComment_34();
    void bttnComment_35();
    void bttnComment_36();
    void bttnComment_37();
    void bttnComment_38();
    void bttnComment_39();
    void bttnComment_40();
    void bttnComment_41();
    void bttnComment_42();
    void bttnComment_43();
    void bttnComment_44();
    void bttnComment_45();
    void bttnComment_46();
    void bttnComment_47();
    void bttnComment_48();
    void bttnComment_49();
    void bttnComment_50();
    void on_verInformacion_clicked();
};
#endif // MAINWINDOW_H
