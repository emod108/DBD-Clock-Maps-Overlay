#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QVector>
#include <QPair>
#include <QString>
#include <string>

// Array which holds pairs of realms' names and directories they are located in
static const QVector<QPair<const QString, const QString>> REALM_PATH_PAIRS = {
    {"No realm", "Choose a realm to continue"},
    {"Autohaven Wreckers", "AUTOHAVEN_WRECKERS"},
    {"Backwater Swamp", "BACKWATER_SWAMP"},
    {"Coldwind Farm", "COLDWIND_FARM"},
    {"Crotus Prenn Asylum", "CROTUS_PRENN_ASYLUM"},
    {"Dvarka Deepwood", "DVARKA_DEEPWOOD"},
    {"Forsaken Boneyard", "FORSAKEN_BONEYARD"},
    {"Gideon Meat Plant", "GIDEON_MEAT_PLANT"},
    {"Grave of Glenvale", "GRAVE_OF_GLENVALE"},
    {"Haddonfield", "HADDONFIELD"},
    {"Hawkins National Laboratory", "HAWKINS_NATIONAL_LABORATORY"},
    {"Lery's Memorial Institute", "LERYS_MEMORIAL_INSTITUTE"},
    {"MacMillan Estate", "MACMILLAN_ESTATE"},
    {"Ormond", "ORMOND"},
    {"Raccoon City", "RACCOON_CITY"},
    {"Red Forest", "RED_FOREST"},
    {"Silent Hill", "SILENT_HILL"},
    {"Springwood", "SPRINGWOOD"},
    {"The Decimated Borgo", "THE_DECIMATED_BORGO"},
    {"Withered Isle", "WITHERED_ISLE"},
    {"Yamaoka Estate", "YAMAOKA_ESTATE"},
};

static const QString CHOOSE_MAP_MESSAGE = "Choose a map";

#endif // CONSTANTS_H
