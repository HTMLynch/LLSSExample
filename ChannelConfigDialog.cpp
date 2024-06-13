#include "ChannelConfigDialog.h"
#include <sstream>
#include <iostream>

QT_BEGIN_NAMESPACE


static int GetIndexOf(QComboBox* pCB, std::string sText)
{
    std::transform(sText.begin(), sText.end(), sText.begin(), ::tolower);
    for(int i = 0; i < pCB->count(); i++) {
        std::string sLower(pCB->itemText(i).toStdString());
        std::transform(sLower.begin(), sLower.end(), sLower.begin(), ::tolower);
        if(sLower == sText) return i;
    }

    return 0;
}


ChannelConfigurationDialog::ChannelConfigurationDialog(std::string sName,
    const json& jConfigJSON, const json& jCurrentConfig, QWidget *parent)
    : QDialog(parent)
    , m_jValidConfigJSON(jConfigJSON)
{
    if(objectName().isEmpty())
        setObjectName(QString::fromUtf8("ChannelConfigurationDialog"));
    resize(653, 452);
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(sizePolicy.hasHeightForWidth());
    setSizePolicy(sizePolicy);
    buttonBox = new QDialogButtonBox(this);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setGeometry(QRect(370, 380, 211, 32));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(
        QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    ChannelName = new QLabel(this);
    ChannelName->setObjectName(QString::fromUtf8("ChannelName"));
    ChannelName->setGeometry(QRect(50, 30, 551, 20));
    QFont font;
    font.setPointSize(20);
    font.setBold(true);
    font.setWeight(75);
    ChannelName->setFont(font);
    ChannelName->setAlignment(Qt::AlignCenter);
    verticalLayoutWidget_4 = new QWidget(this);
    verticalLayoutWidget_4->setObjectName(
        QString::fromUtf8("verticalLayoutWidget_4"));
    verticalLayoutWidget_4->setGeometry(QRect(30, 70, 261, 341));
    verticalLayout_4 = new QVBoxLayout(verticalLayoutWidget_4);
    verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
    verticalLayout_4->setContentsMargins(0, 0, 0, 0);
    horizontalLayout_3 = new QHBoxLayout();
    horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
    label_3 = new QLabel(verticalLayoutWidget_4);
    label_3->setObjectName(QString::fromUtf8("label_3"));
    QFont font1;
    font1.setPointSize(16);
    font1.setBold(true);
    font1.setWeight(75);
    label_3->setFont(font1);
    label_3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

    horizontalLayout_3->addWidget(label_3);

    Coupling = new QComboBox(verticalLayoutWidget_4);
    Coupling->setObjectName(QString::fromUtf8("Coupling"));

    horizontalLayout_3->addWidget(Coupling);


    verticalLayout_4->addLayout(horizontalLayout_3);

    horizontalLayout_4 = new QHBoxLayout();
    horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
    label_4 = new QLabel(verticalLayoutWidget_4);
    label_4->setObjectName(QString::fromUtf8("label_4"));
    label_4->setFont(font1);
    label_4->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

    horizontalLayout_4->addWidget(label_4);

    Range = new QComboBox(verticalLayoutWidget_4);
    Range->setObjectName(QString::fromUtf8("Range"));

    horizontalLayout_4->addWidget(Range);


    verticalLayout_4->addLayout(horizontalLayout_4);

    horizontalLayout_5 = new QHBoxLayout();
    horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
    label_5 = new QLabel(verticalLayoutWidget_4);
    label_5->setObjectName(QString::fromUtf8("label_5"));
    label_5->setFont(font1);
    label_5->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

    horizontalLayout_5->addWidget(label_5);

    Filter = new QComboBox(verticalLayoutWidget_4);
    Filter->setObjectName(QString::fromUtf8("Filter"));

    horizontalLayout_5->addWidget(Filter);


    verticalLayout_4->addLayout(horizontalLayout_5);

    line_4 = new QFrame(verticalLayoutWidget_4);
    line_4->setObjectName(QString::fromUtf8("line_4"));
    line_4->setFrameShape(QFrame::HLine);
    line_4->setFrameShadow(QFrame::Sunken);

    verticalLayout_4->addWidget(line_4);

    label_2 = new QLabel(verticalLayoutWidget_4);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    label_2->setFont(font1);
    label_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

    verticalLayout_4->addWidget(label_2);

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    groupBox = new QGroupBox(verticalLayoutWidget_4);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    QFont font2;
    font2.setPointSize(12);
    font2.setBold(true);
    font2.setWeight(75);
    groupBox->setFont(font2);
    groupBox->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
    verticalLayoutWidget = new QWidget(groupBox);
    verticalLayoutWidget->setObjectName(
        QString::fromUtf8("verticalLayoutWidget"));
    verticalLayoutWidget->setGeometry(QRect(10, 40, 92, 91));
    verticalLayout = new QVBoxLayout(verticalLayoutWidget);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    verticalLayout->setContentsMargins(0, 0, 0, 0);
    QuarterBridge = new QRadioButton(verticalLayoutWidget);
    QuarterBridge->setObjectName(QString::fromUtf8("QuarterBridge"));
    QuarterBridge->setEnabled(false);

    verticalLayout->addWidget(QuarterBridge);

    HalfBridge = new QRadioButton(verticalLayoutWidget);
    HalfBridge->setObjectName(QString::fromUtf8("HalfBridge"));
    HalfBridge->setEnabled(false);

    verticalLayout->addWidget(HalfBridge);

    FullBridge = new QRadioButton(verticalLayoutWidget);
    FullBridge->setObjectName(QString::fromUtf8("FullBridge"));
    FullBridge->setEnabled(false);
    FullBridge->setChecked(true);

    verticalLayout->addWidget(FullBridge);


    horizontalLayout->addWidget(groupBox);

    verticalSpacer = new QSpacerItem(
        20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    horizontalLayout->addItem(verticalSpacer);

    groupBox_2 = new QGroupBox(verticalLayoutWidget_4);
    groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
    groupBox_2->setFont(font2);
    groupBox_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
    verticalLayoutWidget_2 = new QWidget(groupBox_2);
    verticalLayoutWidget_2->setObjectName(
        QString::fromUtf8("verticalLayoutWidget_2"));
    verticalLayoutWidget_2->setGeometry(QRect(10, 40, 92, 91));
    verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
    verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
    verticalLayout_2->setContentsMargins(0, 0, 0, 0);
    Bridge120 = new QRadioButton(verticalLayoutWidget_2);
    Bridge120->setObjectName(QString::fromUtf8("Bridge120"));
    Bridge120->setEnabled(false);

    verticalLayout_2->addWidget(Bridge120);

    Bridge350 = new QRadioButton(verticalLayoutWidget_2);
    Bridge350->setObjectName(QString::fromUtf8("Bridge350"));
    Bridge350->setEnabled(false);
    Bridge350->setChecked(true);

    verticalLayout_2->addWidget(Bridge350);


    horizontalLayout->addWidget(groupBox_2);


    verticalLayout_4->addLayout(horizontalLayout);

    verticalLayout_3 = new QVBoxLayout();
    verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
    verticalLayout_5 = new QVBoxLayout();
    verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));

    verticalLayout_3->addLayout(verticalLayout_5);

    line_3 = new QFrame(verticalLayoutWidget_4);
    line_3->setObjectName(QString::fromUtf8("line_3"));
    line_3->setFrameShape(QFrame::HLine);
    line_3->setFrameShadow(QFrame::Sunken);

    verticalLayout_3->addWidget(line_3);

    label = new QLabel(verticalLayoutWidget_4);
    label->setObjectName(QString::fromUtf8("label"));
    label->setFont(font2);
    label->setAlignment(Qt::AlignCenter);

    verticalLayout_3->addWidget(label);

    horizontalLayout_2 = new QHBoxLayout();
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    ExcitationEnable = new QCheckBox(verticalLayoutWidget_4);
    ExcitationEnable->setObjectName(QString::fromUtf8("ExcitationEnable"));
    ExcitationEnable->setEnabled(true);

    horizontalLayout_2->addWidget(ExcitationEnable);

    ExcitationLevel = new QComboBox(verticalLayoutWidget_4);
    ExcitationLevel->setObjectName(QString::fromUtf8("ExciationLevel"));
    ExcitationLevel->setEnabled(true);
    QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(
        ExcitationLevel->sizePolicy().hasHeightForWidth());
    ExcitationLevel->setSizePolicy(sizePolicy1);

    horizontalLayout_2->addWidget(ExcitationLevel);
    verticalLayout_3->addLayout(horizontalLayout_2);

    excitationTypeLayout = new QHBoxLayout();
    excitationTypeLabel = new QLabel(tr("Type: "));
    QFont fonte;
    fonte.setPointSize(12);
    fonte.setBold(true);
    fonte.setWeight(75);
    excitationTypeLabel->setFont(fonte);
    ExcitationUnipolar = new QRadioButton(tr("Unipolar"));
    ExcitationBipolar = new QRadioButton(tr("Bipolar"));
    excitationTypeLayout->addWidget(excitationTypeLabel);
    excitationTypeLayout->addWidget(ExcitationUnipolar);
    excitationTypeLayout->addWidget(ExcitationBipolar);
    QButtonGroup*   pBG = new QButtonGroup();
    pBG->addButton(ExcitationUnipolar);
    pBG->addButton(ExcitationBipolar);

    verticalLayout_3->addLayout(excitationTypeLayout);


    verticalLayout_4->addLayout(verticalLayout_3);

    verticalLayoutWidget_6 = new QWidget(this);
    verticalLayoutWidget_6->setObjectName(
        QString::fromUtf8("verticalLayoutWidget_6"));
    verticalLayoutWidget_6->setGeometry(QRect(360, 70, 261, 184));
    verticalLayout_6 = new QVBoxLayout(verticalLayoutWidget_6);
    verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
    verticalLayout_6->setContentsMargins(0, 0, 0, 0);
    label_9 = new QLabel(verticalLayoutWidget_6);
    label_9->setObjectName(QString::fromUtf8("label_9"));
    label_9->setFont(font1);
    label_9->setAlignment(Qt::AlignCenter);

    verticalLayout_6->addWidget(label_9);

    line_2 = new QFrame(verticalLayoutWidget_6);
    line_2->setObjectName(QString::fromUtf8("line_2"));
    line_2->setFrameShape(QFrame::HLine);
    line_2->setFrameShadow(QFrame::Sunken);

    verticalLayout_6->addWidget(line_2);

    horizontalLayout_7 = new QHBoxLayout();
    horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
    label_6 = new QLabel(verticalLayoutWidget_6);
    label_6->setObjectName(QString::fromUtf8("label_6"));
    label_6->setFont(font1);
    label_6->setAlignment(Qt::AlignCenter);

    horizontalLayout_7->addWidget(label_6);

    TriggerType = new QComboBox(verticalLayoutWidget_6);
    TriggerType->setObjectName(QString::fromUtf8("TriggerType"));

    horizontalLayout_7->addWidget(TriggerType);


    verticalLayout_6->addLayout(horizontalLayout_7);

    line = new QFrame(verticalLayoutWidget_6);
    line->setObjectName(QString::fromUtf8("line"));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    verticalLayout_6->addWidget(line);

    horizontalLayout_6 = new QHBoxLayout();
    horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
    groupBox_3 = new QGroupBox(verticalLayoutWidget_6);
    groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
    QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Minimum);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(groupBox_3->sizePolicy().hasHeightForWidth());
    groupBox_3->setSizePolicy(sizePolicy2);
    groupBox_3->setMinimumSize(QSize(70, 100));
    groupBox_3->setFont(font2);
    groupBox_3->setFlat(false);
    verticalLayoutWidget_3 = new QWidget(groupBox_3);
    verticalLayoutWidget_3->setObjectName(
        QString::fromUtf8("verticalLayoutWidget_3"));
    verticalLayoutWidget_3->setGeometry(QRect(0, 25, 111, 82));
    verticalLayout_7 = new QVBoxLayout(verticalLayoutWidget_3);
    verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
    verticalLayout_7->setContentsMargins(0, 0, 0, 0);
    TriggerModeOneShot = new QRadioButton(verticalLayoutWidget_3);
    TriggerModeOneShot->setObjectName(QString::fromUtf8("ExcitationUnipolar"));
    TriggerModeOneShot->setEnabled(false);
    TriggerModeOneShot->setFont(font2);
    TriggerModeOneShot->setChecked(true);

    verticalLayout_7->addWidget(TriggerModeOneShot);

    TriggerModeNormal = new QRadioButton(verticalLayoutWidget_3);
    TriggerModeNormal->setObjectName(QString::fromUtf8("TriggerModeNormal"));
    TriggerModeNormal->setEnabled(false);
    TriggerModeNormal->setFont(font2);

    verticalLayout_7->addWidget(TriggerModeNormal);


    horizontalLayout_6->addWidget(groupBox_3);

    verticalLayout_8 = new QVBoxLayout();
    verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
    label_7 = new QLabel(verticalLayoutWidget_6);
    label_7->setObjectName(QString::fromUtf8("label_7"));
    QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy3.setHorizontalStretch(0);
    sizePolicy3.setVerticalStretch(0);
    sizePolicy3.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
    label_7->setSizePolicy(sizePolicy3);
    label_7->setFont(font2);
    label_7->setAlignment(Qt::AlignCenter);

    verticalLayout_8->addWidget(label_7);

    TriggerLevel = new QSlider(verticalLayoutWidget_6);
    TriggerLevel->setObjectName(QString::fromUtf8("TriggerLevel"));
    TriggerLevel->setEnabled(false);
    QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Fixed);
    sizePolicy4.setHorizontalStretch(0);
    sizePolicy4.setVerticalStretch(0);
    sizePolicy4.setHeightForWidth(
        TriggerLevel->sizePolicy().hasHeightForWidth());
    TriggerLevel->setSizePolicy(sizePolicy4);
    TriggerLevel->setMinimum(-1000);
    TriggerLevel->setMaximum(1000);
    TriggerLevel->setOrientation(Qt::Horizontal);

    verticalLayout_8->addWidget(TriggerLevel);

    label_8 = new QLabel(verticalLayoutWidget_6);
    label_8->setObjectName(QString::fromUtf8("label_8"));
    sizePolicy3.setHeightForWidth(label_8->sizePolicy().hasHeightForWidth());
    label_8->setSizePolicy(sizePolicy3);
    label_8->setFont(font2);
    label_8->setAlignment(Qt::AlignCenter);

    verticalLayout_8->addWidget(label_8);

    TriggerHysteresis = new QSlider(verticalLayoutWidget_6);
    TriggerHysteresis->setObjectName(QString::fromUtf8("TriggerHysteresis"));
    TriggerHysteresis->setEnabled(false);
    sizePolicy4.setHeightForWidth(
        TriggerHysteresis->sizePolicy().hasHeightForWidth());
    TriggerHysteresis->setSizePolicy(sizePolicy4);
    TriggerHysteresis->setMaximum(1000);
    TriggerHysteresis->setValue(25);
    TriggerHysteresis->setOrientation(Qt::Horizontal);

    verticalLayout_8->addWidget(TriggerHysteresis);


    horizontalLayout_6->addLayout(verticalLayout_8);


    verticalLayout_6->addLayout(horizontalLayout_6);

    formLayoutWidget = new QWidget(this);
    formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
    formLayoutWidget->setGeometry(QRect(360, 290, 261, 31));
    formLayout = new QFormLayout(formLayoutWidget);
    formLayout->setObjectName(QString::fromUtf8("formLayout"));
    formLayout->setContentsMargins(0, 0, 0, 0);
    label_10 = new QLabel(formLayoutWidget);
    label_10->setObjectName(QString::fromUtf8("label_10"));
    label_10->setFont(font2);

    formLayout->setWidget(0, QFormLayout::LabelRole, label_10);

#ifdef WITH_SAMPLE_RATE
    SampleRate = new QComboBox(formLayoutWidget);
    SampleRate->setObjectName(QString::fromUtf8("SampleRate"));

    formLayout->setWidget(0, QFormLayout::FieldRole, SampleRate);
#endif

    QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QMetaObject::connectSlotsByName(this);

    setWindowTitle(QCoreApplication::translate(
        "ChannelConfigurationDialog", "Dialog", nullptr));
    ChannelName->setText(QCoreApplication::translate(
        "ChannelConfigurationDialog", sName.c_str(), nullptr));
    label_3->setText(QCoreApplication::translate(
        "ChannelConfigurationDialog", "Coupling", nullptr));
    label_4->setText(QCoreApplication::translate(
        "ChannelConfigurationDialog", "Range", nullptr));
    label_5->setText(QCoreApplication::translate(
        "ChannelConfigurationDialog", "Filter", nullptr));
    label_2->setText(QCoreApplication::translate(
        "ChannelConfigurationDialog", "Bridge", nullptr));
    groupBox->setTitle(QCoreApplication::translate(
        "ChannelConfigurationDialog", "Type", nullptr));
    QuarterBridge->setText(QCoreApplication::translate(
        "ChannelConfigurationDialog", "Quarter", nullptr));
    HalfBridge->setText(QCoreApplication::translate(
        "ChannelConfigurationDialog", "Half", nullptr));
    FullBridge->setText(QCoreApplication::translate(
        "ChannelConfigurationDialog", "Full", nullptr));
    groupBox_2->setTitle(QCoreApplication::translate(
        "ChannelConfigurationDialog", "Resistance", nullptr));
    Bridge120->setText(QCoreApplication::translate(
        "ChannelConfigurationDialog", "120", nullptr));
    Bridge350->setText(QCoreApplication::translate(
        "ChannelConfigurationDialog", "350", nullptr));
    label->setText(QCoreApplication::translate(
        "ChannelConfigurationDialog", "Excitation", nullptr));
    ExcitationEnable->setText(QCoreApplication::translate(
        "ChannelConfigurationDialog", "Enable", nullptr));
    label_9->setText(QCoreApplication::translate(
        "ChannelConfigurationDialog", "Trigger", nullptr));
    label_6->setText(QCoreApplication::translate(
        "ChannelConfigurationDialog", "Mode", nullptr));
    TriggerType->setCurrentText(QString());
    groupBox_3->setTitle(QCoreApplication::translate(
        "ChannelConfigurationDialog", "Type", nullptr));
    TriggerModeOneShot->setText(QCoreApplication::translate(
        "ChannelConfigurationDialog", "One-shot", nullptr));
    TriggerModeNormal->setText(QCoreApplication::translate(
        "ChannelConfigurationDialog", "Normal", nullptr));
    label_7->setText(QCoreApplication::translate(
        "ChannelConfigurationDialog", "Level", nullptr));
    label_8->setText(QCoreApplication::translate(
        "ChannelConfigurationDialog", "Hysteresis", nullptr));
#ifdef WITH_SAMPLE_RATE
    label_10->setText(QCoreApplication::translate(
        "ChannelConfigurationDialog", "SampleRate", nullptr));
#endif

    if(jConfigJSON.contains("/filter"_json_pointer)) {
        for(auto& [ idx, filter ] :
            jConfigJSON["/filter"_json_pointer].items()) {
            Filter->addItem(QString(static_cast<std::string>(filter).c_str()));
        }
    }
    if(jConfigJSON.contains("/coupling/type"_json_pointer)) {
        for(auto& [ idx, coupling ] :
            jConfigJSON["/coupling/type"_json_pointer].items()) {
            Coupling->addItem(
                QString(static_cast<std::string>(coupling).c_str()));
        }
    }
    if(jConfigJSON.contains("/trigger/mode"_json_pointer)) {
        for(auto& [ idx, mode ] :
            jConfigJSON["/trigger/mode"_json_pointer].items()) {
            TriggerType->addItem(
                QString(static_cast<std::string>(mode).c_str()));
        }
    }
    if(jConfigJSON.contains("/excitation/level"_json_pointer)) {
        for(auto& [ idx, level ] :
            jConfigJSON["/excitation/level"_json_pointer].items()) {
            double  dLevel = static_cast<double>(level);
            std::ostringstream  out;
            if(::remainder(dLevel, 1.0) != 0.0) {
                out.precision(3);
                out << std::fixed << dLevel;
            } else {
                out.precision(0);
                out << std::fixed << dLevel;
            }
            ExcitationLevel->addItem(QString(out.str().c_str()));
        }
    }
    if(jConfigJSON.contains("/range"_json_pointer)) {
        for(auto& [ idx, range ] : jConfigJSON["/range"_json_pointer].items()) {
            double  dRange = static_cast<double>(range);
            std::ostringstream  out;
            if(dRange < 1.0) {
                out.precision(2);
                out << std::fixed << dRange;
            } else {
                out.precision(0);
                out << std::fixed << dRange;
            }
            Range->addItem(QString(out.str().c_str()));
        }
    }
#ifdef WITH_SAMPLE_RATE
    if(jConfigJSON.contains("/sample_rate"_json_pointer)) {
        for(size_t i = 0; i < jConfigJSON["/sample_rate"_json_pointer].size();
            i += 2)
        {
            json::json_pointer  p("/sample_rate");
            double  dSampleRate = static_cast<double>(jConfigJSON[p / i]);
            std::ostringstream  out;
            if(::remainder(dSampleRate, 1.0) != 0.0) {
                out.precision(3);
                out << std::fixed << dSampleRate;
            } else {
                out.precision(0);
                out << std::fixed << dSampleRate;
            }
            SampleRate->addItem(QString(out.str().c_str()));
        }
    }
#endif

    // Initizlize controls from the current configuration information
    json    jFlattened = jCurrentConfig.flatten();
    double  dRange = 0.0;
    for(auto& [ key, value ] : jFlattened.items()) {
        try {
            // Convert the key to a string
            std::string sKey = static_cast<std::string>(key);

            if(sKey == "/coupling/type") {
                // Working
                std::string lV = static_cast<std::string>(value);
                std::transform(lV.begin(), lV.end(), lV.begin(), ::tolower);
                Coupling->setCurrentIndex(GetIndexOf(Coupling, lV));
                bool bEnable = false;
                if(lV == "strain") bEnable = true;
                Bridge120->setEnabled(bEnable);
                Bridge350->setEnabled(bEnable);
                QuarterBridge->setEnabled(bEnable);
                HalfBridge->setEnabled(bEnable);
                FullBridge->setEnabled(bEnable);

            } else if(sKey == "/coupling/bridge/type") {
                // Working
                std::string bt = static_cast<std::string>(value);
                if(bt == "quarter")
                    QuarterBridge->setChecked(true);
                else if(bt == "half")
                    HalfBridge->setChecked(true);
                else if(bt == "full")
                    FullBridge->setChecked(true);

            } else if(sKey == "/coupling/bridge/resistance") {
                // Working
                std::string bt = static_cast<std::string>(value);
                if(bt == "120")
                    Bridge120->setChecked(true);
                else if(bt == "350")
                    Bridge350->setChecked(true);

            } else if(sKey == "/excitation/enable") {
                // Working
                ExcitationEnable->setChecked(static_cast<bool>(value));

            } else if(sKey == "/excitation/level") {
                // Working
                double  dV = static_cast<double>(value);
                std::ostringstream  out;
                if(::remainder(dV, 1.0) != 0.0) {
                    out.precision(3);
                    out << std::fixed << dV;
                } else {
                    out.precision(0);
                    out << std::fixed << dV;
                }
                ExcitationLevel->setCurrentIndex(
                    GetIndexOf(ExcitationLevel, out.str()));

            } else if(sKey == "/excitation/type") {
                std::string bt = static_cast<std::string>(value);
                if(bt == "unipolar") ExcitationUnipolar->setChecked(true);
                else if(bt == "bipolar") ExcitationBipolar->setChecked(true);

            } else if(sKey == "/scale") {
                ; // Ignored
//                Scale->setText(value);

            } else if(sKey == "/offset") {
                ; // Ignored
//                Offset->setText(value);

            } else if(sKey == "/name") {
                ; // Ignored

            } else if(sKey == "/trigger/mode") {
                TriggerType->setCurrentIndex(
                    GetIndexOf(TriggerType, static_cast<std::string>(value)));
                enableTriggerWidgetsSlot();

            } else if(sKey == "/trigger/type") {
                if(static_cast<std::string>(value) == "normal")
                    TriggerModeNormal->setChecked(true);
                else
                    TriggerModeOneShot->setChecked(true);

            } else if(sKey == "/trigger/level") {
                // Working
                double  dV = static_cast<double>(value);
                // NOTE THIS ONLY WORKS BECAUSE RANGE GETS PARSED 1st
                dV /= dRange;
                int nValue = TriggerLevel->maximum() - TriggerLevel->minimum();
                nValue /= 2;
                nValue *= dV;
                TriggerLevel->setValue(nValue);

            } else if(sKey == "/trigger/hysteresis") {
                // Working
                double  dV = static_cast<double>(value);
                int nValue = TriggerHysteresis->maximum() -
                    TriggerHysteresis->minimum();
                nValue *= (dV / 100.0);
                nValue += TriggerHysteresis->minimum();
                TriggerHysteresis->setValue(nValue);

            } else if(sKey == "/filter") {
                Filter->setCurrentIndex(
                    GetIndexOf(Filter, static_cast<std::string>(value)));

            } else if(sKey == "/sample_rate") {
                ; // Ignored
            } else if(sKey == "/enable") {
                ; // Ignored

            } else if(sKey == "/range") {
                // Working
                dRange = static_cast<double>(value);
                std::ostringstream  out;
                if(dRange < 1.0) {
                    out.precision(2);
                    out << std::fixed << dRange;
                } else {
                    out.precision(0);
                    out << std::fixed << dRange;
                }
                Range->setCurrentIndex(GetIndexOf(Range, out.str()));
            }

            // Unknown keys are ignored
        }

        // Ignore exceptions
        catch(const std::exception& ex) {
            std::cerr << "EXCEPTION: " << ex.what() << std::endl;
        }
    }

    // Connect signals to slots
    connect(Coupling,
        QOverload<int>::of(&QComboBox::currentIndexChanged), this,
        &ChannelConfigurationDialog::enableBridgeWidgetsSlot);

#ifdef WITH_SAMPLE_RATE
    connect(SampleRate,
        QOverload<int>::of(&QComboBox::currentIndexChanged), this,
        &ChannelConfigurationDialog::sampleRateChangeSlot);
#endif

    // Range
    connect(Range,
        QOverload<int>::of(&QComboBox::currentIndexChanged), this,
        &ChannelConfigurationDialog::rangeChangeSlot);

    // Bridge
    connect(Bridge350, &QRadioButton::toggled, this,
        &ChannelConfigurationDialog::bridgeResistance350Slot);
    connect(Bridge120, &QRadioButton::toggled, this,
        &ChannelConfigurationDialog::bridgeResistance120Slot);
    connect(QuarterBridge, &QRadioButton::toggled, this,
        &ChannelConfigurationDialog::bridgeTypeQuarterSlot);
    connect(HalfBridge, &QRadioButton::toggled, this,
        &ChannelConfigurationDialog::bridgeTypeHalfSlot);
    connect(FullBridge, &QRadioButton::toggled, this,
        &ChannelConfigurationDialog::bridgeTypeFullSlot);

    // Excitation
    connect(ExcitationLevel,
        QOverload<int>::of(&QComboBox::currentIndexChanged), this,
        &ChannelConfigurationDialog::excitationSlot);
    connect(ExcitationEnable, &QCheckBox::toggled, this,
        &ChannelConfigurationDialog::excitationSlot);
    connect(ExcitationUnipolar, &QCheckBox::toggled, this,
        &ChannelConfigurationDialog::excitationSlot);
    connect(ExcitationBipolar, &QCheckBox::toggled, this,
        &ChannelConfigurationDialog::excitationSlot);

    // Trigger
    connect(TriggerType,
        QOverload<int>::of(&QComboBox::currentIndexChanged), this,
        &ChannelConfigurationDialog::enableTriggerWidgetsSlot);
    connect(TriggerModeOneShot, &QRadioButton::toggled, this,
        &ChannelConfigurationDialog::triggerModeOneShotSlot);
    connect(TriggerModeNormal, &QRadioButton::toggled, this,
        &ChannelConfigurationDialog::triggerModeNormalSlot);
    connect(TriggerLevel, &QSlider::sliderReleased, this,
        &ChannelConfigurationDialog::triggerLevelSlot);
    connect(TriggerHysteresis, &QSlider::sliderReleased, this,
        &ChannelConfigurationDialog::triggerHysteresisSlot);

    // Filter
    connect(Filter,
        QOverload<int>::of(&QComboBox::currentIndexChanged), this,
        &ChannelConfigurationDialog::filterSlot);
}

void ChannelConfigurationDialog::enableBridgeWidgetsSlot()
{
    bool    bEnable = false;

    if(Coupling->currentText() == tr("strain")) {
        bEnable = true;
        if(QuarterBridge->isChecked())
            m_jConfigJSON["/coupling/bridge/type"_json_pointer] = "quarter";
        if(HalfBridge->isChecked())
            m_jConfigJSON["/coupling/bridge/type"_json_pointer] = "half";
        if(FullBridge->isChecked())
            m_jConfigJSON["/coupling/bridge/type"_json_pointer] = "full";
        if(Bridge350->isChecked())
            m_jConfigJSON["/coupling/bridge/resistance"_json_pointer] = "350";
        if(Bridge120->isChecked())
            m_jConfigJSON["/coupling/bridge/resistance"_json_pointer] = "120";
    }

    m_jConfigJSON["/coupling/type"_json_pointer] =
        Coupling->currentText().toStdString();

    Bridge120->setEnabled(bEnable);
    Bridge350->setEnabled(bEnable);

    QuarterBridge->setEnabled(bEnable);
    HalfBridge->setEnabled(bEnable);
    FullBridge->setEnabled(bEnable);
}

void ChannelConfigurationDialog::enableTriggerWidgetsSlot()
{
    bool    bEnable = true;

    if(TriggerType->currentIndex() == 0) {
        m_jConfigJSON.erase("/trigger"_json_pointer);
        bEnable = false;

    } else {
        if(TriggerModeOneShot->isChecked())
            m_jConfigJSON["/trigger/type"_json_pointer] = "one-shot";
        if(TriggerModeNormal->isChecked())
            m_jConfigJSON["/trigger/type"_json_pointer] = "normal";
        m_jConfigJSON["/trigger/level"_json_pointer] =
            static_cast<double>(TriggerLevel->value()) / 1000.0 * 
                std::stod(Range->currentText().toStdString());
        m_jConfigJSON["/trigger/hysteresis"_json_pointer] =
            static_cast<double>(TriggerHysteresis->value()) / 10.0;
    }

    m_jConfigJSON["/trigger/mode"_json_pointer] =
        TriggerType->currentText().toStdString();

    TriggerModeOneShot->setEnabled(bEnable);
    TriggerModeNormal->setEnabled(bEnable);
    TriggerHysteresis->setEnabled(bEnable);
    TriggerLevel->setEnabled(bEnable);
}

void ChannelConfigurationDialog::rangeChangeSlot()
{
    m_jConfigJSON["/range"_json_pointer] =
        std::stod(Range->currentText().toStdString());

    if(TriggerType->currentIndex() != 0) {
        m_jConfigJSON["/trigger/level"_json_pointer] =
            static_cast<double>(TriggerLevel->value()) / 1000.0 * 
                std::stod(Range->currentText().toStdString());
    }
}

#ifdef WITH_SAMPLE_RATE
void ChannelConfigurationDialog::sampleRateChangeSlot()
{
    m_jConfigJSON["/sample_rate"_json_pointer] =
        std::stod(SampleRate->currentText().toStdString());
}
#endif

void ChannelConfigurationDialog::bridgeResistance350Slot()
{
    if(Bridge350->isChecked())
        m_jConfigJSON["/coupling/bridge/resistance"_json_pointer] = "350";
}

void ChannelConfigurationDialog::bridgeResistance120Slot()
{
    if(Bridge120->isChecked())
        m_jConfigJSON["/coupling/bridge/resistance"_json_pointer] = "120";
}

void ChannelConfigurationDialog::bridgeTypeQuarterSlot()
{
    if(QuarterBridge->isChecked())
        m_jConfigJSON["/coupling/bridge/type"_json_pointer] = "quarter";
}

void ChannelConfigurationDialog::bridgeTypeHalfSlot()
{
    if(HalfBridge->isChecked())
        m_jConfigJSON["/coupling/bridge/type"_json_pointer] = "half";
}

void ChannelConfigurationDialog::bridgeTypeFullSlot()
{
    if(FullBridge->isChecked())
        m_jConfigJSON["/coupling/bridge/type"_json_pointer] = "full";
}

void ChannelConfigurationDialog::excitationSlot()
{
    m_jConfigJSON["/excitation/level"_json_pointer] =
        std::stod(ExcitationLevel->currentText().toStdString());

    m_jConfigJSON["/excitation/enable"_json_pointer] =
        ExcitationEnable->isChecked();

    if(ExcitationUnipolar->isChecked())
        m_jConfigJSON["/excitation/type"_json_pointer] = "unipolar";
    else
        m_jConfigJSON["/excitation/type"_json_pointer] = "bipolar";
}

void ChannelConfigurationDialog::triggerModeOneShotSlot()
{
    if(TriggerModeOneShot->isChecked())
        m_jConfigJSON["/trigger/type"_json_pointer] = "one-shot";
}

void ChannelConfigurationDialog::triggerModeNormalSlot()
{
    if(TriggerModeNormal->isChecked())
        m_jConfigJSON["/trigger/type"_json_pointer] = "normal";
}

void ChannelConfigurationDialog::triggerTypeSlot()
{
std::cerr << TriggerType->currentText().toStdString() << std::endl;
    m_jConfigJSON["/trigger/mode"_json_pointer] =
        TriggerType->currentText().toStdString();
}

void ChannelConfigurationDialog::triggerLevelSlot()
{
    m_jConfigJSON["/trigger/level"_json_pointer] =
        static_cast<double>(TriggerLevel->value()) / 1000.0 * 
            std::stod(Range->currentText().toStdString());
}

void ChannelConfigurationDialog::triggerHysteresisSlot()
{
    m_jConfigJSON["/trigger/hysteresis"_json_pointer] =
        static_cast<double>(TriggerHysteresis->value()) / 10.0;
}

void ChannelConfigurationDialog::filterSlot()
{
    m_jConfigJSON["/filter"_json_pointer] = Filter->currentText().toStdString();
}
