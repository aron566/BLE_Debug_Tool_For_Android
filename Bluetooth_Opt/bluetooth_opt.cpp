/**
  ******************************************************************************
  * @file    bluetooth_opt.cpp
  * @author  aron566
  * @version v1.0
  * @date    2022-1-23
  * @brief   蓝牙操作模块.
  ******************************************************************************
  */
/* Header includes -----------------------------------------------------------*/
#include "bluetooth_opt.h"

/* Macro definitions ---------------------------------------------------------*/
/* Type definitions ----------------------------------------------------------*/
/* Variable declarations -----------------------------------------------------*/
/* Variable definitions ------------------------------------------------------*/
/* Function declarations -----------------------------------------------------*/
/* Function definitions ------------------------------------------------------*/
/**
 * @brief Bluetooth_Opt::Bluetooth_Opt
 * @param parent
 */
Bluetooth_Opt::Bluetooth_Opt(QObject *parent) : QObject(parent)
{
  /* 创建搜索服务：https://doc.qt.io/qt-5/qbluetoothdevicediscoveryagent.html */
  DiscoveryAgent_Obj = new QBluetoothDeviceDiscoveryAgent(this);

  /* 设置BLE的搜索时间 */
  DiscoveryAgent_Obj->setLowEnergyDiscoveryTimeout(Discover_Time_ms);

  /* 连接发现设备信息信号 */
  connect(DiscoveryAgent_Obj, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, \
          this, &Bluetooth_Opt::slot_AddBlueToothDevicesToList);

  /* 连接搜索完成信号 */
  connect(DiscoveryAgent_Obj, &QBluetoothDeviceDiscoveryAgent::finished, this, \
          &Bluetooth_Opt::slot_ScanFinished);

  /* 连接取消信号 */
  connect(DiscoveryAgent_Obj, &QBluetoothDeviceDiscoveryAgent::canceled, this, \
          &Bluetooth_Opt::slot_ScanCanceled);

  /* 开始进行设备搜索 */
  DiscoveryAgent_Obj->start(DiscoveryMethod_Sel);
}

/**
 * @brief Bluetooth_Opt::~Bluetooth_Opt
 */
Bluetooth_Opt::~Bluetooth_Opt()
{
  if(B_Controller_Obj != nullptr)
  {
    if(B_Controller_Obj->state() != QLowEnergyController::UnconnectedState)
    {
      B_Controller_Obj->disconnectFromDevice();
    }
  }
  delete DiscoveryAgent_Obj;
}

/**
 * @brief Bluetooth_Opt::Update_Device_List_Table
 */
void Bluetooth_Opt::Update_Device_List_Table()
{
  /* 停止搜索 */
  DiscoveryAgent_Obj->stop();

  /* 断开连接 */
  if(B_Controller_Obj != nullptr)
  {
    B_Controller_Obj->disconnectFromDevice();
    B_Controller_Obj = nullptr;
  }

  /* 清空设备列表 */
  B_Device_List.clear();

  /* 搜索蓝牙信号 */
  DiscoveryAgent_Obj->start(DiscoveryMethod_Sel);
}

/**
 * @brief Bluetooth_Opt::Connect_B_Device
 * @param B_Device
 */
void Bluetooth_Opt::Connect_B_Device(const QString B_Device)
{

  /* 获取选择的地址 */
  QString bltAddress = B_Device.left(17);

  for(int i = 0; i < B_Device_List.count(); i++)
  {
    /* 地址对比 */
    if(B_Device_List.at(i).address().toString().left(17) == bltAddress)
    {
      QBluetoothDeviceInfo choosenDevice = B_Device_List.at(i);

      /* 停止搜索服务 */
      DiscoveryAgent_Obj->stop();

      /* 获取蓝牙控制器对象 */
      Create_B_Controller_Obj(choosenDevice);
      break;
    }
  }
}

/**
 * @brief Bluetooth_Opt::Disconnect
 */
void Bluetooth_Opt::Disconnect()
{
  if(B_Controller_Obj != nullptr)
  {
    if(B_Controller_Obj->state() != QLowEnergyController::UnconnectedState)
    {
      B_Controller_Obj->disconnectFromDevice();

      /* 清空设备列表 */
      B_Device_List.clear();
    }
  }
}

/**
 * @brief Bluetooth_Opt::Create_B_Controller_Obj
 * @param info
 */
void Bluetooth_Opt::Create_B_Controller_Obj(const QBluetoothDeviceInfo &info)
{
  /* 建立通讯对象 */
  B_Controller_Obj = QLowEnergyController::createCentral(info, this);

  if(B_Controller_Obj == nullptr)
  {
    return;
  }
  /* 连接状态改变信号 */
  connect(B_Controller_Obj, &QLowEnergyController::stateChanged, this, &Bluetooth_Opt::slot_stateChanged);

  /* 连接设备 */
  B_Controller_Obj->connectToDevice();
}

/**
 * @brief Bluetooth_Opt::Search_Characteristic_of_Service
 * @param B_Service
 */
void Bluetooth_Opt::Search_Characteristic_of_Service(QLowEnergyService *B_Service)
{
  if(B_Service == nullptr)
  {
    return;
  }
  QList<QLowEnergyCharacteristic> list = B_Service->characteristics();

  /* 遍历UUID下各特征 */
  for(int i = 0; i < list.count(); i++)
  {
    QLowEnergyCharacteristic Characteristic = list.at(i);

    /* 如果QLowEnergyCharacteristic对象有效，则返回true，否则返回false */
    if(Characteristic.isValid() == false)
    {
      continue;
    }

//    ui->DEBUG_textBrowser->append(Characteristic.name());

    /* 返回特征的属性 */
    /* 这些属性定义了特征的访问权限 */

    /* 检测写权限 */
    if(Characteristic.properties() & QLowEnergyCharacteristic::WriteNoResponse \
       || Characteristic.properties() & QLowEnergyCharacteristic::Write)
    {
//      ui->DEBUG_textBrowser->insertPlainText("\n具有写权限!");

      /* 写权限特性 */
      if(Characteristic.properties() & QLowEnergyCharacteristic::WriteNoResponse)
      {
//        m_writeMode = QLowEnergyService::WriteWithoutResponse;
      }
      else
      {
//        m_writeMode = QLowEnergyService::WriteWithResponse;
      }
    }

    /* 检测读权限 */
    if(Characteristic.properties() & QLowEnergyCharacteristic::Read)
    {
//      ui->DEBUG_textBrowser->insertPlainText("\n具有读权限!");
    }

    /* 描述符定义特征如何由特定客户端配置 */

    /* 可配置描述，特征 */
//    m_notificationDesc = Characteristic.descriptor(QBluetoothUuid::ClientCharacteristicConfiguration);
//    if(m_notificationDesc.isValid())
//    {
//      //写描述符
//      B_Service->writeDescriptor(m_notificationDesc, QByteArray::fromHex("0100"));
//      ui->DEBUG_textBrowser->insertPlainText("\n写描述符!");
//    }
  }
}

/**
 * @brief Bluetooth_Opt::Decode_Service_UUID_Info
 * @param UUID_List
 */
void Bluetooth_Opt::Decode_Service_UUID_Info(QList<QBluetoothUuid> &Uuid_List)
{
  if(Uuid_List.size() == 0)
  {
    return;
  }

  /* 清空列表 */
  B_Service_Obj_List.clear();

  QBluetoothUuid it;
  QLowEnergyService *Service;
  for(int i = 0; i < Uuid_List.size(); i++)
  {
    it = Uuid_List.value(i);

    /* 通知发现的UUID */
    emit signal_Find_New_MainUuid(it.toString());

    /* 创建UUID服务对象 */
    Service = B_Controller_Obj->createServiceObject(it, this);
    B_Service_Obj_List.append(Service);

    /* 搜索该UUID下特征描述符 */
    Search_Characteristic_of_Service(Service);
  }
}

/**
 * @brief Bluetooth_Opt::Bluetooth_Set_Discover_Time
 * @param ms
 */
void Bluetooth_Opt::Bluetooth_Set_Discover_Time(qint32 ms)
{
  Discover_Time_ms = ms;
  DiscoveryAgent_Obj->setLowEnergyDiscoveryTimeout(Discover_Time_ms);
}

/**
 * @brief Bluetooth_Opt::Bluetooth_Set_DiscoveryMethod
 * @param DiscoveryMethod_Sel
 */
void Bluetooth_Opt::Bluetooth_Set_DiscoveryMethod(QBluetoothDeviceDiscoveryAgent::DiscoveryMethod Method)
{
  DiscoveryMethod_Sel = Method;
}

/**
 * @brief Bluetooth_Opt::slot_AddBlueToothDevicesToList
 * @param info
 */
void Bluetooth_Opt::slot_AddBlueToothDevicesToList(const QBluetoothDeviceInfo &info)
{
  /* 获取设备信息，并判断该设备是否为BLE设备 */
  if(info.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration)
  {
    /* 格式化设备地址和设备名称 */
    QString label = QString("%1 %2").arg(info.address().toString()).arg(info.name());

    /* 不存在则添加至设备列表 */
    if(B_Device_List.contains(info) == true)
    {
      return;
    }
    /* 添加设备信息到列表 */
    B_Device_List.append(info);

    /* 通知发现新蓝牙设备 */
    emit signal_Find_New_Device(label);
  }
}

/**
 * @brief MainWindow::slot_ScanFinished
 */
void Bluetooth_Opt::slot_ScanFinished()
{
  qDebug() << "Search Complete.";
}

/**
 * @brief MainWindow::slot_ScanCanceled
 */
void Bluetooth_Opt::slot_ScanCanceled()
{
  qDebug() << "slot_ScanCanceled.";
}

/**
 * @brief Bluetooth_Opt::slot_stateChanged
 * @param state
 */
void Bluetooth_Opt::slot_stateChanged(QLowEnergyController::ControllerState state)
{
  QString str;
  switch(state)
  {
    case QLowEnergyController::UnconnectedState:
      str.append(tr("UnconnectedState"));
      break;
    case QLowEnergyController::ConnectingState:
      str.append(tr("ConnectingState"));
      break;
    case QLowEnergyController::ConnectedState:
      str.append(tr("ConnectedState"));

      /* 触发搜寻服务 */
      B_Controller_Obj->discoverServices();
      break;
    case QLowEnergyController::DiscoveringState:
      str.append(tr("DiscoveringState"));
      break;
    case QLowEnergyController::DiscoveredState:
      str.append(tr("DiscoveredState"));

      /* 获取服务UUID */
      B_Service_List.clear();
      B_Service_List = B_Controller_Obj->services();
      Decode_Service_UUID_Info(B_Service_List);
      break;
    case QLowEnergyController::ClosingState:
      str.append(tr("ClosingState"));
      break;
    case QLowEnergyController::AdvertisingState:
      str.append(tr("AdvertisingState"));
      break;
  }
  emit signal_B_State_Changed(state, str);
}

/* ---------------------------- end of file ----------------------------------*/
