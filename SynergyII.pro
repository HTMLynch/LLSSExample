QT += widgets

QMAKE_CXXFLAGS += -std=c++2a

#LIBS += -lboost

HEADERS = SynergyIIMainWindow.h \
          Channel.h \
          ChannelConfigDialog.h \
          LowLatencyDataClient.h \
          MainWindowWidgets.h

SOURCES = SynergyII.cpp \
          SynergyIIMainWindow.cpp \
          Channel.cpp \
          ChannelConfigDialog.cpp \
          LowLatencyDataClient.cpp

RESOURCES   = SynergyII.qrc

target.path = ./SynergyII
INSTALLS += target
