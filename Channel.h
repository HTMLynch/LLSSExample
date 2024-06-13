#pragma once
#include <iostream>

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

#include  <nlohmann/json.hpp>

using json = nlohmann::json;

QT_BEGIN_NAMESPACE
class QLabel;
class QCheckBox;
class QVBoxLayout;
class QFrame;
class QGridLayout;
class QHBoxLayout;
class QSpacerItem;
class QPushButton;
QT_END_NAMESPACE

class Channel : public QWidget
{
    Q_OBJECT

public:
    Channel(std::string, const json&, QWidget *parent = nullptr);

    void EnableConfigButton(bool bEnable) {
        ConfigureButton->setEnabled(bEnable);
    }

    bool Subscribed(void) { return SubscribeCheckBox->isChecked(); }

    void AccumulateSamples(int);

    QPushButton *ConfigureButton;
    QCheckBox *SubscribeCheckBox;
    QLabel *IdText;
    QLabel *FSTSText;
    QLabel *OffsetText;
    QLabel *ValueText;
    QLabel *TotalSamplesText;

    void ResetSampleCount(void);

    const json& CurrentConfiguration(void) {
        return m_CurrentConfigurationJSON;
    }
    void CurrentConfiguration(const json& j) { m_CurrentConfigurationJSON = j; }

    void UpdateConfiguration(const json& j) {
//std::cerr << "Update\n" << m_CurrentConfiguratonJSON.dump(4) << std::endl;
//std::cerr << "With\n" << j.dump(4) << std::endl;
        json    jCurrentFlat = m_CurrentConfigurationJSON.flatten();
        json    jUpdateFlat = j.flatten();

        for(auto& [ key, value ] : jUpdateFlat.items()) {
            std::string sKey(key);
            if(jCurrentFlat.contains(sKey)) jCurrentFlat[sKey] = value;
        }
        m_CurrentConfigurationJSON = jCurrentFlat.unflatten();
//std::cerr << "Updated\n" << m_CurrentConfiguratonJSON.dump(4) << std::endl;
    }

private slots:

private:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_6;
    QFrame *line;
    QLabel *ChannelNameText;
    QGridLayout *gridLayout;
    QLabel *label_12;
    QLabel *label_9;
    QLabel *label_11;
    QLabel *label_10;
    QLabel *label_mine;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_5;
    QFrame *line_2;

    int m_nTotalSamples;

    json    m_CurrentConfigurationJSON;
};
