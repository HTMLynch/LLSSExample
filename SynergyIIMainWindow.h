#pragma once

#include    <QMainWindow>
#include    <string>
#include    <vector>
#include    <memory>
#include    "Channel.h"
#include    "MainWindowWidgets.h"
#include    "LowLatencyDataClient.h"

class LowLatencyStreamClient;

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QSessionManger;
QT_END_NAMESPACE

class SynergyIIMainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        SynergyIIMainWindow();

    signals:
        void signalChannelAvailable() const;
        void signalChannelUnavailable() const;

        void signalErrorMessage() const;

    protected:
        void closeEvent(QCloseEvent *) override;

    private slots:
        void connectUnit();
        void showInfo();
        void doAcquire();
        void about();
        void handleChannelConfigButton();
        void handleChannelSubscribeButton();

        void ChannelAvailableSlot();
        void ChannelUnavailableSlot();

        void SampleRateDomainSlot() { PopulateSampleRates(); }
        void SampleRateChangedSlot();

        void ErrorMessageSlot();

    private:

        void createActions();
        void createStatusBar();
        void handleLLSCEvents(EventType, const void *, size_t);
        void PopulateSampleRates(void);

        QWidget*                                        m_pCentralWidget;
        QVBoxLayout*                                    m_pChannelsLayoutBox;
        std::string                                     m_sUnitId;
        std::map<std::string, std::unique_ptr<Channel>> m_mChannelsList;

        QAction*                                        m_pInfoAction;
        QAction*                                        m_pAcquireAction;

        std::unique_ptr<LowLatencyDataClient>           m_pLLSC;

        std::mutex                                      m_ListsLock;
        std::vector<ChannelInfo>                        m_vNewAvailableChannels;
        std::vector<std::string>                        m_vNewUnavailableChannels;
        std::map<int, QLabel*>                          m_mChannelValues;

        MainWindowWidgets*                              m_pMainWindowWidgets;

        std::string                                     m_sErrorMessage;
};
