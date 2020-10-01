#include <QCoreApplication>
#include <zenomcore.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    a.setOrganizationName( "RobotLab" );
    a.setApplicationName( "ZenomCore" );

    QStringList args = a.arguments();

    ZenomCore z(&a, args);

    return a.exec();
}
