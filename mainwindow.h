#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once

#include <QMainWindow>
#include "dialog.h"
#include "QCustomPlot/qcustomplot.h"
#include "apitest/apitest/champion.h"
#include "apitest/apitest/player.h"
#include "apitest/apitest/match.h"
#include <QString>
#include <vector>
#include <QDebug>
#include <QPair>
#include <QSystemTrayIcon>
#include <apitest/apitest/datamanip.h>
#include <QtAlgorithms>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setDisplayData(player p, QVector<champion> c, QVector<match> m);
    void displayData();

    void setChampData(std::vector<champion> c);
    void setChampTags(QMap<QString, QVector<QString>> tags);
    QString champIconFileName(int i);
    QString champIconFileName(QString s);
    void displayCurrentSummonerData();
    void loadSettings();
    void saveSettings(); //not implemented

private slots:

    void on_nameSetButton_clicked();

    void on_nameCancleButton_clicked();

    void on_nameEdit_textChanged();

    void on_matchListWidget_currentRowChanged(int currentRow);

    void trayIconClicked(QSystemTrayIcon::ActivationReason);
    void showHideWindow();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);

    void on_server_edit_textChanged();

    void on_authorizationkeys_edit_textChanged();

    void on_application_path_textChanged();

    void trayIconCloseAction_triggered();

    void on_tagFilterComboBox_currentIndexChanged(const QString &arg1);

    void on_graphWindowButton_clicked();

private:
    void displayMatchDetails(int index);
    void setupChampionDetails();
    void updateChampionDetails();
    void displayChampionDetails();
    void displayPlayerDetails(int index);

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;

    void createActions();
    void createTrayIcon();
    void showMessage();

public:

    std::vector<champion> mclv;
    player mp;
    QVector<player> mpl;
    QVector<champion> mcl;
    QVector<match> mml;
    QMap<QString, QVector<QString>> myChampTags;
    std::map<int, std::string> champIDs;
    QVector<QString> activeTags;

    datamanip *myDataManip;


    player *myPlayer;
    QVector<champion*> *myChampionList;
    QVector<match*> *myMatchList;

private:
    QString workingName;
        QString workingserver;
        QString authorization;
        QString application;

    //Dialog *myNewDialog;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
