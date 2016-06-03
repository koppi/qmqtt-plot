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

#ifndef QLEDINDICATOR_H
#define QLEDINDICATOR_H

#include <QWidget>
#include <QResizeEvent>
#include <QColor>

class QLedIndicator : public QWidget
{
    Q_PROPERTY(QColor disconnectedColor1 WRITE setDisconnectedColor1 READ getDisconnectedColor1 );
    Q_PROPERTY(QColor disconnectedColor2 WRITE setDisconnectedColor2 READ getDisconnectedColor2 );
    Q_PROPERTY(QColor onColor1      WRITE setOnColor1     READ getOnColor1   );
    Q_PROPERTY(QColor onColor2      WRITE setOnColor2     READ getOnColor2   );
    Q_PROPERTY(QColor offColor1     WRITE setOffColor1    READ getOffColor1  );
    Q_PROPERTY(QColor offColor2     WRITE setOffColor2    READ getOffColor2  );
    Q_PROPERTY(bool disconnected    WRITE setDisconnected    READ isDisconnected  );

    Q_OBJECT
public:
    QLedIndicator(QWidget *parent);

    void setDisconnected(bool c)  { disconnected = c; update();   }
    void setDisconnectedColor1(QColor c)  { disconnectedColor1 = c; update();   }
    void setDisconnectedColor2(QColor c)  { disconnectedColor2 = c; update();   }
    void setOnColor1(QColor c)  { onColor1  = c; update();   }
    void setOffColor1(QColor c) { offColor1 = c; update();   }
    void setOnColor2(QColor c)  { onColor2  = c; update();   }
    void setOffColor2(QColor c) { offColor2 = c; update();   }

    void setOn(bool onoff)     { on = onoff; }

    bool isDisconnected(void)    { return disconnected;  }
    QColor getDisconnectedColor1(void)    { return disconnectedColor1;  }
    QColor getDisconnectedColor2(void)    { return disconnectedColor2;  }
    QColor getOnColor1(void)    { return onColor1;  }
    QColor getOffColor1(void)   { return offColor1; }
    QColor getOnColor2(void)    { return onColor2;  }
    QColor getOffColor2(void)   { return offColor2; }

    bool isOn() { return on; }

protected:
    virtual void paintEvent (QPaintEvent *event);
    virtual void resizeEvent(QResizeEvent *event);

private:
    qreal   scaledSize;
    QColor  disconnectedColor1, disconnectedColor2;
    QColor  onColor1, offColor1;
    QColor  onColor2, offColor2;
    QPixmap ledBuffer;
    bool disconnected;
    bool on;
};

#endif // QLEDINDICATOR_H
