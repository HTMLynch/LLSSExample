#pragma once

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets/QButtonGroup>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

//#define WITH_SAMPLE_RATE

class ChannelConfigurationDialog : public QDialog
{
    Q_OBJECT
    
    public:
        explicit ChannelConfigurationDialog(std::string, const json&,
            const json&, QWidget* = nullptr);
    
        json    m_pChannelConfigJSON;

        json& ConfigJSON(void) { return m_jConfigJSON; }
    
    private slots:
        void enableBridgeWidgetsSlot();
        void enableTriggerWidgetsSlot();

#ifdef WITH_SAMPLE_RATE
        void sampleRateChangeSlot();
#endif
        void rangeChangeSlot();

        void bridgeResistance350Slot();
        void bridgeResistance120Slot();
        void bridgeTypeQuarterSlot();
        void bridgeTypeHalfSlot();
        void bridgeTypeFullSlot();

        void excitationSlot();

        void triggerModeOneShotSlot();
        void triggerModeNormalSlot();
        void triggerTypeSlot();
        void triggerLevelSlot();
        void triggerHysteresisSlot();

        void filterSlot();

        void reject() { m_jConfigJSON.clear(); QDialog::reject(); }
        void accept() { QDialog::accept(); }
    
    private:
        QDialogButtonBox *buttonBox;
        QLabel *ChannelName;
        QWidget *verticalLayoutWidget_4;
        QVBoxLayout *verticalLayout_4;
        QHBoxLayout *horizontalLayout_3;
        QLabel *label_3;
        QComboBox *Coupling;
        QHBoxLayout *horizontalLayout_4;
        QLabel *label_4;
        QComboBox *Range;
        QHBoxLayout *horizontalLayout_5;
        QLabel *label_5;
        QComboBox *Filter;
        QFrame *line_4;
        QLabel *label_2;
        QHBoxLayout *horizontalLayout;
        QGroupBox *groupBox;
        QWidget *verticalLayoutWidget;
        QVBoxLayout *verticalLayout;
        QRadioButton *QuarterBridge;
        QRadioButton *HalfBridge;
        QRadioButton *FullBridge;
        QSpacerItem *verticalSpacer;
        QGroupBox *groupBox_2;
        QWidget *verticalLayoutWidget_2;
        QVBoxLayout *verticalLayout_2;
        QRadioButton *Bridge120;
        QRadioButton *Bridge350;
        QVBoxLayout *verticalLayout_3;
        QVBoxLayout *verticalLayout_5;
        QFrame *line_3;
        QLabel *label;
        QHBoxLayout *horizontalLayout_2;
        QCheckBox *ExcitationEnable;
        QComboBox *ExcitationLevel;
        QWidget *verticalLayoutWidget_6;
        QVBoxLayout *verticalLayout_6;
        QLabel *label_9;
        QFrame *line_2;
        QHBoxLayout *horizontalLayout_7;
        QLabel *label_6;
        QComboBox *TriggerType;
        QFrame *line;
        QHBoxLayout *horizontalLayout_6;
        QGroupBox *groupBox_3;
        QWidget *verticalLayoutWidget_3;
        QVBoxLayout *verticalLayout_7;
        QRadioButton *TriggerModeOneShot;
        QRadioButton *TriggerModeNormal;
        QVBoxLayout *verticalLayout_8;
        QLabel *label_7;
        QSlider *TriggerLevel;
        QLabel *label_8;
        QSlider *TriggerHysteresis;
        QWidget *formLayoutWidget;
        QFormLayout *formLayout;
        QLabel *label_10;
#ifdef WITH_SAMPLE_RATE
        QComboBox *SampleRate;
#endif
        QHBoxLayout     *excitationTypeLayout;
        QLabel          *excitationTypeLabel;
        QRadioButton    *ExcitationUnipolar;
        QRadioButton    *ExcitationBipolar;
    
        const json& m_jValidConfigJSON;

        json        m_jConfigJSON;
};
