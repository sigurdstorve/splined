#include <QVBoxLayout>
#include <QTableWidget>
#include <QGroupBox>
#include <QRadioButton>
#include <QFormLayout>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QDebug>
#include <QSignalBlocker>
#include "KnotVectorWidget.hpp"


// TODO: Make it impossible by the user to set t1 < t0

KnotVectorWidget::KnotVectorWidget(QWidget* parent, Qt::WindowFlags f)
    : QWidget(parent, f)
{
    auto v_layout = new QVBoxLayout;

    m_table_widget = new QTableWidget;
    v_layout->addWidget(m_table_widget);

    // horizontal layout holding radio button group and misc. knot-related stuff.
    auto h_layout = new QHBoxLayout;

    // box filled with mutually exclusive radio buttons.
    auto group_box = new QGroupBox("Mutually exclusive choices");
    auto rb_box = new QVBoxLayout;
    auto rb_clamped = new QRadioButton("Clamped");
    rb_clamped->setChecked(true);
    auto rb_open    = new QRadioButton("Open");
    rb_open->setChecked(false);
    auto rb_closed  = new QRadioButton("Closed");
    rb_closed->setChecked(false);
    auto rb_custom  = new QRadioButton("Custom");
    rb_custom->setChecked(false);
    rb_box->addWidget(rb_clamped);
    rb_box->addWidget(rb_open);
    rb_box->addWidget(rb_closed);
    rb_box->addWidget(rb_custom);
    rb_box->addStretch(1);
    group_box->setLayout(rb_box);
    h_layout->addWidget(group_box);

    // create editor for misc.
    auto validator = new QDoubleValidator(-1e6, 1e6, 10, this);
    auto form_layout = new QFormLayout;
    m_t0_editor = new QLineEdit;
    m_t0_editor->setValidator(validator);
    m_t0_editor->setText("0.0"); // HACK
    form_layout->addRow("vis. t_start", m_t0_editor);
    m_t1_editor = new QLineEdit;
    m_t1_editor->setValidator(validator);
    form_layout->addRow("vis. t_end", m_t1_editor);
    m_t1_editor->setText("1.0"); // HACK
    m_auto_knot_min_editor = new QLineEdit;
    m_auto_knot_min_editor->setValidator(validator);
    m_auto_knot_min_editor->setText("0.0"); // HACK
    form_layout->addRow("knots t_start:", m_auto_knot_min_editor);
    m_auto_knot_max_editor = new QLineEdit;
    m_auto_knot_max_editor->setValidator(validator);
    m_auto_knot_max_editor->setText("1.0"); // HACK
    form_layout->addRow("knots t_end:", m_auto_knot_max_editor);
    h_layout->addLayout(form_layout);

    v_layout->addLayout(h_layout);
    setLayout(v_layout);

    // setup signals
    connect(rb_clamped, &QRadioButton::clicked, [&]() {
        emit knot_vector_type_changed(KnotVectorType::CLAMPED);
    });
    connect(rb_open, &QRadioButton::clicked, [&]() {
        emit knot_vector_type_changed(KnotVectorType::OPEN);
    });
    connect (rb_closed, &QRadioButton::clicked, [&]() {
        emit knot_vector_type_changed(KnotVectorType::CLOSED);
    });
    connect (rb_custom, &QRadioButton::clicked, [&]() {
        emit knot_vector_type_changed(KnotVectorType::CUSTOM);
    });
    connect(m_t0_editor, &QLineEdit::returnPressed, [&]() {
        emit_limits();
    });
    connect(m_t1_editor, &QLineEdit::returnPressed, [&]() {
        emit_limits();
    });
    connect(m_auto_knot_min_editor, &QLineEdit::returnPressed, [&]() {
        emit_auto_knot_limits();
    });
    connect(m_auto_knot_max_editor, &QLineEdit::returnPressed, [&]() {
        emit_auto_knot_limits();
    });

    connect(m_table_widget, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(on_knots_manually_edited(QTableWidgetItem*)));
}

void KnotVectorWidget::emit_auto_knot_limits() {
    const auto t0 = m_auto_knot_min_editor->text().toDouble();
    const auto t1 = m_auto_knot_max_editor->text().toDouble();
    emit auto_knot_limits_changed(t0, t1);
}

void KnotVectorWidget::emit_limits() {
    const auto t0 = m_t0_editor->text().toDouble();
    const auto t1 = m_t1_editor->text().toDouble();
    emit eval_interval_changed(t0, t1);
}

void KnotVectorWidget::update_knots(const QVector<qreal>& new_knots) {
    // Needed to avoid on_knots_manually_edited() triggered when elements are
    // given values.
    const QSignalBlocker blocker(m_table_widget);

    const auto num_knots = new_knots.size();
    m_table_widget->setColumnCount(num_knots);
    m_table_widget->setRowCount(1);
    for (int i = 0; i < num_knots; i++) {
        auto item = new QTableWidgetItem(QString("%1").arg(new_knots[i]));
        m_table_widget->setItem(0, i, item);
    }
}

void KnotVectorWidget::on_knots_manually_edited(QTableWidgetItem* item) {
    const auto num_knots = m_table_widget->columnCount();
    QVector<qreal> new_knots(num_knots);
    for (int i = 0; i < num_knots; i++) {
        new_knots[i] = m_table_widget->item(0, i)->text().toDouble();
    }
    emit knot_vector_manually_edited(new_knots);
}
