#include "Quiz.h"
#include "User.h"
#include "QuestionGeneration.h"
#include "Screens/MainMenu.h"
#include "Screens/StatsMenu.h"
#include <QApplication>
#include <QGridLayout>
#include <QMainWindow>
#include <QPushButton>
int main(int argc, char *argv[])
{
    User *user = new User("Galen");
    QApplication app(argc, argv);

    QMainWindow *w = new QMainWindow(); 
    MyMemoryTranslator *translator = new MyMemoryTranslator(w);

    w->setGeometry(QRect(QPoint(0, 0), QSize(1920, 1080)));
    
    w->setCentralWidget(new MainMenu(w,user,translator));
    // Display window
    w->setWindowTitle("SmartMentor");
    w->show();
    return app.exec();
}