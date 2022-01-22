#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothSocket>
#include <QBluetoothServer>
#include <QLowEnergyCharacteristic>
#include <QLowEnergyController>
#include <QLowEnergyService>
#include <QList>

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
     * @brief Show_Service_UUID_Info
     * @param UUID_List
     */
    void Show_Service_UUID_Info(QList<QBluetoothUuid> &Uuid_List);

    /**
     * @brief SendMsg
     * @param text
     */
    void SendMsg(QString text);

private:
    void Search_Characteristic(QLowEnergyService *B_Service);
Q_SIGNALS:
    /**
     * @brief signal_Connect_B_Device_Info
     * @param info
     */
    void signal_Connect_B_Device_Info(const QBluetoothDeviceInfo &info);

private slots:
    /**
     * @brief slot_addBlueToothDevicesToList
     * @param info
     */
    void slot_AddBlueToothDevicesToList(const QBluetoothDeviceInfo &info);

    /**
     * @brief slot_scanFinished
     */
    void slot_ScanFinished();

    /**
     * @brief slot_ScanCanceled
     */
    void slot_ScanCanceled();

    /**
     * @brief slot_Create_B_Controller_Obj
     * @param info
     */
    void slot_Create_B_Controller_Obj(const QBluetoothDeviceInfo &info);

    /**
     * @brief slot_stateChanged
     * @param state
     */
    void slot_stateChanged(QLowEnergyController::ControllerState state);

    /**
     * @brief slot_B_error_occurred
     * @param newError
     */
    void slot_B_error_occurred(QLowEnergyController::Error newError);

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
    /* 对象搜索 */
    QBluetoothDeviceDiscoveryAgent *DiscoveryAgent_Obj = nullptr;

    /* 通讯连接 */
    QLowEnergyController *B_Controller_Obj = nullptr;

    /* 通讯服务 */
    QList<QLowEnergyService *> B_Service_Obj_List;
private:
  /* Bluetooth Device */
  QList<QBluetoothDeviceInfo> B_Device_List;

  /* UUID */
  QList<QBluetoothUuid> B_Service_List;  
};
#endif // MAINWINDOW_H
