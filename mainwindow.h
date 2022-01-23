#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include "Bluetooth_Opt/bluetooth_opt.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    /**
     * @brief SendMsg
     * @param text
     */
    void SendMsg(QString text);

private slots:
    /**
     * @brief slot_addBlueToothDevicesToList
     * @param info
     */
    void slot_AddBlueToothDevicesToList(const QString B_Device);

    /**
     * @brief slot_stateChanged
     * @param state
     */
    void slot_stateChanged(QLowEnergyController::ControllerState state, \
                           QString Str);

    /**
     * @brief slot_Find_New_MainUuid
     * @param Uuid
     */
    void slot_Find_New_MainUuid(const QString &Uuid);

    /**
     * @brief on_CONNECT_pushButton_clicked
     */
    void on_CONNECT_pushButton_clicked();

    /**
     * @brief on_SEND_pushButton_clicked
     */
    void on_SEND_pushButton_clicked();

    /**
     * @brief on_UPDATE_pushButton_clicked
     */
    void on_UPDATE_pushButton_clicked();

private:
    Ui::MainWindow *ui;

private:
  /* 蓝牙对象 */
  Bluetooth_Opt *Bluetooth_Obj = nullptr;
};
#endif // MAINWINDOW_H
