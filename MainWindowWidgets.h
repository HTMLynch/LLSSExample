#pragma once

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE
class QLabel;
class QCheckBox;
class QRadioButton;
class QVBoxLayout;
class QFrame;
class QGridLayout;
class QHBoxLayout;
class QSpacerItem;
class QPushButton;
class QTabWidget;
QT_END_NAMESPACE

//#define WITH_TABS

class MainWindowWidgets : public QWidget
{
public:
    MainWindowWidgets(QWidget *parent = nullptr)
        : m_pParent(parent) { setupUi(); }

    QCheckBox *RedundantRecordCheckBox;
    QPushButton *AcquisitionButton;
    QRadioButton *DecimalRadioButton;
    QRadioButton *BinaryRadioButton;
#ifdef WITH_TABS
    QTabWidget *ChannelsTab;
#endif
    QLabel *PreciseStartTimeText;
    QComboBox *SampleRateComboBox;

private:
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QFrame *line;
    QLabel *label;
    QVBoxLayout *verticalLayout;
#ifdef WITH_TABS
    QWidget *tab;
    QWidget *tab_2;
#endif
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QLabel *label_2;

    QWidget*    m_pParent;

    void setupUi(void)
    {
        if(objectName().isEmpty())
            setObjectName(QString::fromUtf8("MainWindowWidgets"));

        gridLayoutWidget = new QWidget(this);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(10, 20, 651, 100));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        AcquisitionButton = new QPushButton(gridLayoutWidget);
        AcquisitionButton->setObjectName(
            QString::fromUtf8("AcquisitionButton"));
        QFont font;
        font.setPointSize(20);
        font.setBold(true);
        font.setWeight(75);
        AcquisitionButton->setFont(font);

        gridLayout->addWidget(AcquisitionButton, 1, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        RedundantRecordCheckBox = new QCheckBox(gridLayoutWidget);
        RedundantRecordCheckBox->setObjectName(
            QString::fromUtf8("RedundantRecordCheckBox"));
        QFont font1;
        font1.setPointSize(12);
        font1.setBold(true);
        font1.setWeight(75);
        RedundantRecordCheckBox->setFont(font1);
        RedundantRecordCheckBox->setFocusPolicy(Qt::WheelFocus);
        RedundantRecordCheckBox->setLayoutDirection(Qt::LeftToRight);

        horizontalLayout->addWidget(RedundantRecordCheckBox);

        line = new QFrame(gridLayoutWidget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        horizontalLayout->addWidget(line);

        label = new QLabel(gridLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setFont(font1);
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(label);

        SampleRateComboBox = new QComboBox(gridLayoutWidget);
        SampleRateComboBox->setObjectName(
            QString::fromUtf8("SampleRateComboBox"));

        horizontalLayout->addWidget(SampleRateComboBox);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        DecimalRadioButton = new QRadioButton(gridLayoutWidget);
        DecimalRadioButton->setObjectName(
            QString::fromUtf8("DecimalRadioButton"));
        DecimalRadioButton->setFont(font1);
        DecimalRadioButton->setChecked(true);

        verticalLayout->addWidget(DecimalRadioButton);

        BinaryRadioButton = new QRadioButton(gridLayoutWidget);
        BinaryRadioButton->setObjectName(
            QString::fromUtf8("BinaryRadioButton"));
        BinaryRadioButton->setFont(font1);

        verticalLayout->addWidget(BinaryRadioButton);


        horizontalLayout->addLayout(verticalLayout);


        gridLayout->addLayout(horizontalLayout, 2, 0, 1, 1);

#ifdef WITH_TABS
        ChannelsTab = new QTabWidget(gridLayoutWidget);
        ChannelsTab->setObjectName(QString::fromUtf8("ChannelsTab"));
        ChannelsTab->setFont(font1);
        ChannelsTab->setTabPosition(QTabWidget::North);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        ChannelsTab->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        ChannelsTab->addTab(tab_2, QString());

        gridLayout->addWidget(ChannelsTab, 0, 0, 1, 1);
#endif

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(
            QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(
            40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        label_2 = new QLabel(gridLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font1);
        label_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(label_2);

        PreciseStartTimeText = new QLabel(gridLayoutWidget);
        PreciseStartTimeText->setObjectName(
            QString::fromUtf8("PreciseStartTimeText"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(
            PreciseStartTimeText->sizePolicy().hasHeightForWidth());
        PreciseStartTimeText->setSizePolicy(sizePolicy);
        PreciseStartTimeText->setMinimumSize(QSize(200, 0));
        PreciseStartTimeText->setFont(font1);
        PreciseStartTimeText->setAlignment(
            Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(PreciseStartTimeText);


        gridLayout->addLayout(horizontalLayout_2, 3, 0, 1, 1);

        AcquisitionButton->setText(QCoreApplication::translate(
            "MainWindowWidgets", "Start Acquisition", nullptr));
        RedundantRecordCheckBox->setText(QCoreApplication::translate(
            "MainWindowWidgets", "Redundant Record", nullptr));
        label->setText(QCoreApplication::translate(
            "MainWindowWidgets", "Sample Rate", nullptr));
        DecimalRadioButton->setText(QCoreApplication::translate(
            "MainWindowWidgets", "Decimal", nullptr));
        BinaryRadioButton->setText(QCoreApplication::translate(
            "MainWindowWidgets", "Binary", nullptr));
#ifdef WITH_TABS
        ChannelsTab->setTabText(ChannelsTab->indexOf(tab),
            QCoreApplication::translate("MainWindowWidgets", "Tab 1", nullptr));
        ChannelsTab->setTabText(ChannelsTab->indexOf(tab_2),
            QCoreApplication::translate("MainWindowWidgets", "Tab 2", nullptr));
#endif
        label_2->setText(QCoreApplication::translate(
            "MainWindowWidgets", "Precise Start Time:   ", nullptr));
    }
};
