#include <QString>
#include <iostream>
#include <QDir>
#include <QCoreApplication>
std::string bin_dir;
void test_QString() {
    QString qs(QString::asprintf("%04d", 1));
    std::cout << qs.toStdString() << std::endl;
}
void test_QDir() {
    //std::cout << QCoreApplication::applicationFilePath().toStdString() << std::endl;
    //std::cout << QDir().currentPath().toStdString() << std::endl;
    QString mk = QString::fromStdString(bin_dir + "/a");
    if (!QDir(mk).exists()) std::cout << mk.toStdString() << " not exists " << std::endl;
    QDir().mkdir(mk);
    if (QDir(mk).exists()) std::cout << mk.toStdString() << " is exists " << std::endl;
    

}
int main(int argc, char* argv[]) {
    std::string argv_str = std::string(argv[0]);
    bin_dir = argv_str.substr(0, argv_str.find_last_of("\\"));
    //test_QString();
    test_QDir();
    getchar();
    return 0;
}