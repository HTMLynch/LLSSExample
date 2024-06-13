#include    <sstream>
#include    <iostream>
#include    <iomanip>
#include    <QtWidgets>
#include    <QGroupBox>
#include    <QGridLayout>
#include    <QLineEdit>
#include    <QSize>
#include    "SynergyIIMainWindow.h"
#include    "ChannelConfigDialog.h"
#include    <nlohmann/json.hpp>

static boost::asio::io_context  io_context;

using json = nlohmann::json;

SynergyIIMainWindow::SynergyIIMainWindow()
    : m_pCentralWidget(new QWidget(this))
{
    setCentralWidget(m_pCentralWidget);

    createActions();
    createStatusBar();

    setUnifiedTitleAndToolBarOnMac(true);

    // Connect signals to slots
    connect(this, &SynergyIIMainWindow::signalChannelAvailable, this,
        &SynergyIIMainWindow::ChannelAvailableSlot);
    connect(this, &SynergyIIMainWindow::signalChannelUnavailable, this,
        &SynergyIIMainWindow::ChannelUnavailableSlot);
    connect(this, &SynergyIIMainWindow::signalErrorMessage, this,
        &SynergyIIMainWindow::ErrorMessageSlot);
}

void SynergyIIMainWindow::closeEvent(QCloseEvent *pEvent)
{
    pEvent->accept();
}

static int GetIndexOf(QComboBox* pCB, std::string sText)
{
    std::transform(sText.begin(), sText.end(), sText.begin(), ::tolower);
    for(int i = 0; i < pCB->count(); i++)
        if(pCB->itemText(i).toStdString() == sText) return i;

    return 0;
}

void SynergyIIMainWindow::ChannelAvailableSlot()
{
    std::unique_lock<std::mutex>    lk(m_ListsLock);

    // Iterater over the new available channels list
    for(auto& ci : m_vNewAvailableChannels) {
        // Create a new channel widget
        std::unique_ptr<Channel>    pChannel =
            std::make_unique<Channel>(ci.sName, ci.jConfig);

        // Select the proper sample rate in the combo box
        double  dSampleRate =
            static_cast<double>(ci.jConfig["/sample_rate"_json_pointer]);
        std::ostringstream  out;
        if(::remainder(dSampleRate, 1.0) != 0.0) {
            out.precision(3);
            out << std::fixed << dSampleRate;
        } else {
            out.precision(0);
            out << std::fixed << dSampleRate;
        }
        m_pMainWindowWidgets->SampleRateComboBox->setCurrentIndex(GetIndexOf(
            m_pMainWindowWidgets->SampleRateComboBox, out.str()));

        // Add the channel widget to the layout
        m_pChannelsLayoutBox->addWidget(pChannel.get());

        // Connect handler to the configuration button
        connect(pChannel->ConfigureButton, &QPushButton::released, this,
            &SynergyIIMainWindow::handleChannelConfigButton);


        // Connect handler to the subscribe button
        connect(pChannel->SubscribeCheckBox, &QCheckBox::stateChanged, this,
            &SynergyIIMainWindow::handleChannelSubscribeButton);

        // Add the channel object to the list of channels
        m_mChannelsList[ci.sName] = std::move(pChannel);
    }

    // Clear the new available channels list
    m_vNewAvailableChannels.clear();
}

void SynergyIIMainWindow::ChannelUnavailableSlot()
{
    std::unique_lock<std::mutex>    lk(m_ListsLock);

    // Iterater over the new available channels list
    for(auto& sChannelName : m_vNewUnavailableChannels) {
        m_pChannelsLayoutBox->removeWidget(m_mChannelsList[sChannelName].get());
        m_mChannelsList.erase(sChannelName);
    }

    // Clear the new available channels list
    m_vNewUnavailableChannels.clear();
}

void SynergyIIMainWindow::PopulateSampleRates(void)
{
    // Default to decimal domain
    int nDomain = 0;

    // Get sample rate domain
    if(m_pMainWindowWidgets->BinaryRadioButton->isChecked()) nDomain = 1;

    // Set the sample rates
    m_pMainWindowWidgets->SampleRateComboBox->clear();
    for(auto& [ adc, info ] : m_pLLSC->ValidConfigJSON().items()) {
        std::string sAdc(adc);

        // If this key doesn't start with "adc" skip it
        if(sAdc.substr(0, 3) != "adc") continue;

        // Generate the json pointer for sample rates under this key
        json::json_pointer  jpSampleRates;
        jpSampleRates /= sAdc;
        jpSampleRates /= "sample_rate";

        // Make sure sample rates exist in this JSON object
        if(m_pLLSC->ValidConfigJSON().contains(jpSampleRates)) {
            const json& jSampleRates = 
                m_pLLSC->ValidConfigJSON()[jpSampleRates];

            // Iterate over the object adding sample rates from the
            // selected domain
            int nEntry = 0;
            int nDefaultEntry = 0;
            for(size_t i = nDomain; i < jSampleRates.size(); i += 2, nEntry++) {

                double  dSampleRate = static_cast<double>(jSampleRates[i]);

                // Skip if the rate is 0
                if(!(dSampleRate < 0) && !(dSampleRate > 0)) continue;

                std::ostringstream  out;
                if(::remainder(dSampleRate, 1.0) != 0.0) {
                    out.precision(3);
                    out << std::fixed << dSampleRate;
                } else {
                    out.precision(0);
                    out << std::fixed << dSampleRate;
                }
                m_pMainWindowWidgets->SampleRateComboBox->addItem(
                    QString(out.str().c_str()));

                // If this is 1000 or 1024, set it as the default entry
                if(dSampleRate == 1000.0 || dSampleRate == 1024.0)
                    nDefaultEntry = nEntry;
            }

            // Set the default entry
            m_pMainWindowWidgets->SampleRateComboBox->
                setCurrentIndex(nDefaultEntry);

            break;
        }
    }
}

void SynergyIIMainWindow::ErrorMessageSlot()
{
    QMessageBox::information(this, "Error from LLSS", m_sErrorMessage.c_str());
}

void SynergyIIMainWindow::handleLLSCEvents(EventType nType, const void *p,
    size_t sz)
{
    std::unique_lock<std::mutex>    lk(m_ListsLock);

    if(nType == EVENT_TYPE_AVAILABLE_CHANNEL) {
        // New available channel.  NOTE this has to be done via signal/slot
        // because this function is running is a thread other than the one
        // that created this window.
        const ChannelInfo*  pCi = reinterpret_cast<const ChannelInfo*>(p);
        m_vNewAvailableChannels.push_back(*pCi);
        emit signalChannelAvailable();

    } else if(nType ==  EVENT_TYPE_UNAVAILABLE_CHANNEL) {
        // Channel is no longer available
        std::string sChannelName(reinterpret_cast<const char *>(p), sz);
        m_vNewUnavailableChannels.push_back(sChannelName);
        emit signalChannelUnavailable();

    } else if(nType == EVENT_TYPE_CHANNEL_SUBSCRIBED) {
        // Channel successfully subscribed
        const ChannelSubscribedInfo* pCi =
            reinterpret_cast<const ChannelSubscribedInfo*>(p);
        m_mChannelsList[pCi->sName]->IdText->setText(
            std::to_string(pCi->nId).c_str());
        m_mChannelValues[pCi->nId] = m_mChannelsList[pCi->sName]->ValueText;
        m_mChannelsList[pCi->sName]->setProperty("CHId", pCi->nId);
        m_mChannelsList[pCi->sName]->ResetSampleCount();
#if 0
        for(auto& [ sName, pChannel ] : m_mChannelsList)
            pChannel->EnableConfigButton(false);
#endif

        // Disable sample changes if ANY channel is subscribed
        m_pMainWindowWidgets->DecimalRadioButton->setEnabled(false);
        m_pMainWindowWidgets->BinaryRadioButton->setEnabled(false);
        m_pMainWindowWidgets->SampleRateComboBox->setEnabled(false);

    } else if(nType == EVENT_TYPE_CHANNEL_UNSUBSCRIBED) {
        // Channel successfully unsubscribed
        bool    bSomeSubscribed = false;
        for(auto& [ sName, pChannel ] : m_mChannelsList) {
            if(pChannel->Subscribed()) {
                bSomeSubscribed = true;
                break;
            }
        }

        // If no channels subscribed, enable sample changes
        if(!bSomeSubscribed) {
            m_pMainWindowWidgets->DecimalRadioButton->setEnabled(true);
            m_pMainWindowWidgets->BinaryRadioButton->setEnabled(true);
            m_pMainWindowWidgets->SampleRateComboBox->setEnabled(true);
        }

#if 0
        if(bSomeSubscribed) bSomeSubscribed = false;
        else bSomeSubscribed = true;
        for(auto& [ sName, pChannel ] : m_mChannelsList)
            pChannel->EnableConfigButton(bSomeSubscribed);
#endif

    } else if(nType == EVENT_TYPE_CHANNEL_FIRST_SAMPLE_TS) {
        // First sample timestamp for this channel
        const ChannelTimestampInfo* pCi =
            reinterpret_cast<const ChannelTimestampInfo*>(p);
        m_mChannelsList[pCi->sName]->FSTSText->setText(
            std::to_string(pCi->dFirstSampleTimestamp).c_str());

    } else if(nType == EVENT_TYPE_CHANNEL_OFFSET) {
        // Ground reading taken at beginning of acquistion for this channel
        const ChannelOffsetInfo*    pCi =
            reinterpret_cast<const ChannelOffsetInfo*>(p);
        if(m_mChannelsList[pCi->sName]->SubscribeCheckBox->checkState() ==
            Qt::Checked)
            m_mChannelsList[pCi->sName]->OffsetText->setText(
                std::to_string(pCi->dOffset).c_str());

    } else if(nType == EVENT_TYPE_CHANNEL_DATA) {
        // Data for this channel
        const ChannelDataInfo*    pCi =
            reinterpret_cast<const ChannelDataInfo*>(p);
        std::string sValue = std::to_string(pCi->pData[pCi->nSamples - 1]);
        m_mChannelValues[pCi->nId]->setText(sValue.c_str());
        for(auto& [ sName, pChannel ] : m_mChannelsList) {
            if(pCi->nId == m_mChannelsList[sName]->property("CHId").toInt()) {
                pChannel->AccumulateSamples(pCi->nSamples);
                break;
            }
        }

    } else if(nType == EVENT_TYPE_ACQUIRE) {
        // Acquisition state change
        const bool*   bState = reinterpret_cast<const bool*>(p);
        if(*bState) {
            m_pMainWindowWidgets->AcquisitionButton->
                setText(tr("Stop Acquisition"));
            m_pMainWindowWidgets->DecimalRadioButton->setEnabled(false);
            m_pMainWindowWidgets->BinaryRadioButton->setEnabled(false);
            m_pMainWindowWidgets->SampleRateComboBox->setEnabled(false);
            m_pMainWindowWidgets->RedundantRecordCheckBox->setEnabled(false);

            for(auto& [ sName, pChannel ] : m_mChannelsList)
                pChannel->ResetSampleCount();

        } else {
            m_pMainWindowWidgets->AcquisitionButton->
                setText(tr("Start Acquisition"));
#if 0
            m_pMainWindowWidgets->DecimalRadioButton->setEnabled(true);
            m_pMainWindowWidgets->BinaryRadioButton->setEnabled(true);
            m_pMainWindowWidgets->SampleRateComboBox->setEnabled(true);
#endif
            m_pMainWindowWidgets->RedundantRecordCheckBox->setEnabled(true);
        }

    } else if(nType == EVENT_TYPE_CONFIG_INFO_AVAILABLE) {
        PopulateSampleRates();

    } else if(nType == EVENT_TYPE_PRECISE_START_TIME) {
        m_pMainWindowWidgets->PreciseStartTimeText->setText(
            reinterpret_cast<const char *>(p));

    } else if(nType == EVENT_TYPE_ERROR) {
        m_sErrorMessage.assign(reinterpret_cast<const char *>(p));
        emit signalErrorMessage();

    } else {
        ;
    }
}


void SynergyIIMainWindow::connectUnit()
{
    bool    bOk = false;
    QString sUnit = QInputDialog::getText(this,
        tr("Connect to SynergyII Device"),
        tr("Enter unit id of SynergyII device to connect to"),
        QLineEdit::Normal, QString(), &bOk);

    if(bOk && !sUnit.isEmpty()) {
        // Remove the existing widgets from the layout box
        for(auto& [ name, pChannel ] : m_mChannelsList) {
            m_pChannelsLayoutBox->removeWidget(pChannel.get());
        }

        // Clear out the channels list to destroy the previous channel objects
        m_mChannelsList.clear();

        // Disable acquire and info menu items
        m_pInfoAction->setEnabled(false);

        // Connect to the specified unit
        std::string sHost("synergy2-");
        sHost += sUnit.toStdString() + ".local";
        std::string sPort("10006");

        LowLatencyDataClient  *pLLSC;
        try {
            pLLSC = new LowLatencyDataClient(io_context, sHost, sPort,
                [this](EventType nType, const void *p, size_t nSize) {
                    handleLLSCEvents(nType, p, nSize);
                });
        }
        catch(...) {
            // Fail, leave everything uninitialized
            m_pLLSC.reset(nullptr);
            QMessageBox::about(this, tr("Connect ERROR"),
                tr("Unable to connect to SynergyII device"));
            return;
        }
        m_pLLSC.reset(pLLSC);

        m_sUnitId = sUnit.toStdString();
        std::string sConnected("SynergyII - Unit ");
        sConnected += m_sUnitId;
        this->setWindowTitle(sConnected.c_str());

        m_pInfoAction->setEnabled(true);
    }
}

void SynergyIIMainWindow::showInfo()
{
    std::stringstream   ss;
    ss << "<h1>Unit: synergy2-" << m_sUnitId << "</h1>";
}

void SynergyIIMainWindow::doAcquire()
{
    if(m_pMainWindowWidgets->RedundantRecordCheckBox->isChecked())
        m_pLLSC->Acquire(true);
    else
        m_pLLSC->Acquire(false);
}

void SynergyIIMainWindow::about()
{
    QMessageBox::about(this, tr("About SynergyII"),
        tr("The <b>SynergyII</b> application is a demonstration of "
            "use of the <b>Low Latency Streaming Service</b> provided "
            "by Hi-Techniques' <i>SynergyII</i> data acquisition device"));
}

void SynergyIIMainWindow::createActions()
{
    // Add "Action" to the menu bar
    QMenu*      pActionMenu = menuBar()->addMenu(tr("&Action"));

    // Add "Action -> Connect" to the action menu
    QAction*    pConnectAction = pActionMenu->addAction(tr("&Connect"), this,
        &SynergyIIMainWindow::connectUnit);
    pConnectAction->setStatusTip(
        tr("Connect to a SynergyII device"));

    // Add "Action -> Info" to the action menu
    m_pInfoAction = pActionMenu->addAction(tr("&Info"), this,
        &SynergyIIMainWindow::showInfo);
    m_pInfoAction->setEnabled(false);
    m_pInfoAction->setStatusTip(
        tr("Show information about connnected SynergyII device"));

    // Add a separator to the action menu
    pActionMenu->addSeparator();

    // Add "Action -> About" to action menu
    pActionMenu->addSeparator();
    QAction*    pAboutAction = pActionMenu->addAction(tr("&About"), this,
        &SynergyIIMainWindow::about);
    pAboutAction->setStatusTip(
        tr("Show information about this application"));

    // Add "Action -> Exit" to action menu
    const QIcon exitIcon = QIcon::fromTheme("application-exit");
    QAction*    pExitAction = pActionMenu->addAction(exitIcon, tr("&Exit"),
        this, &QWidget::close);
    pExitAction->setShortcuts(QKeySequence::Quit);
    pExitAction->setStatusTip(tr("Exit the application"));

    // Create the layout box for the channel widgets
    m_pChannelsLayoutBox = new QVBoxLayout(m_pCentralWidget);

    // Add the main window widgets to the group box
    m_pMainWindowWidgets = new MainWindowWidgets(m_pCentralWidget);
    m_pChannelsLayoutBox->addWidget(m_pMainWindowWidgets);
    m_pMainWindowWidgets->setMinimumSize(0, 120);

    connect(m_pMainWindowWidgets->DecimalRadioButton, &QRadioButton::toggled,
        this, &SynergyIIMainWindow::SampleRateDomainSlot);
    connect(m_pMainWindowWidgets->BinaryRadioButton, &QRadioButton::toggled,
        this, &SynergyIIMainWindow::SampleRateDomainSlot);
    connect(m_pMainWindowWidgets->AcquisitionButton, &QPushButton::clicked,
        this, &SynergyIIMainWindow::doAcquire);
    connect(m_pMainWindowWidgets->SampleRateComboBox,
        QOverload<int>::of(&QComboBox::currentIndexChanged), this,
        &SynergyIIMainWindow::SampleRateChangedSlot);
}

void SynergyIIMainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void SynergyIIMainWindow::handleChannelConfigButton()
{
    const QObject*  pObj = QObject::sender();
    std::string     sName = pObj->property("CHName").toString().toStdString();

    const json& j = m_pLLSC->ValidConfigJSON()["/adc940s0"_json_pointer];
    json::json_pointer  p;
    p /= sName;
    ChannelConfigurationDialog* pDialog;

    if(m_mChannelsList.contains(sName)) {
        const json& jCurrentConfig =
            m_mChannelsList[sName]->CurrentConfiguration();
        pDialog =
            new ChannelConfigurationDialog(sName, j, jCurrentConfig, this);

    } else return;

    pDialog->exec();

    json    jConfig = pDialog->ConfigJSON();

    if(jConfig.type() != json::value_t::null && jConfig.size() != 0) {
        // Update the current configuration information from the new config
        m_mChannelsList[sName]->UpdateConfiguration(jConfig);

        // Add enable ans sample rate
        jConfig["/enable"_json_pointer] = true;
        jConfig["/sample_rate"_json_pointer] = std::stod(
            m_pMainWindowWidgets->SampleRateComboBox->
                currentText().toStdString());

//std::cerr << jConfig.dump(4) << std::endl;
        // Configure the channel
        m_pLLSC->ConfigureChannel(sName, jConfig);
    }
}

void SynergyIIMainWindow::handleChannelSubscribeButton()
{
    const QCheckBox*    pObj = reinterpret_cast<QCheckBox*>(QObject::sender());
    std::string     sName = pObj->property("CHName").toString().toStdString();

    if(pObj->checkState() == Qt::Unchecked) {
        m_pLLSC->UnsubscribeChannel(
            m_mChannelsList[sName]->property("CHId").toInt());

    } else m_pLLSC->SubscribeChannel(sName, 1);
}

void SynergyIIMainWindow::SampleRateChangedSlot()
{
    json    jConfig;

    if(m_mChannelsList.size() < 4) return;
    std::string sName = m_mChannelsList.begin()->first;

    jConfig["/enable"_json_pointer] = true;
    jConfig["/sample_rate"_json_pointer] = std::stod(
        m_pMainWindowWidgets->SampleRateComboBox->
            currentText().toStdString());

    m_pLLSC->ConfigureChannel(sName, jConfig);
}
