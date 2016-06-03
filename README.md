### A Qt5 app that plots timestamped MQTT data

![qmqtt-plot screenshot](qmqtt-plot.png)

#### Build and install qmqtt-plot

```bash
git clone https://github.com/koppi/qmqtt-plot
cd qmqtt-plot
cd libqmqtt
qmake
make -j$(nproc)
sudo make install
cd ..
qmake
make
./qmqtt-plot
```

* Enter your MQTT broker's hostname and click connect.

#### Libraries included

* [qcustomplot.com](http://qcustomplot.com)
* [qmqtt](https://github.com/emqtt/qmqtt)

