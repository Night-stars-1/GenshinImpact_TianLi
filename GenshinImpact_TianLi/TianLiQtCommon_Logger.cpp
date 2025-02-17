﻿#define _LOG_SHOW

#ifdef _DEBUG
#define __DEFINE_LOGGER
#else 
#ifdef _LOG_SHOW
#define __DEFINE_LOGGER
#else
#endif // _LOG_SHOW
#endif // _DEBUG

#ifdef __DEFINE_LOGGER

#include "TianLiQtCommon_Logger.h"

#include <QMutex>
#include <QDateTime>
#include <QTableWidget>


#include <QNetworkReply>
#include <QNetworkAccessManager>

#include "Logger/TianLi.Logger/TianLi.Logger.h"
#pragma comment(lib, "TianLi.Logger.lib")

TianLiQtCommon_Logger& TianLiQtCommon_Logger::GetInstance()
{
	static QMutex mutex;
	static QScopedPointer<TianLiQtCommon_Logger> instance;
	if (instance.isNull())
	{
		mutex.lock();
		if (instance.isNull())
		{
			instance.reset(new TianLiQtCommon_Logger(NULL));
			TianLi::Logger::Logger::get_instance().set_callback([](const char* time, const char* type, const char* func, const char* msg) {
				TianLiQtCommon_Logger::GetInstance().append(time, type, func, msg);
			});
		}
		mutex.unlock();
	}
	return *instance.data();
}

TianLiQtCommon_Logger::TianLiQtCommon_Logger(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setAttribute(Qt::WA_QuitOnClose, false);
	//this->setWindowFlags(Qt::Tool);

	ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);     
	ui.tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);    
	ui.tableWidget->verticalHeader()->setVisible(false);                     
	ui.tableWidget->resizeColumnsToContents();                               
	ui.tableWidget->resizeRowsToContents();                                  
	ui.tableWidget->resizeColumnToContents(0);                               
	ui.tableWidget->setColumnCount(4);                                       
	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

	QFont font = ui.tableWidget->font();
	font.setPointSize(7);
	ui.tableWidget->setFont(font);
	
	QHeaderView* widthlist = ui.tableWidget->horizontalHeader();             
	widthlist->setStyleSheet("QHeaderView::section{border:0px;background-color:#f2f1f7;color:#7f7f7f;}");
	widthlist->resizeSection(0, 120);		
	widthlist->resizeSection(1, 80);
	widthlist->resizeSection(2, 600);
	widthlist->resizeSection(3, 100);
	widthlist->setStretchLastSection(true);
	QStringList list;                                                     
	// list << "时间戳" << "类型" << "函数" << "消息" ;
	list << QString::fromLocal8Bit("时间戳") << QString::fromLocal8Bit("类型") << QString::fromLocal8Bit("函数") << QString::fromLocal8Bit("消息");
	ui.tableWidget->setHorizontalHeaderLabels(list);
	ui.tableWidget->show();
	this->show();


	// Qt HttpGet
	QNetworkAccessManager* manager = new QNetworkAccessManager(this);
	connect(manager, &QNetworkAccessManager::finished, [=](QNetworkReply* reply) {
		if (reply->error() == QNetworkReply::NoError) {
			QByteArray bytes = reply->readAll();
			QString result(bytes);
			LogInfo(result.toStdString().c_str());
		}
		else {
			LogError("HttpGet Error");
		}
	reply->deleteLater();
	manager->deleteLater();
		});
	manager->get(QNetworkRequest(QUrl("http://download.api.weixitianli.com/GenshinImpactTianLi/Version/Latest")));
}

TianLiQtCommon_Logger::~TianLiQtCommon_Logger()
{
}

void TianLiQtCommon_Logger::append(QString time, QString type, QString func, QString msg)
{
	int rowcount = ui.tableWidget->rowCount();           //��ȡ��ǰ����
	ui.tableWidget->insertRow(rowcount);                 //������
	ui.tableWidget->setItem(rowcount, 0, new QTableWidgetItem(time)); //��������������
	QTableWidgetItem* item_type = new QTableWidgetItem(type);
	// Info Error Warning Debug other
	// blue red orange yellow green
	if (type == "Info")
	{
		item_type->setBackground(QColor(0, 0, 255, 100));
	}
	else if (type == "Error")
	{
		item_type->setBackground(QColor(255, 0, 0, 100));
	}
	else if (type == "Warning")
	{
		item_type->setBackground(QColor(255, 255, 0, 100));
	}
	else if (type == "Debug")
	{
		item_type->setBackground(QColor(0, 255, 0, 100));
	}
	else if(type=="Track")
	{
		item_type->setBackground(QColor(255, 255, 0,100));
	}
	else
	{
		item_type->setBackground(QColor(0, 255, 255, 100));
	}
	
	ui.tableWidget->setItem(rowcount, 1, item_type);
	ui.tableWidget->setItem(rowcount, 2, new QTableWidgetItem(func));
	ui.tableWidget->setItem(rowcount, 3, new QTableWidgetItem(msg));
}

void TianLiQtCommon_Logger::log(QString type, QString func, QString msg)
{
	QDateTime time = QDateTime::currentDateTime();  
	QString timestr = time.toString("hh:mm:ss:ms");             //������ʾ��ʽ
	append(timestr, type, func, msg);
}

void TianLiQtCommon_Logger::Trace(QString func, QString msg)
{
	log("Track", func, msg);
}

void TianLiQtCommon_Logger::Debug(QString func, QString msg)
{
	log("Debug", func, msg);
}

void TianLiQtCommon_Logger::Info(QString func, QString msg)
{
	log("Info", func, msg);
}

void TianLiQtCommon_Logger::Warn(QString func, QString msg)
{
	log("Warning", func, msg);
}

void TianLiQtCommon_Logger::Error(QString func, QString msg)
{
	log("Error", func, msg);
}

#endif