#include    "Channel.h"
#include    <iostream>

Channel::Channel(std::string sName, const json& jConfig, QWidget *parent)
    : QWidget(parent)
    , m_nTotalSamples(0)
    , m_CurrentConfigurationJSON(jConfig)
{
    layoutWidget = this;
    layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
    layoutWidget->setGeometry(QRect(10, 10, 561, 159));
    verticalLayout_6 = new QVBoxLayout(layoutWidget);
    verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
    verticalLayout_6->setContentsMargins(5, 0, 5, 0);
    line = new QFrame(layoutWidget);
    line->setObjectName(QString::fromUtf8("line"));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    verticalLayout_6->addWidget(line);

    ChannelNameText = new QLabel(layoutWidget);
    ChannelNameText->setObjectName(QString::fromUtf8("ChannelNameText"));
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(
        ChannelNameText->sizePolicy().hasHeightForWidth());
    ChannelNameText->setSizePolicy(sizePolicy);
    QFont font;
    font.setPointSize(22);
    font.setBold(true);
    font.setWeight(75);
    ChannelNameText->setFont(font);
    ChannelNameText->setAlignment(Qt::AlignCenter);

    verticalLayout_6->addWidget(ChannelNameText);

    gridLayout = new QGridLayout();
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    label_12 = new QLabel(layoutWidget);
    label_12->setObjectName(QString::fromUtf8("label_12"));
    QFont font1;
    font1.setPointSize(12);
    font1.setBold(true);
    font1.setWeight(75);
    label_12->setFont(font1);
    label_12->setAlignment(Qt::AlignCenter);

    gridLayout->addWidget(label_12, 0, 1, 1, 1);

    label_9 = new QLabel(layoutWidget);
    label_9->setObjectName(QString::fromUtf8("label_9"));
    label_9->setFont(font1);
    label_9->setAlignment(Qt::AlignCenter);

    gridLayout->addWidget(label_9, 0, 2, 1, 1);

    label_11 = new QLabel(layoutWidget);
    label_11->setObjectName(QString::fromUtf8("label_11"));
    label_11->setFont(font1);
    label_11->setAlignment(Qt::AlignCenter);

    gridLayout->addWidget(label_11, 0, 0, 1, 1);

    label_mine = new QLabel(layoutWidget);
    label_mine->setObjectName(QString::fromUtf8("label_mine"));
    label_mine->setFont(font1);
    label_mine->setAlignment(Qt::AlignCenter);

    gridLayout->addWidget(label_mine, 0, 3, 1, 1);

    label_10 = new QLabel(layoutWidget);
    label_10->setObjectName(QString::fromUtf8("label_10"));
    label_10->setFont(font1);
    label_10->setAlignment(Qt::AlignCenter);

    gridLayout->addWidget(label_10, 0, 4, 1, 1);

    IdText = new QLabel(layoutWidget);
    IdText->setObjectName(QString::fromUtf8("IdText"));
    QFont font2;
    font2.setPointSize(14);
    font2.setItalic(false);
    IdText->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    IdText->setFont(font2);
    IdText->setFrameShape(QFrame::Box);

    gridLayout->addWidget(IdText, 1, 0, 1, 1);

    FSTSText = new QLabel(layoutWidget);
    FSTSText->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    FSTSText->setObjectName(QString::fromUtf8("FSTSText"));
    FSTSText->setFont(font2);
    FSTSText->setFrameShape(QFrame::Box);

    gridLayout->addWidget(FSTSText, 1, 1, 1, 1);

    OffsetText = new QLabel(layoutWidget);
    OffsetText->setObjectName(QString::fromUtf8("OffsetText"));
    QFont font3;
    font3.setPointSize(14);
    OffsetText->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    OffsetText->setFont(font3);
    OffsetText->setFrameShape(QFrame::Box);

    gridLayout->addWidget(OffsetText, 1, 2, 1, 1);

    TotalSamplesText = new QLabel(layoutWidget);
    TotalSamplesText->setObjectName(QString::fromUtf8("TotalSamplesText"));
    TotalSamplesText->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    TotalSamplesText->setFont(font3);
    TotalSamplesText->setFrameShape(QFrame::Box);

    gridLayout->addWidget(TotalSamplesText, 1, 3, 1, 1);

    ValueText = new QLabel(layoutWidget);
    ValueText->setObjectName(QString::fromUtf8("ValueText"));
    ValueText->setFont(font3);
    ValueText->setFrameShape(QFrame::Box);
//    ValueText->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    ValueText->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    gridLayout->addWidget(ValueText, 1, 4, 1, 1);

    gridLayout->setColumnMinimumWidth(0, 75);
    gridLayout->setColumnMinimumWidth(1, 100);
    gridLayout->setColumnMinimumWidth(2, 75);
    gridLayout->setColumnMinimumWidth(3, 75);
    gridLayout->setColumnMinimumWidth(4, 75);

    verticalLayout_6->addLayout(gridLayout);

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    SubscribeCheckBox = new QCheckBox(layoutWidget);
    SubscribeCheckBox->setObjectName(QString::fromUtf8("SubscribeCheckBox"));
    QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(
        SubscribeCheckBox->sizePolicy().hasHeightForWidth());
    SubscribeCheckBox->setSizePolicy(sizePolicy1);
    SubscribeCheckBox->setFont(font1);

    horizontalLayout->addWidget(SubscribeCheckBox);

    horizontalSpacer_5 =
        new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout->addItem(horizontalSpacer_5);

    ConfigureButton = new QPushButton(layoutWidget);
    ConfigureButton->setObjectName(QString::fromUtf8("ConfigureButton"));
    ConfigureButton->setFont(font1);
    ConfigureButton->setProperty("CHName", sName.c_str());

    horizontalLayout->addWidget(ConfigureButton);

    verticalLayout_6->addLayout(horizontalLayout);

    line_2 = new QFrame(layoutWidget);
    line_2->setObjectName(QString::fromUtf8("line_2"));
    line_2->setFrameShape(QFrame::HLine);
    line_2->setFrameShadow(QFrame::Sunken);

    verticalLayout_6->addWidget(line_2);

    ChannelNameText->setText(tr(sName.c_str()));
    label_12->setText(QCoreApplication::translate(
        "Channel", "1st Sample TS", nullptr));
    label_9->setText(QCoreApplication::translate(
        "Channel", "Ground Reading", nullptr));
    label_11->setText(QCoreApplication::translate(
        "Channel", "Id", nullptr));
    label_mine->setText(QCoreApplication::translate(
        "Channel", "Sample Count", nullptr));
    label_10->setText(QCoreApplication::translate(
        "Channel", "Value", nullptr));
    IdText->setText(QString());
    FSTSText->setText(QString());
    OffsetText->setText(QString());
    TotalSamplesText->setText(QString());
    ValueText->setText(QString());
    SubscribeCheckBox->setText(QCoreApplication::translate(
        "Channel", "Subscribe", nullptr));
    SubscribeCheckBox->setProperty("CHName", sName.c_str());
    ConfigureButton->setText(QCoreApplication::translate(
        "Channel", "configure...", nullptr));
}

void Channel::AccumulateSamples(int nSamples)
{
    m_nTotalSamples += nSamples;
    TotalSamplesText->setText(std::to_string(m_nTotalSamples).c_str());
}

void Channel::ResetSampleCount(void)
{
    m_nTotalSamples = 0;
}
