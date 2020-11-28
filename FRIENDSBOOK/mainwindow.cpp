#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <fstream>
#include <QDate>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //INICIAR SESIÓN O REGISTRARSE
    connect(ui->bttnLogin,SIGNAL(released()),this,SLOT(iniciarSesion()));
    connect(ui->bttnGoSignup,SIGNAL(released()),this,SLOT(irRegistrar()));
    connect(ui->bttnSignup,SIGNAL(released()),this,SLOT(registrar()));
    connect(ui->bttnBack,SIGNAL(released()),this,SLOT(regresarIniciarSesion()));

    //MENU
    connect(ui->menuHome,SIGNAL(released()),this,SLOT(verPubGeneral()));
    connect(ui->menuProfile,SIGNAL(released()),this,SLOT(verPerfil()));
    connect(ui->menuLogut,SIGNAL(released()),this,SLOT(salir()));
    connect(ui->menuHomeIcon,SIGNAL(released()),this,SLOT(verPubGeneral()));
    connect(ui->menuProfileIcon,SIGNAL(released()),this,SLOT(verPerfil()));
    connect(ui->menuLogutIcon,SIGNAL(released()),this,SLOT(salir()));


    //FILTRADO
    connect(ui->filtrar,SIGNAL(released()),this,SLOT(filtro()));
    connect(ui->orderLikes,SIGNAL(released()),this,SLOT(filtradoLikes()));
    connect(ui->orderRecientes,SIGNAL(released()),this,SLOT(filtradoRecientes()));
    connect(ui->orderAntiguos,SIGNAL(released()),this,SLOT(filtradoAntiguos()));
    connect(ui->orderRelevante,SIGNAL(released()),this,SLOT(filtradoRelevantes()));

    //FILTRADO PERFIL
    connect(ui->orderLikesProfile,SIGNAL(released()),this,SLOT(filtradoLikesPerfil()));
    connect(ui->orderRecientesProfile,SIGNAL(released()),this,SLOT(filtradoRecientesPerfil()));
    connect(ui->orderAntiguosProfile,SIGNAL(released()),this,SLOT(filtradoAntiguosPerfil()));
    connect(ui->orderRelevanteProfile,SIGNAL(released()),this,SLOT(filtradoRelevantesPerfil()));

    //COMENTARIOS
    connect(ui->regresar,SIGNAL(released()),this,SLOT(verPubGeneralConFiltradoAnterior()));

    //AÑDIR PUBLICACION
    connect(ui->addpublication,SIGNAL(released()),this,SLOT(agregarPub()));

    //FOLLOWERS
    connect(ui->bttnSearch,SIGNAL(released()),this,SLOT(buscFri()));
    connect(ui->seeprofile,SIGNAL(released()),this,SLOT(verFriend()));
    connect(ui->bttnGoSearch,SIGNAL(released()),this,SLOT(agregarFollow()));
    connect(ui->bttnBack_2,SIGNAL(released()),this,SLOT(volverFollows()));
    connect(ui->bttnGoProfile_2,SIGNAL(released()),this,SLOT(verFollw2()));
    connect(ui->follow,SIGNAL(released()),this,SLOT(seguir()));

    //PUBLICACIONES
    agregarConnects();
    connect(ui->bttnLikeOnePub,SIGNAL(released()),this,SLOT(addLike()));

    connect(ui->regresarProfile,SIGNAL(released()),this,SLOT(regresarPerfil()));
}

MainWindow::~MainWindow()
{
    QFile fileU("../FRIENDSBOOK/users.tsv");
        if(fileU.open(QIODevice::WriteOnly | QIODevice::Text)) {
             QTextStream outU(&fileU);
             for(int i = 0; i<lstUsuarios.size();i++){
                 outU << QString::number(i+1) + "\t" + lstUsuarios.at(i)->getMail() + "\t" + lstUsuarios.at(i)->getFullname() + "\t" + lstUsuarios.at(i)->getRegisterDate()+"\t"+ lstUsuarios.at(i)->getPassword() +"\n";
             }
        }
        fileU.close();

        QFile fileP("../FRIENDSBOOK/publications.tsv");
        if(fileP.open(QIODevice::WriteOnly | QIODevice::Text)) {
             QTextStream outP(&fileP);
             for(int i = 0; i<globalPub.size();i++){
                 outP << QString::number(i+1) + "\t" + QString::number(globalPub.at(i)->getidU()) + "\t" + globalPub.at(i)->gettitle() + "\t" + globalPub.at(i)->getdesc() + "\t" + globalPub.at(i)->getpubdate()+"\t"+QString::number(globalPub.at(i)->getnumlikes())+"\n";
             }
        }
        fileP.close();

        QFile fileC("../FRIENDSBOOK/comment.tsv");
        if(fileC.open(QIODevice::WriteOnly | QIODevice::Text)) {
             QTextStream outC(&fileC);
             for(int i = 0; i<globalComm.size();i++){
                 outC << QString::number(i+1) + "\t" + QString::number(globalComm.at(i)->get_idP()) + "\t" + globalComm.at(i)->get_date() + "\t" + globalComm.at(i)->get_text()+"\n";
             }
        }
        fileC.close();

        QFile fileI("../FRIENDSBOOK/interaction.tsv");
        if(fileI.open(QIODevice::WriteOnly | QIODevice::Text)) {
             QTextStream outI(&fileI);
             for(int i = 0; i<lstUsuarios.size();i++){
                 QString share;
                 for(int j=0;j<lstUsuarios.at(i)->num_interactions();j++){
                     if(lstUsuarios.at(i)->retorn_inter(j)->get_shared() == true){
                         share = "TRUE";
                     }else share = "FALSE";
                 outI << QString::number(i+1) + "\t" + QString::number(lstUsuarios.at(i)->retorn_inter(j)->getidP()) + "\t" + lstUsuarios.at(i)->retorn_inter(j)->get_fecha() + "\t" + share+"\n";
        }
             }
        }
        fileI.close();

        QFile fileF("../FRIENDSBOOK/followers.tsv");
        if(fileF.open(QIODevice::WriteOnly | QIODevice::Text)) {
             QTextStream outF(&fileF);
             for(int i = 0; i<lstUsuarios.size();i++){
                 for(int j=0;j<lstUsuarios.at(i)->numfollower();j++)
                 outF << QString::number(i+1) + "\t" + QString::number(lstUsuarios.at(i)->retorn_follow(j))+"\n";
             }
        }
        fileF.close();
    delete ui;
}

void MainWindow::agregarPub(){
    QString currentPubDate = QDate::currentDate().toString("yyyy-MM-dd");
    QString currentPub = ui->cajapublic->toPlainText();
    QString currentTittle = ui->pubTittleBox->toPlainText();
    if(currentPub != "" && currentTittle != "" && currentPubDate != ""){
        Publication *auxPub = new Publication(currentTittle,currentPub,currentPubDate,0,idU+1);
        globalPub.push_back(auxPub);
        avlGlobalLikes->add(auxPub);
        avlGlobalFecha->add(auxPub);
        avlGlobalRelevantes->add(auxPub);
        avlGlobalLikesPerfil->add(auxPub);
        avlGlobalFechaPerfil->add(auxPub);
        avlGlobalRelevantesPerfil->add(auxPub);
        ordenTitulo.agregar(auxPub);
        ordenPerfil.agregar(auxPub);
        ui->cajapublic->setText("");
        ui->pubTittleBox->setText("");
        if(tipoFiltradoOrden == 0) {
            filtradoLikesPerfil();
        } else if(tipoFiltradoOrden == 1) {
            filtradoRecientesPerfil();
        } else if(tipoFiltradoOrden == 2) {
            filtradoAntiguosPerfil();
        } else if(tipoFiltradoOrden == 3) {
            filtradoRelevantesPerfil();
        }
        ui->pPub->setText(QString::number(ui->pPub->text().toInt() + 1));
    }
}

void MainWindow::agregarFollow(){
    ui->stackedWidgetFollow->setCurrentIndex(1);
    ui->fError->hide();
}

void MainWindow::buscFri(){
    ui->people->clear();
        if(ui->search->text() != ""){
        QString buscare = ui->search->text();
        if(ui->search->text().size() > 1){
            for(int i=0;i<lstUsuarios.size();i++){
                 QString elem = lstUsuarios.at(i)->getMail();
                 bool ver = false;
                for(int j = 0 ; j<lstUsuarios.at(idU)->numfollower();j++){
                    if(lstUsuarios.at(idU)->retorn_follow(j) == i+1){
                    ver= true;
                    }
                }
                if(elem[0] == buscare[0] && elem[1] == buscare[1] && ver == false){
                ui->people->addItem(elem);
                }
            }
            ui->search->clear();
        }else {ui->bError_2->show();ui->bError_2->setText("MINIMO DOS LETRAS");}
    }else {ui->bError_2->show();ui->bError_2->setText("MINIMO DOS LETRAS");}
}

void MainWindow::addLike(){
    bool nuevo;
    QString existem;

    lstUsuarios.at(idU)->verif_interac(idF+1,nuevo,existem);
    if(existem == "EXISTE"){
        if(nuevo == false){
            globalPub.at(idF)->darlike();
            globalPub.at(idF)->editar_interac(idF+1,true);
            lstUsuarios.at(idU)->edit_interaction(idF+1,true);
            ui->bttnLikeOnePub->setText("thumbs-up "+ QString::number(globalPub.at(idF)->getnumlikes()));
            avlGlobalLikes->clear();
                       for(int i=0;i<globalPub.size();i++){
                           avlGlobalLikes->add(globalPub.at(i));
                       }
        }
        else{
            globalPub.at(idF)->quitarlike();
            globalPub.at(idF)->editar_interac(idF+1,false);
            lstUsuarios.at(idU)->edit_interaction(idF+1,false);
            ui->bttnLikeOnePub->setText("thumbs-up "+ QString::number(globalPub.at(idF)->getnumlikes()));
            avlGlobalLikes->clear();
                       for(int i=0;i<globalPub.size();i++){
                           avlGlobalLikes->add(globalPub.at(i));
                       }
         }
    }else if(existem == "NO EXISTE"){
        Interaction *nuevecito = new Interaction("",true,idF+1);
        globalPub.at(idF)->darlike();
        globalPub.at(idF)->agregar_interactions(nuevecito);
        lstUsuarios.at(idU)->add_myinteractions(nuevecito);
        ui->bttnLikeOnePub->setText("thumbs-up "+ QString::number(globalPub.at(idF)->getnumlikes()));
        avlGlobalLikes->clear();
                   for(int i=0;i<globalPub.size();i++){
                       avlGlobalLikes->add(globalPub.at(i));
                   }
    }

    /*
    if (tipoFiltradoOrden == 0) {
        avlGlobalLikes->clear();
        for(Publication* pub : globalPub){
            avlGlobalLikes->add(pub);
        }
    } else if (tipoFiltradoOrden == 1 || tipoFiltradoOrden == 2) {
        avlGlobalFecha->clear();
        for(Publication* pub : globalPub){
            avlGlobalFecha->add(pub);
        }
    } else if (tipoFiltradoOrden == 3) {
        avlGlobalRelevantes->clear();
        for(Publication* pub : globalPub){
            avlGlobalRelevantes->add(pub);
        }
    }
    */
}

void MainWindow::iniciarSesion() {
    correo = ui->iEmail->text();
    QString password = ui->iPassword->text();

    if(correo != "" && password != ""){
        if(lstUsuarios.size() != 0){
            for(int i=0;i<lstUsuarios.size();i++){
                if(lstUsuarios.at(i)->getMail() == correo && lstUsuarios.at(i)->getPassword() == password){
                    for(int j=0;j<lstUsuarios.at(i)->numfollower();j++){
                        int amigo;
                        amigo = lstUsuarios.at(i)->retorn_follow(j) - 1 ;
                        ui->follows->addItem(lstUsuarios.at(amigo)->getFullname());
                    }
                    ui->lbl->hide();
                    idU=i;
                    verPubGeneral();
                    ui->iEmail->setText("");
                    ui->iPassword->setText("");
                }
            }
            ui->iError->show();
            ui->iError->setText("INGRESE UN CORREO EXISTENTE");
        }else{
             ui->iError->show();
             ui->iError->setText("INGRESE UN CORREO EXISTENTE");
        }
    }else{
        ui->iError->show();
        ui->iError->setText("PRIMERO INGRESE UN CORREO");
    }
}

void MainWindow::irRegistrar() {
    ui->stackedWidgetLogin->setCurrentIndex(1);
    ui->rError->hide();
    ui->iError->hide();
}

void MainWindow::load() {
    ui->lbl->show();
    ui->iError->hide();
    ui->rError->hide();
    ui->follow->hide();
    ui->bError_2->hide();
    ui->fError->hide();
    agregarWidgets();
    layout = new QVBoxLayout;
    ui->stackedWidgetFollow->setCurrentIndex(0);
    ui->stackedWidgetGeneral->setCurrentIndex(0);
    ui->stackedWidgetLogin->setCurrentIndex(0);

    fstream seg("../FRIENDSBOOK/users.tsv");

    if (seg.is_open()) {
        string line;
        QString mail, nick, date, password;
        while (getline(seg, line, '\t')) {
            getline(seg, line, '\t');
            mail = QString::fromStdString(line);
            getline(seg, line, '\t');
            nick = QString::fromStdString(line);
            getline(seg, line, '\t');
            date = QString::fromStdString(line);
            getline(seg, line);
            password= QString::fromStdString(line);
            lstCorreos.push_back(mail);   //PENDIENTE
            lstUsuarios.push_back(new User(mail,nick,date,password));
        }
    }

    fstream ter("../FRIENDSBOOK/publications.tsv");

    if(ter.is_open()){

        string line;
        int  numLikes, idUs, count=0;
        QString QpubDate, Qdesc, Qtitulo;
        while (getline(ter, line, '\t')) {
                getline(ter, line, '\t');
                idUs = (QString::fromStdString(line)).toInt();
                getline(ter, line, '\t');
                Qtitulo= QString::fromStdString(line);
                getline(ter, line, '\t');
                Qdesc= QString::fromStdString(line);
                getline(ter, line, '\t');
                QpubDate= QString::fromStdString(line);
                getline(ter, line);
                numLikes = (QString::fromStdString(line)).toInt();
                globalPub.push_back(new Publication(Qtitulo,Qdesc,QpubDate,numLikes,idUs, count));
                ++count;
        }
    }

    fstream com("../FRIENDSBOOK/comment.tsv");

    if (com.is_open()) {
        string line;
        int idPub;
        int id;
        QString date, text;
        while (getline(com, line, '\t')) {
            id = (QString::fromStdString(line)).toInt();
            getline(com, line, '\t');
            idPub = (QString::fromStdString(line)).toInt();
            getline(com, line, '\t');
            date = QString::fromStdString(line);
            getline(com, line);
            text = QString::fromStdString(line);
            globalComm.push_back(new Comment(date,text,idPub));
            globalPub.at(idPub-1)->agregar_comments(new Comment(date,text));
        }
    }

    fstream inter("../FRIENDSBOOK/interaction.tsv");

    if (inter.is_open()) {
        string line;
        int idPub, id;
        QString date, text;
        Interaction *aux;
        while (getline(inter, line, '\t')) {
            id = (QString::fromStdString(line)).toInt();
            getline(inter, line, '\t');
            idPub = (QString::fromStdString(line)).toInt();
            getline(inter, line, '\t');
            date = QString::fromStdString(line);
            getline(inter, line);
            text = QString::fromStdString(line);
            if(line == "TRUE")
                aux = new Interaction(date,true,idPub);
            else
                aux = new Interaction(date,false,idPub);
            lstUsuarios.at(id-1)->add_myinteractions(aux);
            globalPub.at(idPub-1)->agregar_interactions(aux);
        }
    }

    fstream follow("../FRIENDSBOOK/followers.tsv");

    if(follow.is_open()){
        string line;
        int idu, idF;
        User aux;
        while (getline(follow, line, '\t')) {
            idu = (QString::fromStdString(line)).toInt();
            getline(follow, line);
            idF = (QString::fromStdString(line)).toInt();
            lstUsuarios.at(idu-1)->agregar_amigo(idF);
        }
    }

    avlGlobalLikes = new AvlTree<Publication*,int>([](Publication* lp){return lp->getnumlikes();});
    avlGlobalFecha = new AvlTree<Publication*,QString>([](Publication* lp){return lp->getpubdate();});
    avlGlobalRelevantes = new AvlTree<Publication*,int>([](Publication* lp){return lp->getnum_comments();});

    avlGlobalLikesPerfil = new AvlTree<Publication*,int>([](Publication* lp){return lp->getnumlikes();});
    avlGlobalFechaPerfil = new AvlTree<Publication*,QString>([](Publication* lp){return lp->getpubdate();});
    avlGlobalRelevantesPerfil = new AvlTree<Publication*,int>([](Publication* lp){return lp->getnum_comments();});

    for(int i=0; i<globalPub.size();i++){
        avlGlobalLikes->add(globalPub.at(i));
        avlGlobalFecha->add(globalPub.at(i));
        avlGlobalRelevantes->add(globalPub.at(i));
    }
}

void MainWindow::registrar() {
    correo=ui->rEmail->text();
    QString date = QDate::currentDate().toString("yyyy-MM-dd");
    QString nick = ui->rName->text();
    QString password = ui->rPassword->text();
    int verif = 0;
     int verifes = 0;

    if(correo != "" && password != ""){
       if(correo.size()>= 7){
        if(correo[0] != '@' && correo[correo.size()-4] == '.' && correo[correo.size() - 3] == 'c' && correo[correo.size() - 2] == 'o' && correo[correo.size()-1] == 'm' && correo[correo.size()-5] != '@'){

            for (int i = 0; i < correo.size(); i++) {
                        if (correo[i] == '@') {
                            verif++;
                        }
                        if (correo[i] == ' ') {
                            verifes++;
                        }
                    }
            if(lstCorreos.size() != 0 && verif == 1 && verifes == 0){
            if(lstCorreos.contains(correo) == true){
                ui->rError->show();
                ui->rError->setText("EL CORREO YA EXISTE");
            }else{
            lstCorreos.push_back(correo);
            prueba = new User(correo,nick,date,password);
            lstUsuarios.push_back(prueba);
            ui->rEmail->setText("");
            ui->rPassword->setText("");
            ui->rName->setText("");
            ui->rError->show();
            ui->rError->setStyleSheet("QLabel {color: white}");
            ui->rError->setText("REGISTRO COMPLETADO");
            }

            }else{
                if(lstCorreos.size() == 0 && verif==1 && verifes == 0){
                    lstCorreos.push_back(correo);
                    prueba = new User(correo,nick,date,password);
                    lstUsuarios.push_back(prueba);
                    ui->rEmail->setText("");
                    ui->rPassword->setText("");
                    ui->rName->setText("");
                    ui->rError->show();
                    ui->rError->setStyleSheet("QLabel {color: white}");
                    ui->rError->setText("REGISTRO COMPLETADO");
                }
                else if(lstCorreos.size() == 0 && verif !=1 && verifes == 0){
                    ui->rError->show();
                    ui->rError->setText("FORMATO DEL CORREO: user@algo.com");

                }else if(lstCorreos.size() != 0 && verif !=1 && verifes == 0){
                    ui->rError->show();
                    ui->rError->setText("FORMATO DEL CORREO: user@algo.com");
                }else if(lstCorreos.size() != 0 && verif == 1 && verifes != 0){
                    ui->rError->show();
                    ui->rError->setText("NO COLOQUE ESPACIOS");
                }
        }
        }else{
            ui->rError->show();
            ui->rError->setText("EL CORREO DEBE TENER LA FORMA user@algo.com");
        }
    }
       else{
        ui->rError->show();
        ui->rError->setText("EL CORREO DEBE TENER LA FORMA user@algo.com");
       }
       }else{
        ui->rError->show();
        ui->rError->setText("PRIMERO COMPLETE LOS RECUADROS");
    }
}

void MainWindow::regresarIniciarSesion() {
    ui->stackedWidgetLogin->setCurrentIndex(0);
    ui->rError->hide();
    ui->rError->setStyleSheet("QLabel {color:  rgb(255, 0, 4)}");
    ui->iError->hide();
}

void MainWindow::salir() {
    ui->stackedWidgetGeneral->setCurrentIndex(0);
    ui->stackedWidgetLogin->setCurrentIndex(0);
    ui->lbl->show();
    ui->iError->hide();
    ui->follows->clear();
}

void MainWindow::verPerfil() {
    ui->follow->hide();
    ui->mypublications->setGeometry(260,90,761,361);
    ui->verInformacion->setGeometry(548,460,191,31);
    ui->stackedWidgetGeneral->setCurrentIndex(2);
    ui->mypublications->clear();
    ui->stackedWidgetGeneral->setCurrentIndex(2);
    ui->pubTittleBox->show();
    ui->cajapublic->show();
    ui->heart->hide();
    ui->addpublication->show();
    ui->pName->setText(lstUsuarios.at(idU)->getFullname());
    ui->pEmail->setText(lstUsuarios.at(idU)->getMail());
    QStringList list = lstUsuarios.at(idU)->getRegisterDate().split(QLatin1Char('-'));
    QString date = list[2] + " de ";
    switch(list[1].toInt()) {
        case 1: date += "enero"; break;
        case 2: date += "febrero"; break;
        case 3: date += "marzo"; break;
        case 4: date += "abril"; break;
        case 5: date += "mayo"; break;
        case 6: date += "junio"; break;
        case 7: date += "julio"; break;
        case 8: date += "agosto"; break;
        case 9: date += "septiembre"; break;
        case 10: date += "octubre"; break;
        case 11: date += "noviembre"; break;
        case 12: date += "diciembre"; break;
    }
    date += " de " + list[0];
    ui->pDate->setText(date);
    ui->pFollow->setText(QString::number(lstUsuarios.at(idU)->numfollower()));
    int pubs = 0;
    QString publi;
    List<Publication*> aux;
    //PUBLICACIONES
    for(int i=0;i<globalPub.size();i++){
        if(globalPub.at(i)->getidU() == idU + 1){
            pubs++;
            publi = "Titulo: " + globalPub.at(i)->gettitle() + "\nFecha: " + globalPub.at(i)->getpubdate() + "\n" + globalPub.at(i)->getdesc();
            ui->mypublications->addItem(publi);
            aux.agregar(globalPub.at(i));
        }
    }

    ordenPerfil = aux;

    ui->pPub->setText(QString::number(pubs));
    perfriend = idU;
    filtradoLikesPerfil();
}

void MainWindow::verComentariosP(int idF) {
    ui->regresarProfile->show();
    ui->regresar->hide();
    Publication* pub = globalPub.at(idF);
    ui->nameOnePub->setText(lstUsuarios.at(pub->getidU()-1)->getFullname());
    QStringList list = pub->getpubdate().split(QLatin1Char('-'));
    QString date = list[2] + " de ";
    switch(list[1].toInt()) {
        case 1: date += "enero"; break;
        case 2: date += "febrero"; break;
        case 3: date += "marzo"; break;
        case 4: date += "abril"; break;
        case 5: date += "mayo"; break;
        case 6: date += "junio"; break;
        case 7: date += "julio"; break;
        case 8: date += "agosto"; break;
        case 9: date += "septiembre"; break;
        case 10: date += "octubre"; break;
        case 11: date += "noviembre"; break;
        case 12: date += "diciembre"; break;
    }
    date += " de " + list[0];
    ui->dateOnePub->setText(date);
    ui->onePub->setText(pub->gettitle() + "\n" + pub->getdesc());
    ui->onePub->setWordWrap(true);
    ui->bttnLikeOnePub->setText("thumbs-up "+ QString::number(pub->getnumlikes()));
    ui->bttnCommentOnePub->setText("comment-alt "+ QString::number(pub->getnum_comments()));

    QLayoutItem* child;
    while((child = layout->takeAt(0))) {
        delete child->widget();
        delete child;
    }

    AvlTree<Comment*,QString>* avlFechaCommentario = new AvlTree<Comment*,QString>([](Comment* c){return c->get_date();});
    for(Comment* c : pub->getAllComentarios()) {
        avlFechaCommentario->add(c);
    }
    auto b = [&](Comment* c) {
        QLabel* label = new QLabel();
        label->setStyleSheet({"border: 1.1px solid #222A48;"
                              "border-color: rgb(255, 255, 255);"
                              "color:#FFFFFF;"
                              "font: 10pt Century Gothic;"});

        layout->addWidget(label);
        label->setText(c->get_text());
        label->setWordWrap(true);
    };
    avlFechaCommentario->postOrderTodo(b);
    ui->scrollPubComments->setLayout(layout);
}

void MainWindow::on_verInformacion_clicked()
{
    if(ui->mypublications->count() != 0 && ui->mypublications->selectedItems().size() != 0){
       QString pub = ui->mypublications->currentItem()->text();
       int esp=0;
       int salto=0;
       QString title;
       QString date;
       QString desc;
       for(int i=0;i<pub.size();i++){

           if(pub[i]=='\n'){
               esp = 0;
               salto++;
           }
           if(esp > 0 && salto == 0){
               title = title + pub[i];
           }else if(esp > 0 && salto == 1 && pub[i] !='\n'){
               date = date + pub[i];
           }else if(salto == 2 && pub[i]!='\n'){
               desc = desc + pub[i];
           }
           if(pub[i]==' '){
               esp++;
           }
       }
       Publication* temp = new Publication(title,desc,date);
       aux1 = temp;
       //ui->publicacion->addItem(pub);
       for(int i=0;i<globalPub.size();i++){
           temp = globalPub.at(i);
           if (temp->getdesc() == aux1->getdesc() && temp->getpubdate() == aux1->getpubdate() && temp->gettitle() == aux1->gettitle()) {

               //temp->imprimircomments(ui->comments);
               idF= i;
               verComentariosP(idF);
               ui->stackedWidgetGeneral->setCurrentIndex(1);
               ui->stackedWidgetParcial->setCurrentIndex(1);
               //ui->numlikes->setText("Likes: "+QString::number(temp.getnumlikes()));
           }
       }
    }
}

void MainWindow::verPubGeneral() {
    ui->stackedWidgetGeneral->setCurrentIndex(1);
    ui->stackedWidgetFollow->setCurrentIndex(0);
    ui->stackedWidgetParcial->setCurrentIndex(0);
    filtradoLikes();
}

void MainWindow::verPubGeneralConFiltradoAnterior() {
    ui->stackedWidgetGeneral->setCurrentIndex(1);
    ui->stackedWidgetFollow->setCurrentIndex(0);
    ui->stackedWidgetParcial->setCurrentIndex(0);
    if (tipoFiltradoOrden == 0) {
        filtradoLikes();
    } else if (tipoFiltradoOrden == 1) {
        filtradoRecientes();
    } else if (tipoFiltradoOrden == 2) {
        filtradoAntiguos();
    } else if (tipoFiltradoOrden == 3) {
        filtradoRelevantes();
    }
    //TODO: SET LIKE BUTTON
}

void MainWindow::on_newComment_returnPressed()
{
    QString newComment = ui->newComment->text();
    if(!newComment.isEmpty()){
        Comment *temporal = new Comment(QDate::currentDate().toString("yyyy-MM-dd"),newComment);
        globalPub.at(idF)->agregar_comments(temporal);
        globalComm.push_back(new Comment(QDate::currentDate().toString("yyyy-MM-dd"),newComment,idF+1));
        ui->newComment->setText("");
        verComentariosGeneral(idF);
    }
}

void MainWindow::QPushButtonChange(QPushButton* p1) {
    p1->setStyleSheet({"background:transparent;"
            "border:none;"
            "border-bottom:1px solid #44548E;"
            "color:#44548E;"
            "font: 11pt Century Gothic;"});
}

void MainWindow::QPushButtonOriginal(QPushButton* p1, QPushButton* p2, QPushButton* p3) {
    p1->setStyleSheet({"background:transparent;"
            "border:none;"
            "border-bottom:1px solid #FFFFFF;"
            "color:#FFFFFF;"
            "font: 11pt Century Gothic;"});
    p2->setStyleSheet({"background:transparent;"
            "border:none;"
            "border-bottom:1px solid #FFFFFF;"
            "color:#FFFFFF;"
            "font: 11pt Century Gothic;"});
    p3->setStyleSheet({"background:transparent;"
            "border:none;"
            "border-bottom:1px solid #FFFFFF;"
            "color:#FFFFFF;"
            "font: 11pt Century Gothic;"});
}

void MainWindow::llenarLabelsButtonsTotal() {
    for(int i = 0; i < 50; ++i) {
        Publication* pub = orden.getPos(i);
        QStringList list = pub->getpubdate().split(QLatin1Char('-'));
        QString date = list[2] + " de ";
        switch(list[1].toInt()) {
            case 1: date += "enero"; break;
            case 2: date += "febrero"; break;
            case 3: date += "marzo"; break;
            case 4: date += "abril"; break;
            case 5: date += "mayo"; break;
            case 6: date += "junio"; break;
            case 7: date += "julio"; break;
            case 8: date += "agosto"; break;
            case 9: date += "septiembre"; break;
            case 10: date += "octubre"; break;
            case 11: date += "noviembre"; break;
            case 12: date += "diciembre"; break;
        }
        date += " de " + list[0];

        vLabelNameDate.at(i)->setText(lstUsuarios.at(pub->getidU()-1)->getFullname() + "\n" + date);
        vLabelNameDate.at(i)->setWordWrap(true);
        vLabelNameDate.at(i)->show();
        vLabelTitleDesc.at(i)->setText(pub->gettitle() + "\n" + pub->getdesc());
        vLabelTitleDesc.at(i)->setWordWrap(true);
        vLabelTitleDesc.at(i)->show();
        vBttnLikes.at(i)->setText("thumbs-up "+ QString::number(pub->getnumlikes()));
        vBttnLikes.at(i)->show();
        vBttnComments.at(i)->setText("comment-alt "+ QString::number(pub->getnum_comments()));
        vBttnComments.at(i)->show();
        vLabelPhoto.at(i)->show();
    }
}

void MainWindow::llenarLabelsButtonsParcial() {
    QMessageBox e;
        for(int i = 0; i < 50; ++i) {
            if(i < ordenTitulo.tam_list()) {
                Publication* pub = ordenTitulo.getPos(i);
                QStringList list = pub->getpubdate().split(QLatin1Char('-'));
                QString date = list[2] + " de ";
                switch(list[1].toInt()) {
                    case 1: date += "enero"; break;
                    case 2: date += "febrero"; break;
                    case 3: date += "marzo"; break;
                    case 4: date += "abril"; break;
                    case 5: date += "mayo"; break;
                    case 6: date += "junio"; break;
                    case 7: date += "julio"; break;
                    case 8: date += "agosto"; break;
                    case 9: date += "septiembre"; break;
                    case 10: date += "octubre"; break;
                    case 11: date += "noviembre"; break;
                    case 12: date += "diciembre"; break;
                }
                date += " de " + list[0];

                vLabelNameDate.at(i)->setText(lstUsuarios.at(pub->getidU()-1)->getFullname() + "\n" + date);
                vLabelNameDate.at(i)->setWordWrap(true);
                vLabelNameDate.at(i)->show();
                vLabelTitleDesc.at(i)->setText(pub->gettitle() + "\n" + pub->getdesc());
                vLabelTitleDesc.at(i)->setWordWrap(true);
                vLabelTitleDesc.at(i)->show();
                vBttnLikes.at(i)->setText("thumbs-up "+ QString::number(pub->getnumlikes()));
                vBttnLikes.at(i)->show();
                vBttnComments.at(i)->setText("comment-alt "+ QString::number(pub->getnum_comments()));
                vBttnComments.at(i)->show();
                vLabelPhoto.at(i)->show();
            } else {
                vLabelNameDate.at(i)->hide();
                vLabelTitleDesc.at(i)->hide();
                vBttnLikes.at(i)->hide();
                vBttnComments.at(i)->hide();
                vLabelPhoto.at(i)->hide();
            }
    }

    for(int i = ordenTitulo.tam_list(); i < 50 - ordenTitulo.tam_list(); ++i) {
        vLabelNameDate.at(i)->hide();
        vLabelTitleDesc.at(i)->hide();
        vBttnLikes.at(i)->hide();
        vBttnComments.at(i)->hide();
        vLabelPhoto.at(i)->hide();
    }
    if(ordenTitulo.tam_list() == 0) {
       ui->lblNingunaPub->show();
    }
}

void MainWindow::filtradoLikesPerfil() {
     ui->mypublications->clear();
    tipoFiltradoGeneral = 0;
    tipoFiltradoOrden = 0;
    QPushButtonOriginal(ui->orderRecientesProfile, ui->orderAntiguosProfile, ui->orderRelevanteProfile);
    QPushButtonChange(ui->orderLikesProfile);
    QString publi;
    ui->lblNingunaPub->hide();
    List<Publication*> lstPub;


    avlGlobalLikesPerfil->clearI();
    avlGlobalLikesPerfil->clear();

    for(int i =0;i<ordenPerfil.tam_list();i++){
        avlGlobalLikesPerfil->add(ordenPerfil.getPos(i));
    }
    auto b = [&](Publication* a) { lstPub.agregar(a); };
    avlGlobalLikesPerfil->postOrder(b);
    ordenPerfil2 = lstPub;

    for(int i =0;i<ordenPerfil2.tam_list();i++){
        publi = "Titulo: " + ordenPerfil2.getPos(i)->gettitle() + "\nFecha: " + ordenPerfil2.getPos(i)->getpubdate() + "\n" + ordenPerfil2.getPos(i)->getdesc();
        ui->mypublications->addItem(publi);
    }

}

void MainWindow::filtradoRecientesPerfil() {

    ui->mypublications->clear();
    tipoFiltradoGeneral = 0;
    tipoFiltradoOrden = 1;
    QPushButtonOriginal(ui->orderLikesProfile, ui->orderAntiguosProfile, ui->orderRelevanteProfile);
    QPushButtonChange(ui->orderRecientesProfile);

    ui->lblNingunaPub->hide();
    List<Publication*> lstPub;
    QString publi;
    avlGlobalFechaPerfil->clearI();
    avlGlobalFechaPerfil->clear();

    for(int i =0;i<ordenPerfil.tam_list();i++){
        avlGlobalFechaPerfil->add(ordenPerfil.getPos(i));
    }
    auto b = [&](Publication* a) { lstPub.agregar(a); };
    avlGlobalFechaPerfil->postOrder(b);
    ordenPerfil2 = lstPub;

    for(int i =0;i<ordenPerfil2.tam_list();i++){
        publi = "Titulo: " + ordenPerfil2.getPos(i)->gettitle() + "\nFecha: " + ordenPerfil2.getPos(i)->getpubdate() + "\n" + ordenPerfil2.getPos(i)->getdesc();
        ui->mypublications->addItem(publi);
    }


}

void MainWindow::filtradoAntiguosPerfil() {

    ui->mypublications->clear();
    tipoFiltradoGeneral = 0;
    tipoFiltradoOrden = 2;
    QPushButtonOriginal(ui->orderLikesProfile, ui->orderRecientesProfile, ui->orderRelevanteProfile);
    QPushButtonChange(ui->orderAntiguosProfile);

    ui->lblNingunaPub->hide();
    List<Publication*> lstPub;
    QString publi;
    avlGlobalFechaPerfil->clearI();
    avlGlobalFechaPerfil->clear();

    for(int i =0;i<ordenPerfil.tam_list();i++){
        avlGlobalFechaPerfil->add(ordenPerfil.getPos(i));
    }
    auto b = [&](Publication* a) { lstPub.agregar(a); };
    avlGlobalFechaPerfil->inOrder(b);
    ordenPerfil2 = lstPub;

    for(int i =0;i<ordenPerfil2.tam_list();i++){
        publi = "Titulo: " + ordenPerfil2.getPos(i)->gettitle() + "\nFecha: " + ordenPerfil2.getPos(i)->getpubdate() + "\n" + ordenPerfil2.getPos(i)->getdesc();
        ui->mypublications->addItem(publi);
    }

}

void MainWindow::filtradoRelevantesPerfil() {

    ui->mypublications->clear();
    tipoFiltradoGeneral = 0;
    tipoFiltradoOrden = 3;
    QPushButtonOriginal(ui->orderLikesProfile, ui->orderAntiguosProfile, ui->orderRecientesProfile);
    QPushButtonChange(ui->orderRelevanteProfile);

    ui->lblNingunaPub->hide();
    List<Publication*> lstPub;
    QString publi;
    avlGlobalRelevantesPerfil->clearI();
    avlGlobalRelevantesPerfil->clear();

    for(int i =0;i<ordenPerfil.tam_list();i++){
        avlGlobalRelevantesPerfil->add(ordenPerfil.getPos(i));
    }

    auto b = [&](Publication* a) { lstPub.agregar(a); };
    avlGlobalRelevantesPerfil->postOrder(b);
    ordenPerfil2 = lstPub;

    for(int i =0;i<ordenPerfil2.tam_list();i++){
        publi = "Titulo: " + ordenPerfil2.getPos(i)->gettitle() + "\nFecha: " + ordenPerfil2.getPos(i)->getpubdate() + "\n" + ordenPerfil2.getPos(i)->getdesc();
        ui->mypublications->addItem(publi);
    }

}

//FILTRADO GENERAL LIKES
void MainWindow::filtradoLikes() {
    tipoFiltradoGeneral = 0;
    tipoFiltradoOrden = 0;
    QPushButtonOriginal(ui->orderRecientes, ui->orderAntiguos, ui->orderRelevante);
    QPushButtonChange(ui->orderLikes);

    ui->lblNingunaPub->hide();
    List<Publication*> lstPub;

    avlGlobalLikes->clearI();
    auto b = [&](Publication* a) { lstPub.agregar(a); };
    avlGlobalLikes->postOrder(b);
    orden = lstPub;

    llenarLabelsButtonsTotal();
}

//FILTRADO GENERAL RECIENTES
void MainWindow::filtradoRecientes() {
    tipoFiltradoGeneral = 0;
    tipoFiltradoOrden = 1;
    QPushButtonOriginal(ui->orderLikes, ui->orderAntiguos, ui->orderRelevante);
    QPushButtonChange(ui->orderRecientes);

    ui->lblNingunaPub->hide();
    List<Publication*> lstPub;

    avlGlobalFecha->clearI();
    auto b = [&](Publication* a) { lstPub.agregar(a); };
    avlGlobalFecha->postOrder(b);
    orden = lstPub;

    llenarLabelsButtonsTotal();
}

//FILTRADO GENERAL ANTIGUOS
void MainWindow::filtradoAntiguos() {
    tipoFiltradoGeneral = 0;
    tipoFiltradoOrden = 2;
    QPushButtonOriginal(ui->orderLikes, ui->orderRecientes, ui->orderRelevante);
    QPushButtonChange(ui->orderAntiguos);

    ui->lblNingunaPub->hide();
    List<Publication*> lstPub;

    avlGlobalFecha->clearI();
    auto b = [&](Publication* a) { lstPub.agregar(a); };
    avlGlobalFecha->inOrder(b);
    orden = lstPub;

    llenarLabelsButtonsTotal();;
}

//FILTRADO GENERAL RELEVANTES
void MainWindow::filtradoRelevantes() {
    tipoFiltradoGeneral = 0;
    tipoFiltradoOrden = 3;
    QPushButtonOriginal(ui->orderLikes, ui->orderAntiguos, ui->orderRecientes);
    QPushButtonChange(ui->orderRelevante);

    ui->lblNingunaPub->hide();
    List<Publication*> lstPub;

    avlGlobalRelevantes->clearI();
    auto b = [&](Publication* a) { lstPub.agregar(a); };
    avlGlobalRelevantes->postOrder(b);
    orden = lstPub;

    llenarLabelsButtonsTotal();
}

//FILTRADO POR TITULO
void MainWindow::filtro(){
    ui->lblNingunaPub->hide();
    tipoFiltradoGeneral = 1;
    if(ui->buscpub->text() != ""){
        List<Publication*> aux;
        QString encontrar = ui->buscpub->text();

        if(ui->filtros->currentIndex() == 0) { //IGUAL A
            for(int i=0; i < orden.tam_list(); ++i) {
                if(encontrar == orden.getPos(i)->gettitle()) {
                    aux.agregar(orden.getPos(i));
                }
            }
        } else if(ui->filtros->currentIndex() == 1) { //INICIA CON
            for(int i=0; i < orden.tam_list(); ++i) {
                if(orden.getPos(i)->gettitle().startsWith(encontrar)) {
                    aux.agregar(orden.getPos(i));
                }
            }
        } else if(ui->filtros->currentIndex() == 2){ //FINALIZA CON
            for(int i=0; i < orden.tam_list(); ++i) {
                if(orden.getPos(i)->gettitle().endsWith(encontrar)) {
                    aux.agregar(orden.getPos(i));
                }
            }
        } else if(ui->filtros->currentIndex() == 3){ // ESTÁ CONTENIDO EN
            for(int i=0; i < orden.tam_list(); ++i) {
                if(orden.getPos(i)->gettitle().contains(encontrar)) {
                    aux.agregar(orden.getPos(i));
                }
            }
        } else if(ui->filtros->currentIndex() == 4){ // NO ESTÁ CONTENIDO EN
            for(int i=0; i < orden.tam_list(); ++i) {
                if(!orden.getPos(i)->gettitle().contains(encontrar)) {
                    aux.agregar(orden.getPos(i));
                }
            }
        }
        ordenTitulo = aux;
        llenarLabelsButtonsParcial();

    }
}

void MainWindow::verFriend(){

    if(ui->follows->count() != 0 && ui->follows->selectedItems().size() != 0){
        ui->mypublications->setGeometry(260,90,761,461);
        ui->verInformacion->setGeometry(548,565,191,31);
        List<Publication*>aux;
        ui->pubTittleBox->hide();
        ui->cajapublic->hide();
        ui->addpublication->hide();
         ui->stackedWidgetGeneral->setCurrentIndex(2);
         ui->mypublications->clear();
         ui->follow->hide();
         ui->heart->show();
       QString friendNick = ui->follows->currentItem()->text();
       for (int i=0;i<lstUsuarios.size();i++){
           if (lstUsuarios.at(i)->getFullname()==friendNick)
             perfriend=i;
        }
       ui->follow->hide();
       ui->pName->setText(lstUsuarios.at(perfriend)->getFullname());
       ui->pEmail->setText(lstUsuarios.at(perfriend)->getMail());
       QStringList list = lstUsuarios.at(perfriend)->getRegisterDate().split(QLatin1Char('-'));
       QString date = list[2] + " de ";
       switch(list[1].toInt()) {
       case 1: date += "enero"; break;
       case 2: date += "febrero"; break;
       case 3: date += "marzo"; break;
       case 4: date += "abril"; break;
       case 5: date += "mayo"; break;
       case 6: date += "junio"; break;
       case 7: date += "julio"; break;
       case 8: date += "agosto"; break;
       case 9: date += "septiembre"; break;
       case 10: date += "octubre"; break;
       case 11: date += "noviembre"; break;
       case 12: date += "diciembre"; break;
       }
       date += " de " + list[0];
       ui->pDate->setText(date);
       ui->pFollow->setText(QString::number(lstUsuarios.at(perfriend)->numfollower()));
       int pubs = 0;
       QString publi;
       for(int i=0;i<globalPub.size();i++){
           if(globalPub.at(i)->getidU() == perfriend + 1){
               pubs++;
               publi = "Titulo: " + globalPub.at(i)->gettitle() + "\nFecha: " + globalPub.at(i)->getpubdate() + "\n" + globalPub.at(i)->getdesc();
               ui->mypublications->addItem(publi);
               aux.agregar(globalPub.at(i));
           }   
       }
        ordenPerfil = aux;
       ui->pPub->setText(QString::number(pubs));
       ui->fError->hide();
    }else{
        ui->fError->show();
        ui->fError->setText("SELECCIONE UN USUARIO");
    }

}

void MainWindow::volverFollows(){
    ui->stackedWidgetFollow->setCurrentIndex(0);
    ui->search->setText("");
    ui->people->clear();
    ui->bError_2->hide();
}

void MainWindow::verFollw2(){
    if(ui->people->count() != 0 && ui->people->selectedItems().size() != 0){

        ui->mypublications->setGeometry(260,90,761,461);
        ui->verInformacion->setGeometry(548,565,191,31);
        ui->mypublications->clear();
        List<Publication*>aux;
        ui->pubTittleBox->hide();
        ui->cajapublic->hide();
        ui->addpublication->hide();
        ui->follow->show();
        ui->heart->hide();
         ui->stackedWidgetGeneral->setCurrentIndex(2);
       QString correito = ui->people->currentItem()->text();
       ui->people->clear();
       for (int i=0;i<lstUsuarios.size();i++){
           if (lstUsuarios.at(i)->getMail()==correito)
             perfriend=i;
        }
       ui->pName->setText(lstUsuarios.at(perfriend)->getFullname());
       ui->pEmail->setText(lstUsuarios.at(perfriend)->getMail());
       QStringList list = lstUsuarios.at(perfriend)->getRegisterDate().split(QLatin1Char('-'));
       QString date = list[2] + " de ";
       switch(list[1].toInt()) {
       case 1: date += "enero"; break;
       case 2: date += "febrero"; break;
       case 3: date += "marzo"; break;
       case 4: date += "abril"; break;
       case 5: date += "mayo"; break;
       case 6: date += "junio"; break;
       case 7: date += "julio"; break;
       case 8: date += "agosto"; break;
       case 9: date += "septiembre"; break;
       case 10: date += "octubre"; break;
       case 11: date += "noviembre"; break;
       case 12: date += "diciembre"; break;
       }
       date += " de " + list[0];
       ui->pDate->setText(date);
       ui->pFollow->setText(QString::number(lstUsuarios.at(perfriend)->numfollower()));
       int pubs = 0;
       QString publi;
       for(int i=0;i<globalPub.size();i++){
           if(globalPub.at(i)->getidU() == perfriend + 1){
               pubs++;
               publi = "Titulo: " + globalPub.at(i)->gettitle() + "\nFecha: " + globalPub.at(i)->getpubdate() + "\n" + globalPub.at(i)->getdesc();
               ui->mypublications->addItem(publi);
               aux.agregar(globalPub.at(i));
           }
       }
       ordenPerfil = aux;
       ui->pPub->setText(QString::number(pubs));
       ui->bError_2->hide();
    }else{
        ui->bError_2->show();
        ui->bError_2->setText("SELECCIONE UN USUARIO");
    }
}

void MainWindow::verComentarios(int idP) {
    Publication* pub;
    if (tipoFiltradoGeneral == 0) {
        pub = orden.getPos(idP);
    } else if (tipoFiltradoGeneral == 1) {
        pub = ordenTitulo.getPos(idP);
    }

    ui->regresarProfile->hide();
    ui->regresar->show();
    idF = pub->getId();
    ui->nameOnePub->setText(lstUsuarios.at(pub->getidU()-1)->getFullname());
    QStringList list = pub->getpubdate().split(QLatin1Char('-'));
    QString date = list[2] + " de ";
    switch(list[1].toInt()) {
        case 1: date += "enero"; break;
        case 2: date += "febrero"; break;
        case 3: date += "marzo"; break;
        case 4: date += "abril"; break;
        case 5: date += "mayo"; break;
        case 6: date += "junio"; break;
        case 7: date += "julio"; break;
        case 8: date += "agosto"; break;
        case 9: date += "septiembre"; break;
        case 10: date += "octubre"; break;
        case 11: date += "noviembre"; break;
        case 12: date += "diciembre"; break;
    }
    date += " de " + list[0];
    ui->dateOnePub->setText(date);
    ui->onePub->setText(pub->gettitle() + "\n" + pub->getdesc());
    ui->onePub->setWordWrap(true);
    ui->bttnLikeOnePub->setText("thumbs-up "+ QString::number(pub->getnumlikes()));
    ui->bttnCommentOnePub->setText("comment-alt "+ QString::number(pub->getnum_comments()));

    QLayoutItem* child;
    while((child = layout->takeAt(0))) {
        delete child->widget();
        delete child;
    }

    AvlTree<Comment*,QString>* avlFechaCommentario = new AvlTree<Comment*,QString>([](Comment* c){return c->get_date();});
    for(Comment* c : pub->getAllComentarios()) {
        avlFechaCommentario->add(c);
    }
    auto b = [&](Comment* c) {
        QLabel* label = new QLabel();
        label->setStyleSheet({"border: 1.1px solid #222A48;"
                              "border-color: rgb(255, 255, 255);"
                              "color:#FFFFFF;"
                              "font: 10pt Century Gothic;"});

        layout->addWidget(label);
        label->setText(c->get_text());
        label->setWordWrap(true);
    };
    avlFechaCommentario->postOrderTodo(b);
    ui->scrollPubComments->setLayout(layout);
}

void MainWindow::verComentariosGeneral(int idF) {
    Publication* pub;
    if (tipoFiltradoGeneral == 0) {
        pub = globalPub.at(idF);
    } else if (tipoFiltradoGeneral == 1) {
        pub = globalPub.at(idF);
    }
    idF = pub->getId();
    ui->nameOnePub->setText(lstUsuarios.at(pub->getidU()-1)->getFullname());
    QStringList list = pub->getpubdate().split(QLatin1Char('-'));
    QString date = list[2] + " de ";
    switch(list[1].toInt()) {
        case 1: date += "enero"; break;
        case 2: date += "febrero"; break;
        case 3: date += "marzo"; break;
        case 4: date += "abril"; break;
        case 5: date += "mayo"; break;
        case 6: date += "junio"; break;
        case 7: date += "julio"; break;
        case 8: date += "agosto"; break;
        case 9: date += "septiembre"; break;
        case 10: date += "octubre"; break;
        case 11: date += "noviembre"; break;
        case 12: date += "diciembre"; break;
    }
    date += " de " + list[0];
    ui->dateOnePub->setText(date);
    ui->onePub->setText(pub->gettitle() + "\n" + pub->getdesc());
    ui->onePub->setWordWrap(true);
    ui->bttnLikeOnePub->setText("thumbs-up "+ QString::number(pub->getnumlikes()));
    ui->bttnCommentOnePub->setText("comment-alt "+ QString::number(pub->getnum_comments()));

    QLayoutItem* child;
    while((child = layout->takeAt(0))) {
        delete child->widget();
        delete child;
    }

    AvlTree<Comment*,QString>* avlFechaCommentario = new AvlTree<Comment*,QString>([](Comment* c){return c->get_date();});
    for(Comment* c : pub->getAllComentarios()) {
        avlFechaCommentario->add(c);
    }
    auto b = [&](Comment* c) {
        QLabel* label = new QLabel();
        label->setStyleSheet({"border: 1.1px solid #222A48;"
                              "border-color: rgb(255, 255, 255);"
                              "color:#FFFFFF;"
                              "font: 10pt Century Gothic;"});

        layout->addWidget(label);
        label->setText(c->get_text());
        label->setWordWrap(true);
    };
    avlFechaCommentario->postOrderTodo(b);
    ui->scrollPubComments->setLayout(layout);
}

void MainWindow::regresarPerfil() {
    ui->stackedWidgetGeneral->setCurrentIndex(2);
}

void MainWindow::seguir(){

    lstUsuarios.at(idU)->agregar_amigo(perfriend+1);
    ui->follows->addItem(lstUsuarios.at(perfriend)->getFullname());
    ui->heart->show();
    ui->follow->hide();

}

void MainWindow::agregarWidgets() {
    vLabelPhoto.append(ui->lblPhoto_1);
    vLabelPhoto.append(ui->lblPhoto_2);
    vLabelPhoto.append(ui->lblPhoto_3);
    vLabelPhoto.append(ui->lblPhoto_4);
    vLabelPhoto.append(ui->lblPhoto_5);
    vLabelPhoto.append(ui->lblPhoto_6);
    vLabelPhoto.append(ui->lblPhoto_7);
    vLabelPhoto.append(ui->lblPhoto_8);
    vLabelPhoto.append(ui->lblPhoto_9);
    vLabelPhoto.append(ui->lblPhoto_10);
    vLabelPhoto.append(ui->lblPhoto_11);
    vLabelPhoto.append(ui->lblPhoto_12);
    vLabelPhoto.append(ui->lblPhoto_13);
    vLabelPhoto.append(ui->lblPhoto_14);
    vLabelPhoto.append(ui->lblPhoto_15);
    vLabelPhoto.append(ui->lblPhoto_16);
    vLabelPhoto.append(ui->lblPhoto_17);
    vLabelPhoto.append(ui->lblPhoto_18);
    vLabelPhoto.append(ui->lblPhoto_19);
    vLabelPhoto.append(ui->lblPhoto_20);
    vLabelPhoto.append(ui->lblPhoto_21);
    vLabelPhoto.append(ui->lblPhoto_22);
    vLabelPhoto.append(ui->lblPhoto_23);
    vLabelPhoto.append(ui->lblPhoto_24);
    vLabelPhoto.append(ui->lblPhoto_25);
    vLabelPhoto.append(ui->lblPhoto_26);
    vLabelPhoto.append(ui->lblPhoto_27);
    vLabelPhoto.append(ui->lblPhoto_28);
    vLabelPhoto.append(ui->lblPhoto_29);
    vLabelPhoto.append(ui->lblPhoto_30);
    vLabelPhoto.append(ui->lblPhoto_31);
    vLabelPhoto.append(ui->lblPhoto_32);
    vLabelPhoto.append(ui->lblPhoto_33);
    vLabelPhoto.append(ui->lblPhoto_34);
    vLabelPhoto.append(ui->lblPhoto_35);
    vLabelPhoto.append(ui->lblPhoto_36);
    vLabelPhoto.append(ui->lblPhoto_37);
    vLabelPhoto.append(ui->lblPhoto_38);
    vLabelPhoto.append(ui->lblPhoto_39);
    vLabelPhoto.append(ui->lblPhoto_40);
    vLabelPhoto.append(ui->lblPhoto_41);
    vLabelPhoto.append(ui->lblPhoto_42);
    vLabelPhoto.append(ui->lblPhoto_43);
    vLabelPhoto.append(ui->lblPhoto_44);
    vLabelPhoto.append(ui->lblPhoto_45);
    vLabelPhoto.append(ui->lblPhoto_46);
    vLabelPhoto.append(ui->lblPhoto_47);
    vLabelPhoto.append(ui->lblPhoto_48);
    vLabelPhoto.append(ui->lblPhoto_49);
    vLabelPhoto.append(ui->lblPhoto_50);

    vLabelNameDate.append(ui->lblNameDate_1);
    vLabelNameDate.append(ui->lblNameDate_2);
    vLabelNameDate.append(ui->lblNameDate_3);
    vLabelNameDate.append(ui->lblNameDate_4);
    vLabelNameDate.append(ui->lblNameDate_5);
    vLabelNameDate.append(ui->lblNameDate_6);
    vLabelNameDate.append(ui->lblNameDate_7);
    vLabelNameDate.append(ui->lblNameDate_8);
    vLabelNameDate.append(ui->lblNameDate_9);
    vLabelNameDate.append(ui->lblNameDate_10);
    vLabelNameDate.append(ui->lblNameDate_11);
    vLabelNameDate.append(ui->lblNameDate_12);
    vLabelNameDate.append(ui->lblNameDate_13);
    vLabelNameDate.append(ui->lblNameDate_14);
    vLabelNameDate.append(ui->lblNameDate_15);
    vLabelNameDate.append(ui->lblNameDate_16);
    vLabelNameDate.append(ui->lblNameDate_17);
    vLabelNameDate.append(ui->lblNameDate_18);
    vLabelNameDate.append(ui->lblNameDate_19);
    vLabelNameDate.append(ui->lblNameDate_20);
    vLabelNameDate.append(ui->lblNameDate_21);
    vLabelNameDate.append(ui->lblNameDate_22);
    vLabelNameDate.append(ui->lblNameDate_23);
    vLabelNameDate.append(ui->lblNameDate_24);
    vLabelNameDate.append(ui->lblNameDate_25);
    vLabelNameDate.append(ui->lblNameDate_26);
    vLabelNameDate.append(ui->lblNameDate_27);
    vLabelNameDate.append(ui->lblNameDate_28);
    vLabelNameDate.append(ui->lblNameDate_29);
    vLabelNameDate.append(ui->lblNameDate_30);
    vLabelNameDate.append(ui->lblNameDate_31);
    vLabelNameDate.append(ui->lblNameDate_32);
    vLabelNameDate.append(ui->lblNameDate_33);
    vLabelNameDate.append(ui->lblNameDate_34);
    vLabelNameDate.append(ui->lblNameDate_35);
    vLabelNameDate.append(ui->lblNameDate_36);
    vLabelNameDate.append(ui->lblNameDate_37);
    vLabelNameDate.append(ui->lblNameDate_38);
    vLabelNameDate.append(ui->lblNameDate_39);
    vLabelNameDate.append(ui->lblNameDate_40);
    vLabelNameDate.append(ui->lblNameDate_41);
    vLabelNameDate.append(ui->lblNameDate_42);
    vLabelNameDate.append(ui->lblNameDate_43);
    vLabelNameDate.append(ui->lblNameDate_44);
    vLabelNameDate.append(ui->lblNameDate_45);
    vLabelNameDate.append(ui->lblNameDate_46);
    vLabelNameDate.append(ui->lblNameDate_47);
    vLabelNameDate.append(ui->lblNameDate_48);
    vLabelNameDate.append(ui->lblNameDate_49);
    vLabelNameDate.append(ui->lblNameDate_50);

    vLabelTitleDesc.append(ui->lblTitleDesc_1);
    vLabelTitleDesc.append(ui->lblTitleDesc_2);
    vLabelTitleDesc.append(ui->lblTitleDesc_3);
    vLabelTitleDesc.append(ui->lblTitleDesc_4);
    vLabelTitleDesc.append(ui->lblTitleDesc_5);
    vLabelTitleDesc.append(ui->lblTitleDesc_6);
    vLabelTitleDesc.append(ui->lblTitleDesc_7);
    vLabelTitleDesc.append(ui->lblTitleDesc_8);
    vLabelTitleDesc.append(ui->lblTitleDesc_9);
    vLabelTitleDesc.append(ui->lblTitleDesc_10);
    vLabelTitleDesc.append(ui->lblTitleDesc_11);
    vLabelTitleDesc.append(ui->lblTitleDesc_12);
    vLabelTitleDesc.append(ui->lblTitleDesc_13);
    vLabelTitleDesc.append(ui->lblTitleDesc_14);
    vLabelTitleDesc.append(ui->lblTitleDesc_15);
    vLabelTitleDesc.append(ui->lblTitleDesc_16);
    vLabelTitleDesc.append(ui->lblTitleDesc_17);
    vLabelTitleDesc.append(ui->lblTitleDesc_18);
    vLabelTitleDesc.append(ui->lblTitleDesc_19);
    vLabelTitleDesc.append(ui->lblTitleDesc_20);
    vLabelTitleDesc.append(ui->lblTitleDesc_21);
    vLabelTitleDesc.append(ui->lblTitleDesc_22);
    vLabelTitleDesc.append(ui->lblTitleDesc_23);
    vLabelTitleDesc.append(ui->lblTitleDesc_24);
    vLabelTitleDesc.append(ui->lblTitleDesc_25);
    vLabelTitleDesc.append(ui->lblTitleDesc_26);
    vLabelTitleDesc.append(ui->lblTitleDesc_27);
    vLabelTitleDesc.append(ui->lblTitleDesc_28);
    vLabelTitleDesc.append(ui->lblTitleDesc_29);
    vLabelTitleDesc.append(ui->lblTitleDesc_30);
    vLabelTitleDesc.append(ui->lblTitleDesc_31);
    vLabelTitleDesc.append(ui->lblTitleDesc_32);
    vLabelTitleDesc.append(ui->lblTitleDesc_33);
    vLabelTitleDesc.append(ui->lblTitleDesc_34);
    vLabelTitleDesc.append(ui->lblTitleDesc_35);
    vLabelTitleDesc.append(ui->lblTitleDesc_36);
    vLabelTitleDesc.append(ui->lblTitleDesc_37);
    vLabelTitleDesc.append(ui->lblTitleDesc_38);
    vLabelTitleDesc.append(ui->lblTitleDesc_39);
    vLabelTitleDesc.append(ui->lblTitleDesc_40);
    vLabelTitleDesc.append(ui->lblTitleDesc_41);
    vLabelTitleDesc.append(ui->lblTitleDesc_42);
    vLabelTitleDesc.append(ui->lblTitleDesc_43);
    vLabelTitleDesc.append(ui->lblTitleDesc_44);
    vLabelTitleDesc.append(ui->lblTitleDesc_45);
    vLabelTitleDesc.append(ui->lblTitleDesc_46);
    vLabelTitleDesc.append(ui->lblTitleDesc_47);
    vLabelTitleDesc.append(ui->lblTitleDesc_48);
    vLabelTitleDesc.append(ui->lblTitleDesc_49);
    vLabelTitleDesc.append(ui->lblTitleDesc_50);

    vBttnLikes.append(ui->bttnLike_1);
    vBttnLikes.append(ui->bttnLike_2);
    vBttnLikes.append(ui->bttnLike_3);
    vBttnLikes.append(ui->bttnLike_4);
    vBttnLikes.append(ui->bttnLike_5);
    vBttnLikes.append(ui->bttnLike_6);
    vBttnLikes.append(ui->bttnLike_7);
    vBttnLikes.append(ui->bttnLike_8);
    vBttnLikes.append(ui->bttnLike_9);
    vBttnLikes.append(ui->bttnLike_10);
    vBttnLikes.append(ui->bttnLike_11);
    vBttnLikes.append(ui->bttnLike_12);
    vBttnLikes.append(ui->bttnLike_13);
    vBttnLikes.append(ui->bttnLike_14);
    vBttnLikes.append(ui->bttnLike_15);
    vBttnLikes.append(ui->bttnLike_16);
    vBttnLikes.append(ui->bttnLike_17);
    vBttnLikes.append(ui->bttnLike_18);
    vBttnLikes.append(ui->bttnLike_19);
    vBttnLikes.append(ui->bttnLike_20);
    vBttnLikes.append(ui->bttnLike_21);
    vBttnLikes.append(ui->bttnLike_22);
    vBttnLikes.append(ui->bttnLike_23);
    vBttnLikes.append(ui->bttnLike_24);
    vBttnLikes.append(ui->bttnLike_25);
    vBttnLikes.append(ui->bttnLike_26);
    vBttnLikes.append(ui->bttnLike_27);
    vBttnLikes.append(ui->bttnLike_28);
    vBttnLikes.append(ui->bttnLike_29);
    vBttnLikes.append(ui->bttnLike_30);
    vBttnLikes.append(ui->bttnLike_31);
    vBttnLikes.append(ui->bttnLike_32);
    vBttnLikes.append(ui->bttnLike_33);
    vBttnLikes.append(ui->bttnLike_34);
    vBttnLikes.append(ui->bttnLike_35);
    vBttnLikes.append(ui->bttnLike_36);
    vBttnLikes.append(ui->bttnLike_37);
    vBttnLikes.append(ui->bttnLike_38);
    vBttnLikes.append(ui->bttnLike_39);
    vBttnLikes.append(ui->bttnLike_40);
    vBttnLikes.append(ui->bttnLike_41);
    vBttnLikes.append(ui->bttnLike_42);
    vBttnLikes.append(ui->bttnLike_43);
    vBttnLikes.append(ui->bttnLike_44);
    vBttnLikes.append(ui->bttnLike_45);
    vBttnLikes.append(ui->bttnLike_46);
    vBttnLikes.append(ui->bttnLike_47);
    vBttnLikes.append(ui->bttnLike_48);
    vBttnLikes.append(ui->bttnLike_49);
    vBttnLikes.append(ui->bttnLike_50);

    vBttnComments.append(ui->bttnComment_1 );
    vBttnComments.append(ui->bttnComment_2 );
    vBttnComments.append(ui->bttnComment_3 );
    vBttnComments.append(ui->bttnComment_4 );
    vBttnComments.append(ui->bttnComment_5 );
    vBttnComments.append(ui->bttnComment_6 );
    vBttnComments.append(ui->bttnComment_7 );
    vBttnComments.append(ui->bttnComment_8 );
    vBttnComments.append(ui->bttnComment_9 );
    vBttnComments.append(ui->bttnComment_10);
    vBttnComments.append(ui->bttnComment_11);
    vBttnComments.append(ui->bttnComment_12);
    vBttnComments.append(ui->bttnComment_13);
    vBttnComments.append(ui->bttnComment_14);
    vBttnComments.append(ui->bttnComment_15);
    vBttnComments.append(ui->bttnComment_16);
    vBttnComments.append(ui->bttnComment_17);
    vBttnComments.append(ui->bttnComment_18);
    vBttnComments.append(ui->bttnComment_19);
    vBttnComments.append(ui->bttnComment_20);
    vBttnComments.append(ui->bttnComment_21);
    vBttnComments.append(ui->bttnComment_22);
    vBttnComments.append(ui->bttnComment_23);
    vBttnComments.append(ui->bttnComment_24);
    vBttnComments.append(ui->bttnComment_25);
    vBttnComments.append(ui->bttnComment_26);
    vBttnComments.append(ui->bttnComment_27);
    vBttnComments.append(ui->bttnComment_28);
    vBttnComments.append(ui->bttnComment_29);
    vBttnComments.append(ui->bttnComment_30);
    vBttnComments.append(ui->bttnComment_31);
    vBttnComments.append(ui->bttnComment_32);
    vBttnComments.append(ui->bttnComment_33);
    vBttnComments.append(ui->bttnComment_34);
    vBttnComments.append(ui->bttnComment_35);
    vBttnComments.append(ui->bttnComment_36);
    vBttnComments.append(ui->bttnComment_37);
    vBttnComments.append(ui->bttnComment_38);
    vBttnComments.append(ui->bttnComment_39);
    vBttnComments.append(ui->bttnComment_40);
    vBttnComments.append(ui->bttnComment_41);
    vBttnComments.append(ui->bttnComment_42);
    vBttnComments.append(ui->bttnComment_43);
    vBttnComments.append(ui->bttnComment_44);
    vBttnComments.append(ui->bttnComment_45);
    vBttnComments.append(ui->bttnComment_46);
    vBttnComments.append(ui->bttnComment_47);
    vBttnComments.append(ui->bttnComment_48);
    vBttnComments.append(ui->bttnComment_49);
    vBttnComments.append(ui->bttnComment_50);
}

void MainWindow::agregarConnects() {
    connect(ui->bttnComment_1,SIGNAL(released()),this,SLOT(bttnComment_1()));
    connect(ui->bttnComment_2,SIGNAL(released()),this,SLOT(bttnComment_2()));
    connect(ui->bttnComment_3,SIGNAL(released()),this,SLOT(bttnComment_3()));
    connect(ui->bttnComment_4,SIGNAL(released()),this,SLOT(bttnComment_4()));
    connect(ui->bttnComment_5,SIGNAL(released()),this,SLOT(bttnComment_5()));
    connect(ui->bttnComment_6,SIGNAL(released()),this,SLOT(bttnComment_6()));
    connect(ui->bttnComment_7,SIGNAL(released()),this,SLOT(bttnComment_7()));
    connect(ui->bttnComment_8,SIGNAL(released()),this,SLOT(bttnComment_8()));
    connect(ui->bttnComment_9,SIGNAL(released()),this,SLOT(bttnComment_9()));
    connect(ui->bttnComment_10,SIGNAL(released()),this,SLOT(bttnComment_10()));
    connect(ui->bttnComment_11,SIGNAL(released()),this,SLOT(bttnComment_11()));
    connect(ui->bttnComment_12,SIGNAL(released()),this,SLOT(bttnComment_12()));
    connect(ui->bttnComment_13,SIGNAL(released()),this,SLOT(bttnComment_13()));
    connect(ui->bttnComment_14,SIGNAL(released()),this,SLOT(bttnComment_14()));
    connect(ui->bttnComment_15,SIGNAL(released()),this,SLOT(bttnComment_15()));
    connect(ui->bttnComment_16,SIGNAL(released()),this,SLOT(bttnComment_16()));
    connect(ui->bttnComment_17,SIGNAL(released()),this,SLOT(bttnComment_17()));
    connect(ui->bttnComment_18,SIGNAL(released()),this,SLOT(bttnComment_18()));
    connect(ui->bttnComment_19,SIGNAL(released()),this,SLOT(bttnComment_19()));
    connect(ui->bttnComment_20,SIGNAL(released()),this,SLOT(bttnComment_20()));
    connect(ui->bttnComment_21,SIGNAL(released()),this,SLOT(bttnComment_21()));
    connect(ui->bttnComment_22,SIGNAL(released()),this,SLOT(bttnComment_22()));
    connect(ui->bttnComment_23,SIGNAL(released()),this,SLOT(bttnComment_23()));
    connect(ui->bttnComment_24,SIGNAL(released()),this,SLOT(bttnComment_24()));
    connect(ui->bttnComment_25,SIGNAL(released()),this,SLOT(bttnComment_25()));
    connect(ui->bttnComment_26,SIGNAL(released()),this,SLOT(bttnComment_26()));
    connect(ui->bttnComment_27,SIGNAL(released()),this,SLOT(bttnComment_27()));
    connect(ui->bttnComment_28,SIGNAL(released()),this,SLOT(bttnComment_28()));
    connect(ui->bttnComment_29,SIGNAL(released()),this,SLOT(bttnComment_29()));
    connect(ui->bttnComment_30,SIGNAL(released()),this,SLOT(bttnComment_30()));
    connect(ui->bttnComment_31,SIGNAL(released()),this,SLOT(bttnComment_31()));
    connect(ui->bttnComment_32,SIGNAL(released()),this,SLOT(bttnComment_32()));
    connect(ui->bttnComment_33,SIGNAL(released()),this,SLOT(bttnComment_33()));
    connect(ui->bttnComment_34,SIGNAL(released()),this,SLOT(bttnComment_34()));
    connect(ui->bttnComment_35,SIGNAL(released()),this,SLOT(bttnComment_35()));
    connect(ui->bttnComment_36,SIGNAL(released()),this,SLOT(bttnComment_36()));
    connect(ui->bttnComment_37,SIGNAL(released()),this,SLOT(bttnComment_37()));
    connect(ui->bttnComment_38,SIGNAL(released()),this,SLOT(bttnComment_38()));
    connect(ui->bttnComment_39,SIGNAL(released()),this,SLOT(bttnComment_39()));
    connect(ui->bttnComment_40,SIGNAL(released()),this,SLOT(bttnComment_40()));
    connect(ui->bttnComment_41,SIGNAL(released()),this,SLOT(bttnComment_41()));
    connect(ui->bttnComment_42,SIGNAL(released()),this,SLOT(bttnComment_42()));
    connect(ui->bttnComment_43,SIGNAL(released()),this,SLOT(bttnComment_43()));
    connect(ui->bttnComment_44,SIGNAL(released()),this,SLOT(bttnComment_44()));
    connect(ui->bttnComment_45,SIGNAL(released()),this,SLOT(bttnComment_45()));
    connect(ui->bttnComment_46,SIGNAL(released()),this,SLOT(bttnComment_46()));
    connect(ui->bttnComment_47,SIGNAL(released()),this,SLOT(bttnComment_47()));
    connect(ui->bttnComment_48,SIGNAL(released()),this,SLOT(bttnComment_48()));
    connect(ui->bttnComment_49,SIGNAL(released()),this,SLOT(bttnComment_49()));
    connect(ui->bttnComment_50,SIGNAL(released()),this,SLOT(bttnComment_50()));
}

void MainWindow::bttnComment_1()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(0);
}

void MainWindow::bttnComment_2()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(1);
}

void MainWindow::bttnComment_3()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(2);
}

void MainWindow::bttnComment_4()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(3);
}

void MainWindow::bttnComment_5()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(4);
}

void MainWindow::bttnComment_6()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(5);
}

void MainWindow::bttnComment_7()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(6);
}

void MainWindow::bttnComment_8()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(7);
}

void MainWindow::bttnComment_9()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(8);
}

void MainWindow::bttnComment_10()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(9);
}

void MainWindow::bttnComment_11()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(10);
}

void MainWindow::bttnComment_12()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(11);
}

void MainWindow::bttnComment_13()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(12);
}

void MainWindow::bttnComment_14()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(13);
}

void MainWindow::bttnComment_15()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(14);
}

void MainWindow::bttnComment_16()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(15);
}

void MainWindow::bttnComment_17()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(16);
}

void MainWindow::bttnComment_18()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(17);
}

void MainWindow::bttnComment_19()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(18);
}

void MainWindow::bttnComment_20()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(19);
}

void MainWindow::bttnComment_21()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(20);
}

void MainWindow::bttnComment_22()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(21);
}

void MainWindow::bttnComment_23()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(22);
}

void MainWindow::bttnComment_24()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(23);
}

void MainWindow::bttnComment_25()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(24);
}

void MainWindow::bttnComment_26()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(25);
}

void MainWindow::bttnComment_27()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(26);
}

void MainWindow::bttnComment_28()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(27);
}

void MainWindow::bttnComment_29()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(28);
}

void MainWindow::bttnComment_30()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(29);
}

void MainWindow::bttnComment_31()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(30);
}

void MainWindow::bttnComment_32()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(31);
}

void MainWindow::bttnComment_33()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(32);
}

void MainWindow::bttnComment_34()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(33);
}

void MainWindow::bttnComment_35()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(34);
}

void MainWindow::bttnComment_36()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(35);
}

void MainWindow::bttnComment_37()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(36);
}

void MainWindow::bttnComment_38()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(37);
}

void MainWindow::bttnComment_39()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(38);
}

void MainWindow::bttnComment_40()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(39);
}

void MainWindow::bttnComment_41()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(40);
}

void MainWindow::bttnComment_42()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(41);
}

void MainWindow::bttnComment_43()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(42);
}

void MainWindow::bttnComment_44()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(43);
}

void MainWindow::bttnComment_45()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(44);
}

void MainWindow::bttnComment_46()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(45);
}

void MainWindow::bttnComment_47()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(46);
}

void MainWindow::bttnComment_48()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(47);
}

void MainWindow::bttnComment_49()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(48);
}

void MainWindow::bttnComment_50()
{
    ui->stackedWidgetParcial->setCurrentIndex(1);
    verComentarios(49);
}

