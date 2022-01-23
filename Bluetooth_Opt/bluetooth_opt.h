/**
 *  @file bluetooth_opt.h
 *
 *  @date 2022-1-23
 *
 *  @author aron566
 *
 *  @brief 蓝牙操作接口，添加模块：QT       += bluetooth
 *
 *  @version V1.0
 */
#ifndef BLUETOOTH_OPT_H
#define BLUETOOTH_OPT_H
/** Includes -----------------------------------------------------------------*/
#include <QObject>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothSocket>
#include <QBluetoothServer>
#include <QLowEnergyCharacteristic>
#include <QLowEnergyController>
#include <QLowEnergyService>
#include <QList>
/** Private includes ---------------------------------------------------------*/

/** Private defines ----------------------------------------------------------*/

/** Exported typedefines -----------------------------------------------------*/

/** Exported constants -------------------------------------------------------*/

/** Exported macros-----------------------------------------------------------*/
/** Exported variables -------------------------------------------------------*/
/** Exported functions prototypes --------------------------------------------*/
/**
 * @brief The Bluetooth_Opt class
 */
class Bluetooth_Opt : public QObject
{
  Q_OBJECT
public:
  explicit Bluetooth_Opt(QObject *parent = nullptr);
  ~Bluetooth_Opt();

public:
  /**
     * @brief Bluetooth_Set_Discover_Time 设置搜索蓝牙时间
     * @param ms
     */
    void Bluetooth_Set_Discover_Time(qint32 ms = 20000);

    /**
     * @brief Bluetooth_Set_DiscoveryMethod
     * @param DiscoveryMethod_Sel
     */
    void Bluetooth_Set_DiscoveryMethod(QBluetoothDeviceDiscoveryAgent::DiscoveryMethod Method = \
        QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);

    /**
     * @brief Update_Device_List_Table 更新设备列表
     */
    void Update_Device_List_Table();

    /**
     * @brief Connect_B_Device 连接蓝牙设备
     * @param B_Device
     */
    void Connect_B_Device(const QString B_Device);
    /**
     * @brief Disconnect
     */
    void Disconnect();

    /**
     * @brief Decode_Service_UUID_Info
     * @param UUID_List
     */
    void Decode_Service_UUID_Info(QList<QBluetoothUuid> &Uuid_List);

    /**
     * @brief SendMsg
     * @param text
     */
    void SendMsg(QString text);
signals:
  /**
   * @brief signal_Find_New_Device 发现蓝牙设备
   * @param B_Device 蓝牙设备名，格式：地址 名称
   */
  void signal_Find_New_Device(const QString B_Device);

  /**
   * @brief signal_Find_New_MainUuid 发现新UUID
   * @param Uuid
   */
  void signal_Find_New_MainUuid(const QString &Uuid);

  /**
   * @brief signal_B_State_Changed 蓝牙状态改变
   * @param state
   */
  void signal_B_State_Changed(QLowEnergyController::ControllerState state, QString info);
private:
    /**
     * @brief Search_Characteristic_of_Service
     * @param B_Service
     */
    void Search_Characteristic_of_Service(QLowEnergyService *B_Service);

    /**
     * @brief slot_Create_B_Controller_Obj
     * @param info
     */
    void Create_B_Controller_Obj(const QBluetoothDeviceInfo &info);
public slots:
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
   * @brief slot_stateChanged
   * @param state
   */
  void slot_stateChanged(QLowEnergyController::ControllerState state);
private:
  qint32 Discover_Time_ms = 20000;
private:
  /* 对象搜索 */
  QBluetoothDeviceDiscoveryAgent *DiscoveryAgent_Obj = nullptr;

  /* 搜寻方法协议 */
  QBluetoothDeviceDiscoveryAgent::DiscoveryMethod DiscoveryMethod_Sel = \
      QBluetoothDeviceDiscoveryAgent::LowEnergyMethod;

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

#endif // BLUETOOTH_OPT_H
/******************************** End of file *********************************/
