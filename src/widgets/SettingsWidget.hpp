#pragma once
#include <QWidget>

class QFormLayout;
class QCheckBox;

class SettingsWidget : public QWidget {
Q_OBJECT
public:
   SettingsWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

   // Returns status of "join ends" checkbox
   bool get_join_ends() const;

protected:
   QFormLayout*     m_form_layout;
   QCheckBox*       m_checkbox;
};