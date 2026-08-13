#include "userHandling.h"

void programmStart(Database *dataBase) {
    if (dataBase->adminExists()) {
        // Loginseite
    } else {
        // Adminseite mit Parameter für role = admin
    }
}
