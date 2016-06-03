/*
 * Copyright (c) 2016 Jakob Flierl <jakob.flierl@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of mqttc nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <QApplication>
#include <QDesktopWidget>
#include <QLabel>
#include <QKeyEvent>
#include <QDateTime>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    zoomTime = 1;

    ui->log->setMaximumBlockCount(100);

    QList<QKeySequence> s; // exit app with ESC or CTRL+Q
    s << QKeySequence(Qt::Key_Escape) << QKeySequence(Qt::CTRL + Qt::Key_Q);
    ui->action_Quit->setShortcuts(s);

    mqttHost = new QLineEdit();
    mqttHost->setMaximumWidth(200);

    ui->toolBar->insertWidget(ui->actionConnect_Disconnect, mqttHost);

    led1 = new QLedIndicator(this);
    led1->setOn(true);

    lag = new QLabel();
    lag->setMinimumWidth(150);
    lag->setAlignment(Qt::AlignRight);

    ui->status->addWidget(led1);
    ui->statusbar->addWidget(lag, 0);

    ui->plot->addGraph();
    ui->plot->legend->setVisible(true);

    //ui->plot->graph(0)->setAntialiased(false);
    ui->plot->graph(0)->setAntialiased(true);

    //ui->plot->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->plot->graph(0)->setLineStyle(QCPGraph::lsStepLeft);
    // ui->plot->graph(0)->setLineStyle(QCPGraph::lsLine);
    QPen pen(Qt::black);
    pen.setWidth(1);
    pen.setColor(QColor(0,0,0,0x7f));
    ui->plot->graph(0)->setPen(pen);
    ui->plot->graph(0)->setName("");

    QCPScatterStyle myScatter;
    myScatter.setShape(QCPScatterStyle::ssCircle);
    myScatter.setPen(QPen(Qt::blue));
    myScatter.setBrush(Qt::black);
    myScatter.setSize(2);
    ui->plot->graph(0)->setScatterStyle(myScatter);

    ui->plot->xAxis->setRange(0,10);
    ui->plot->yAxis->setRange(-0.25,1.25);
    ui->plot->yAxis->setAutoTickCount(5);

    ui->plot->xAxis->setDateTimeFormat("h:mm:ss");
    ui->plot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->plot->xAxis->setAutoTickStep(false);
    ui->plot->xAxis->setTickStep(1);

    ui->plot->yAxis->rescale(true);
    ui->plot->rescaleAxes();

    ui->plot->axisRect()->setupFullAxesBox(true);

    ui->plot->replot();

    ui->tabWidget->setTabText(0, tr("Plot"));
    ui->tabWidget->setTabText(1, tr("Log"));
    ui->tabWidget->setCurrentIndex(0);

    settings = new QSettings();

    subscriber = new Subscriber(QHostAddress(EXAMPLE_HOST),
                                EXAMPLE_PORT, this);

    //Publisher publisher;
    //publisher.connectToHost();

    timer = new QTimer(this);
    timer->setSingleShot(false);

    connect(timer, SIGNAL(timeout()), this, SLOT(updatePlot()));

    connect(subscriber, SIGNAL(signalReceived(QString, QString)), this, SLOT(onReceived(QString, QString)));

    connect(subscriber, SIGNAL(connected()),    this, SLOT(onConnected()));
    connect(subscriber, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

    connect(ui->actionConnect_Disconnect, SIGNAL(toggled(bool)), this, SLOT(connectDisconnectMQTT(bool)));

    loadSettings();

    timer->start(1000/120); // fps
}

void MainWindow::on_action_Zoom_In_triggered() {
    zoomTime /= 2;
    if (zoomTime > 60) {
        ui->plot->xAxis->setTickStep(60);
        ui->plot->xAxis->setDateTimeFormat("h:mm");
    } else if (zoomTime > 1) {
        ui->plot->xAxis->setTickStep(1);
        ui->plot->xAxis->setDateTimeFormat("h:mm:ss");
    } else {
        ui->plot->xAxis->setDateTimeFormat("h:mm:ss:zzz");
        ui->plot->xAxis->setTickStep(0.1);
    }
}

void MainWindow::on_action_Zoom_Out_triggered() {
    zoomTime *= 2;
    if (zoomTime > 60) {
        ui->plot->xAxis->setTickStep(60);
        ui->plot->xAxis->setDateTimeFormat("h:mm");
    } else if (zoomTime > 1) {
        ui->plot->xAxis->setTickStep(1);
        ui->plot->xAxis->setDateTimeFormat("h:mm:ss");
    } else {
        ui->plot->xAxis->setDateTimeFormat("h:mm:ss:zzz");
        ui->plot->xAxis->setTickStep(0.1);
    }
}

void MainWindow::connectDisconnectMQTT(bool connect) {
    if (connect) {
        if (!subscriber->connectionState()) {
            led1->setDisconnected(true);
            subscriber->setHostName(mqttHost->text());
            subscriber->connectToHost();
            subscriber->setAutoReconnect(true);
            ui->statusbar->showMessage(tr("Connecting to %1").arg(mqttHost->text()));

        }
    } else {
        subscriber->setAutoReconnect(false);
        subscriber->disconnectFromHost();
        subscriber->cleanSession();
        led1->setDisconnected(true);
    }
}

void MainWindow::onConnected() {
    ui->statusbar->showMessage(tr("Connected to %1").arg(subscriber->hostName()), 1000);
    log(QString("connected to %1").arg(subscriber->hostName()));
    led1->setDisconnected(false);
    ui->actionConnect_Disconnect->setChecked(true);
    ui->actionConnect_Disconnect->setToolTip(tr("Disconnect"));
}

void MainWindow::onDisconnected() {
    ui->statusbar->showMessage(tr("Disconnected"), 1000);
    log("disconnected");
    led1->setDisconnected(true);
    ui->actionConnect_Disconnect->setChecked(false);
    ui->actionConnect_Disconnect->setToolTip(tr("Connect"));
}

void MainWindow::onReceived(QString topic, QString msg) {
    double tsl = QDateTime::currentMSecsSinceEpoch() / 1000.0;

    log(QString("%1: %2").arg(topic, msg));

    if (QString(EXAMPLE_TOPIC) == topic) {

        bool ok1, ok2;
        double ts = msg.split(" ")[0].toDouble(&ok1);
        double tmp = msg.split(" ")[1].toDouble(&ok2);


        if (ok1 && ok2) {
            led1->setOn(tmp);

            ui->plot->graph(0)->addData(ts, tmp);
            ui->plot->graph(0)->removeDataBefore(ts - qMin((int)(zoomTime*10), 600)); // max 10 minutes buffer size
            ui->plot->graph(0)->setName(topic);

            double lagt = (tsl - ts) * 1000;
            lag->setText(tr("%1%2 msec lag").arg(lagt < 0 ? '-' : ' ').arg(qFabs(lagt),5,'f',2,' '));

        } else {
            QString info;
            info = QString("%1 %2 %3 %4").arg(QString::number(ts, 'f'),
                                              QString::number(tmp),
                                              QString::number(ok1),
                                              QString::number(ok2));

            log(tr("%1: error while converting %1").arg(topic, info));
        }
    }
}

void MainWindow::loadSettings() {
    settings->beginGroup("gui");

    restoreGeometry(settings->value("geometry", saveGeometry() ).toByteArray());
    restoreState(settings->value("state", saveState() ).toByteArray());
    move(settings->value("pos", pos()).toPoint());
    resize(settings->value("size", size()).toSize());

    if ( settings->value("fullscreen", isFullScreen() ).toBool()) {
        showFullScreen();
    } else {
        showNormal();
    }
    if ( settings->value("maximized", isMaximized() ).toBool()) {
        showMaximized();
    }

    zoomTime = settings->value("zoomTime", 1).toDouble();

    settings->endGroup();

    settings->beginGroup("mqtt");
    mqttHost->setText(settings->value("host", "127.0.0.1").toString());
    settings->endGroup();
}

void MainWindow::saveSettings() {
    settings->beginGroup("gui");

    settings->setValue("geometry", saveGeometry());
    settings->setValue("state", saveState());
    settings->setValue("fullscreen", isFullScreen());
    settings->setValue("maximized",  isMaximized());

    if ( !isMaximized() && !isFullScreen() ) {
        settings->setValue("pos", pos());
        settings->setValue("size", size());
    }

    settings->setValue("zoomTime", zoomTime);

    settings->endGroup();

    settings->beginGroup("mqtt");
    settings->setValue("host", mqttHost->text());
    settings->endGroup();
}

void MainWindow::closeEvent(QCloseEvent * event) {
    saveSettings();
    event->accept();
}

void MainWindow::keyPressEvent(QKeyEvent *e) {
    int keyInt = e->key();
    Qt::Key key = static_cast<Qt::Key>(keyInt);

    if (key == Qt::Key_unknown) {
        log(tr("Unknown key from a macro probably."));
        return;
    }
    // the user have clicked just and only the special keys Ctrl, Shift, Alt, Meta.
    if (key == Qt::Key_Control ||
            key == Qt::Key_Shift ||
            key == Qt::Key_Alt ||
            key == Qt::Key_Meta)
    {
        // qDebug() << "Single click of special key: Ctrl, Shift, Alt or Meta";
        // qDebug() << "New KeySequence:" << QKeySequence(keyInt).toString(QKeySequence::NativeText);
        // return;
    }

    // check for a combination of user clicks
    Qt::KeyboardModifiers modifiers = e->modifiers();
    QString keyText = e->text();
    // if the keyText is empty than it's a special key like F1, F5, ...
    //  qDebug() << "Pressed Key:" << keyText;
    QList<Qt::Key> modifiersList;
    if (modifiers & Qt::ShiftModifier)
        keyInt += Qt::SHIFT;
    if (modifiers & Qt::ControlModifier)
        keyInt += Qt::CTRL;
    if (modifiers & Qt::AltModifier)
        keyInt += Qt::ALT;
    if (modifiers & Qt::MetaModifier)
        keyInt += Qt::META;

    QString seq = QKeySequence(keyInt).toString(QKeySequence::NativeText);
    // qDebug() << "KeySequence:" << seq;

    return; // skip built in command if overridden by shortcut
    QMainWindow::keyPressEvent(e);
}

void MainWindow::log(QString l) {
    QString ts;
    ts.sprintf("%06.5f", QDateTime::currentMSecsSinceEpoch() / 1000.0);
    ui->log->appendPlainText(QString("%1 %2").arg(ts, l));
}

void MainWindow::updatePlot() {
    double tsl = QDateTime::currentMSecsSinceEpoch() / 1000.0;
    ui->plot->xAxis->setRange(tsl - zoomTime, tsl);
    ui->plot->replot(QCustomPlot::rpQueued);
}

void MainWindow::on_action_Print_triggered() {
    QPrinter printer;
    printer.setPageMargins (15,15,15,15, QPrinter::Millimeter);
    printer.setPageSize(QPrinter::A4);
    printer.setOrientation(QPrinter::Landscape);
    //printer.setResolution(QPrinter::HighResolution);
    printer.setFullPage(false);

    QPrintPreviewDialog preview(&printer, this);
    preview.setWindowTitle("Preview Dialog");
    Qt::WindowFlags flags(Qt::WindowTitleHint | Qt::Window);
    preview.setWindowFlags(flags);
    connect(&preview, SIGNAL(paintRequested(QPrinter *)), SLOT(printPreview(QPrinter *)));
    preview.exec();
}

void MainWindow::printPreview(QPrinter *printer) {
    printer->setPageSize(QPrinter::A4);
    QCPPainter painter(printer);
    QRectF pageRect = printer->pageRect(QPrinter::DevicePixel);

    int plotWidth = ui->plot->viewport().width();
    int plotHeight = ui->plot->viewport().height();
    double scale = pageRect.width()/(double)plotWidth;

    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);

    painter.setMode(QCPPainter::pmVectorized);
    painter.setMode(QCPPainter::pmNoCaching);

    // comment this out if you want cosmetic thin lines (always 1 pixel thick independent of pdf zoom level)
    painter.setMode(QCPPainter::pmNonCosmetic);

    painter.scale(scale, scale);
    ui->plot->toPainter(&painter, plotWidth, plotHeight);
}

MainWindow::~MainWindow()
{
    delete ui;
}
