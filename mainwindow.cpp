#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QScreen>
#include "QDebug"

#define SOFT_VERSION "v0.0.1"

/**
 * @brief MainWindow::MainWindow
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle(tr("Bluetooth Debug Tool By aron566 " SOFT_VERSION));

    QScreen *screen = QApplication::screens().at(0);
    qDebug() << "Screen Size:" << "x:" << screen->size().width() << "y:" << screen->size().height();

    /* Apply style sheet */
    QFile file(":/qdarkstyle/dark/style.qss");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        this->setStyleSheet(file.readAll());
        file.close();
    }

    /* 蓝牙对象 */
    Bluetooth_Obj = new Bluetooth_Opt(this);

    /* 设置BLE的搜索时间 */
    Bluetooth_Obj->Bluetooth_Set_Discover_Time(20000);

    /* 连接发现设备信息信号 */
    connect(Bluetooth_Obj, &Bluetooth_Opt::signal_Find_New_Device, this, &MainWindow::slot_AddBlueToothDevicesToList);

    /* 连接发现新UUID */
    connect(Bluetooth_Obj, &Bluetooth_Opt::signal_Find_New_MainUuid, this, &MainWindow::slot_Find_New_MainUuid);

    /* 连接状态改变 */
    connect(Bluetooth_Obj, &Bluetooth_Opt::signal_B_State_Changed, this, &MainWindow::slot_stateChanged);

    /* 设备搜索方法 */
    Bluetooth_Obj->Bluetooth_Set_DiscoveryMethod(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
}

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
  delete ui;
}

/**
 * @brief MainWindow::slot_Find_New_MainUuid
 * @param UUID_List
 */
void MainWindow::slot_Find_New_MainUuid(const QString &Uuid)
{
  ui->DEBUG_textBrowser->append(QString(tr("Find UUID: %1")).arg(Uuid));

  /* 添加到列表 */
  ui->MAIN_UUID_listWidget->addItem(Uuid);
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
void MainWindow::slot_AddBlueToothDevicesToList(const QString B_Device)
{
  ui->DEBUG_textBrowser->append("find device:");
  ui->DEBUG_textBrowser->append(B_Device);

  /* 检查设备是否已存在，避免重复添加 */
  QList<QListWidgetItem *> items = ui->B_DEVICE_listWidget->findItems(B_Device, Qt::MatchExactly);

  /* 不存在则添加至设备列表 */
  if(items.empty() == true)
  {
    QListWidgetItem *item = new QListWidgetItem(B_Device);
    ui->B_DEVICE_listWidget->addItem(item);
  }
}

/**
 * @brief MainWindow::slot_stateChanged
 * @param state
 */
void MainWindow::slot_stateChanged(QLowEnergyController::ControllerState state, \
                                   QString Str)
{
  switch(state)
  {
    case QLowEnergyController::UnconnectedState:
      ui->CONNECT_pushButton->setText(tr("连接"));
      break;
    case QLowEnergyController::ConnectingState:
      ui->CONNECT_pushButton->setText(tr("连接中"));
      break;
    case QLowEnergyController::ConnectedState:
      ui->CONNECT_pushButton->setText(tr("断开连接"));
      break;
    case QLowEnergyController::DiscoveringState:
      break;
    case QLowEnergyController::DiscoveredState:
      break;
    case QLowEnergyController::ClosingState:
      break;
    case QLowEnergyController::AdvertisingState:
      break;
  }
  ui->DEBUG_textBrowser->append(QString("slot_stateChanged: %1").arg(Str));
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

  if(ui->CONNECT_pushButton->text() == "连接")
  {
    /* 获取选择的地址 */
    QString bltAddress = ui->B_DEVICE_listWidget->currentItem()->text().left(17);

    /* 连接 */
    Bluetooth_Obj->Connect_B_Device(bltAddress);
  }
  else
  {
    /* 断开连接 */
    Bluetooth_Obj->Disconnect();
    /* 清空列表 */
    ui->MAIN_UUID_listWidget->clear();
    ui->CHARACTERISTIC_UUID_listWidget->clear();
  }
}

/**
 * @brief MainWindow::on_SEND_pushButton_clicked
 */
void MainWindow::on_SEND_pushButton_clicked()
{

}

/**
 * @brief MainWindow::on_UPDATE_pushButton_clicked
 */
void MainWindow::on_UPDATE_pushButton_clicked()
{
  /* 清空设备列表 */
  ui->B_DEVICE_listWidget->clear();

  /* 清空列表 */
  ui->MAIN_UUID_listWidget->clear();
  ui->CHARACTERISTIC_UUID_listWidget->clear();

  /* 触发更新 */
  Bluetooth_Obj->Update_Device_List_Table();
}
