#include "userHandling.h"

int programmStart(Database *dataBase) {
    if (dataBase->adminExists()) {


        //ui->tabWidget->setCurrentIndex(0);


        return 0;

    } else {
        return 1;

    }
}
