#include "SettingsWidget.hpp"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QCheckBox>
#include <QLineEdit>

SettingsWidget::SettingsWidget(QWidget* parent, Qt::WindowFlags f) 
    : QWidget(parent, f)
{
    auto v_layout = new QVBoxLayout;
    m_form_layout = new QFormLayout;

    m_checkbox = new QCheckBox;
    m_checkbox->setChecked(false);
    m_form_layout->addRow("Join ends", m_checkbox);

    v_layout->addLayout(m_form_layout);
    setLayout(v_layout);
}

bool SettingsWidget::get_join_ends() const {
    return m_checkbox->isChecked();
}
