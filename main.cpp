#include <QCoreApplication>
#include <zenomcore.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    a.setOrganizationName( "RobotLab" );
    a.setApplicationName( "ZenomCore" );

    ZenomCore z(&a, argc, argv);

    return a.exec();
}
