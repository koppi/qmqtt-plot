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

#include <QPainter>

#include "ledindicator.h"

QLedIndicator::QLedIndicator(QWidget *parent) : QWidget(parent)
{
    setDisconnected(true);
    scaledSize = 1000;

    setMinimumSize(24, 24);
    setOn(true);
    disconnectedColor1 =  QColor(50,50,50);
    disconnectedColor2 =  QColor(30,30,30);
    onColor1 = QColor(0,255,0);
    onColor2 = QColor(0,192,0);
    offColor1 = QColor(0,28,0);
    offColor2 = QColor(0,128,0);
}

void QLedIndicator::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event)
    update();
}

void QLedIndicator::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event)
    qreal realSize = qMin(width(), height());

    QRadialGradient gradient;
    QPainter painter(this);
    QPen     pen(Qt::black);
    pen.setWidth(1);

    // painter.setRenderHint(QPainter::Antialiasing);
    painter.translate((width())/2, (height())/2);
    painter.scale(realSize/scaledSize, realSize/scaledSize);

    gradient = QRadialGradient (QPointF(-500,-500), 1500, QPointF(-500,-500));
    gradient.setColorAt(0, QColor(224,224,224));
    gradient.setColorAt(1, QColor(28,28,28));
    painter.setPen(pen);
    painter.setBrush(QBrush(gradient));
    painter.drawEllipse(QPointF(2,2), 500-2, 500-2);

    gradient = QRadialGradient (QPointF(500,500), 1500, QPointF(500,500));
    gradient.setColorAt(0, QColor(224,224,224));
    gradient.setColorAt(1, QColor(28,28,28));
    painter.setPen(pen);
    painter.setBrush(QBrush(gradient));
    painter.drawEllipse(QPointF(2,2), 450-2, 450-2);

    painter.setPen(pen);

    if (isDisconnected()) {
        gradient = QRadialGradient (QPointF(500,500), 1500, QPointF(500,500));
        gradient.setColorAt(0, disconnectedColor1);
        gradient.setColorAt(1, disconnectedColor2);
    } else {
        if (isOn()) {
            gradient = QRadialGradient (QPointF(-500,-500), 1500, QPointF(-500,-500));
            gradient.setColorAt(0, onColor1);
            gradient.setColorAt(1, onColor2);
        } else {
            gradient = QRadialGradient (QPointF(500,500), 1500, QPointF(500,500));
            gradient.setColorAt(0, offColor1);
            gradient.setColorAt(1, offColor2);
        }
    }
    painter.setBrush(gradient);
    painter.drawEllipse(QPointF(2,2), 400-2, 400-2);
}
