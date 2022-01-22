#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QScreen>
#include "QDebug"

/**
 * @brief MainWindow::MainWindow
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QScreen *screen = QApplication::screens().at(0);
    qDebug() << "Screen Size:" << "x:" << screen->size().width() << "y:" << screen->size().height();

    /* 创建搜索服务：https://doc.qt.io/qt-5/qbluetoothdevicediscoveryagent.html */
    DiscoveryAgent_Obj = new QBluetoothDeviceDiscoveryAgent(this);

    /* 设置BLE的搜索时间 */
    DiscoveryAgent_Obj->setLowEnergyDiscoveryTimeout(20000);

    /* 连接发现设备信息信号 */
    connect(DiscoveryAgent_Obj, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &MainWindow::slot_AddBlueToothDevicesToList);

    /* 连接搜索完成信号 */
    connect(DiscoveryAgent_Obj, &QBluetoothDeviceDiscoveryAgent::finished, this, &MainWindow::slot_ScanFinished);

    /* 连接取消信号 */
    connect(DiscoveryAgent_Obj, &QBluetoothDeviceDiscoveryAgent::canceled, this, &MainWindow::slot_ScanCanceled);

    /* 连接请求 */
    connect(this, &MainWindow::signal_Connect_B_Device_Info, this, &MainWindow::slot_Create_B_Controller_Obj);

    /* 开始进行设备搜索 */
    DiscoveryAgent_Obj->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
}

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
  if(B_Controller_Obj->state() != QLowEnergyController::UnconnectedState)
  {
    B_Controller_Obj->disconnectFromDevice();
  }
  delete ui;
}

/**
 * @brief MainWindow::Show_Service_UUID_Info
 * @param UUID_List
 */
void MainWindow::Show_Service_UUID_Info(QList<QBluetoothUuid> &Uuid_List)
{
  if(Uuid_List.size() == 0)
  {
    return;
  }

  /* 清空列表 */
  ui->MAIN_UUID_listWidget->clear();
  B_Service_Obj_List.clear();

  QBluetoothUuid it;
  QLowEnergyService *Service;
  for(int i = 0; i < Uuid_List.size(); i++)
  {
    it = Uuid_List.value(i);
    ui->DEBUG_textBrowser->append(QString(tr("Find UUID: %1")).arg(it.toString()));

    /* 添加到列表 */
    ui->MAIN_UUID_listWidget->addItem(it.toString());

    /* 创建UUID服务对象 */
    Service = B_Controller_Obj->createServiceObject(it, this);
    B_Service_Obj_List.append(Service);

    /* 搜索特征描述符 */
    Search_Characteristic(Service);
  }
}

/**
 * @brief MainWindow::Search_Characteristic
 * @param B_Service
 */
void MainWindow::Search_Characteristic(QLowEnergyService *B_Service)
{
  if(B_Service == nullptr)
  {
    return;
  }
  QList<QLowEnergyCharacteristic> list = B_Service->characteristics();

  ui->DEBUG_textBrowser->append(B_Service->serviceName());
  ui->DEBUG_textBrowser->append(QString(tr("Characteristic count = %1")).arg(list.count()));

  /* 遍历UUID下各特征 */
  for(int i = 0; i < list.count(); i++)
  {
    QLowEnergyCharacteristic Characteristic = list.at(i);

    /* 如果QLowEnergyCharacteristic对象有效，则返回true，否则返回false */
    if(Characteristic.isValid() == false)
    {
      continue;
    }

    ui->DEBUG_textBrowser->append(Characteristic.name());
    /* 返回特征的属性 */
    /* 这些属性定义了特征的访问权限 */

    /* 检测写权限 */
    if(Characteristic.properties() & QLowEnergyCharacteristic::WriteNoResponse \
       || Characteristic.properties() & QLowEnergyCharacteristic::Write)
    {
      ui->DEBUG_textBrowser->insertPlainText("\n具有写权限!");

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
 * @brief MainWindow::SendMsg
 * @param text
 */
void MainWindow::SendMsg(QString text)
{
    QByteArray array = text.toLocal8Bit();

//    B_Service->writeCharacteristic(m_writeCharacteristic, array, m_writeMode);
}

/**
 * @brief MainWindow::BleServiceCharacteristicRead
 * @param c
 * @param value
 */
//void MainWindow::BleServiceCharacteristicRead(const QLowEnergyCharacteristic &c, const QByteArray &value)
//{
//    Q_UNUSED(c)

//    ui->DEBUG_textBrowser->insertPlainText("\n当特征读取请求成功返回其值时：");
//    ui->DEBUG_textBrowser->insertPlainText(QString(value));
//}

/**
 * @brief MainWindow::slot_AddBlueToothDevicesToList
 * @param info
 */
void MainWindow::slot_AddBlueToothDevicesToList(const QBluetoothDeviceInfo &info)
{
  /* 获取设备信息，并判断该设备是否为BLE设备 */
  if(info.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration)
  {
    /* 格式化设备地址和设备名称 */
    QString label = QString("%1 %2").arg(info.address().toString()).arg(info.name());

    ui->DEBUG_textBrowser->append("find device:");
    ui->DEBUG_textBrowser->append(label);

    /* 检查设备是否已存在，避免重复添加 */
    QList<QListWidgetItem *> items = ui->B_DEVICE_listWidget->findItems(label, Qt::MatchExactly);

    /* 不存在则添加至设备列表 */
    if(items.empty() == true)
    {
      QListWidgetItem *item = new QListWidgetItem(label);
      ui->B_DEVICE_listWidget->addItem(item);
      B_Device_List.append(info);
    }
  }
}

/**
 * @brief MainWindow::slot_ScanFinished
 */
void MainWindow::slot_ScanFinished()
{
  qDebug() << "Search Complete.";
  ui->DEBUG_textBrowser->append("Search Complete");
}

/**
 * @brief MainWindow::slot_ScanCanceled
 */
void MainWindow::slot_ScanCanceled()
{
  qDebug() << "slot_ScanCanceled.";
  ui->DEBUG_textBrowser->append("slot_ScanCanceled");
}

/**
 * @brief MainWindow::slot_stateChanged
 * @param state
 */
void MainWindow::slot_stateChanged(QLowEnergyController::ControllerState state)
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
      Show_Service_UUID_Info(B_Service_List);
      break;
    case QLowEnergyController::ClosingState:
      str.append(tr("ClosingState"));
      break;
    case QLowEnergyController::AdvertisingState:
      str.append(tr("AdvertisingState"));
      break;
  }
  ui->DEBUG_textBrowser->append(QString("slot_stateChanged: %1").arg(str));
}

/**
 * @brief MainWindow::slot_B_error_occurred
 * @param newError
 */
void MainWindow::slot_B_error_occurred(QLowEnergyController::Error newError)
{
  QString str;
  switch(newError)
  {
    case QLowEnergyController::NoError:
      str.append(tr("NoError"));
      break;
    case QLowEnergyController::UnknownError:
      str.append(tr("UnknownError"));
      break;
    case QLowEnergyController::UnknownRemoteDeviceError:
      str.append(tr("UnknownRemoteDeviceError"));
      break;
    case QLowEnergyController::NetworkError:
      str.append(tr("NetworkError"));
      break;
    case QLowEnergyController::InvalidBluetoothAdapterError:
      str.append(tr("InvalidBluetoothAdapterError"));
      break;
    case QLowEnergyController::ConnectionError:
      str.append(tr("ConnectionError"));
      break;
    case QLowEnergyController::AdvertisingError:
      str.append(tr("AdvertisingError"));
      break;
    case QLowEnergyController::RemoteHostClosedError:
      str.append(tr("RemoteHostClosedError"));
      break;
  }
  ui->DEBUG_textBrowser->append(QString("slot_B_error_Check: %1").arg(str));
}

/**
 * @brief MainWindow::slot_Create_B_Controller_Obj
 * @param info
 */
void MainWindow::slot_Create_B_Controller_Obj(const QBluetoothDeviceInfo &info)
{
  /* 建立通讯对象 */
  B_Controller_Obj = QLowEnergyController::createCentral(info, this);

  if(B_Controller_Obj == nullptr)
  {
    return;
  }
  /* 连接状态改变信号 */
  connect(B_Controller_Obj, &QLowEnergyController::stateChanged, this, &MainWindow::slot_stateChanged);

  /* 连接错误信号 */
//  connect(B_Controller_Obj, &QLowEnergyController::error, this, &MainWindow::slot_B_error_occurred);

  /* 连接设备 */
  B_Controller_Obj->connectToDevice();
}

/**
 * @brief MainWindow::on_CONNECT_pushButton_clicked
 */
void MainWindow::on_CONNECT_pushButton_clicked()
{
  /* 确认选取了某一个蓝牙设备 */
  if(true == ui->B_DEVICE_listWidget->currentItem()->text().isEmpty())
  {
    return;
  }

  /* 获取选择的地址 */
  QString bltAddress = ui->B_DEVICE_listWidget->currentItem()->text().left(17);

  for(int i = 0; i < B_Device_List.count(); i++)
  {
    /* 地址对比 */
    if(B_Device_List.at(i).address().toString().left(17) == bltAddress)
    {
      QBluetoothDeviceInfo choosenDevice = B_Device_List.at(i);

      /* 发送自定义signals==>执行slots:createCtl */
      emit signal_Connect_B_Device_Info(choosenDevice);

      /* 停止搜索服务 */
      DiscoveryAgent_Obj->stop();
      break;
    }
  }
}

/**
 * @brief MainWindow::on_SEND_pushButton_clicked
 */
void MainWindow::on_SEND_pushButton_clicked()
{
//  SendMsg("Hello World");
}

/**
 * @brief MainWindow::on_UPDATE_pushButton_clicked
 */
void MainWindow::on_UPDATE_pushButton_clicked()
{
  /* 停止搜索 */
  DiscoveryAgent_Obj->stop();

  /* 断开连接 */
  if(B_Controller_Obj != nullptr)
  {
    B_Controller_Obj->disconnectFromDevice();
  }

  /* 清空链表 */
  ui->B_DEVICE_listWidget->clear();
  B_Device_List.clear();

  /* 搜索蓝牙信号 */
  DiscoveryAgent_Obj->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
}
