#include "statusdelegate.h"
#include <QComboBox>

StatusDelegate::StatusDelegate(QObject* parent) :
    QStyledItemDelegate(parent)
{}

QWidget* StatusDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& /*option*/, const QModelIndex& /*index*/) const {

    QComboBox* editor = new QComboBox(parent);
    editor->addItems(m_status);
    return editor;
}

void StatusDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {

    QString value = index.model()->data(index, Qt::EditRole).toString();
    QComboBox* comboBox = static_cast<QComboBox*>(editor);

    int currentIndex = comboBox->findText(value);
    if(currentIndex >= 0) {
        comboBox->setCurrentIndex(currentIndex);
    }
}

void  StatusDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const {

    QComboBox* comboBox = static_cast<QComboBox*>(editor);
    model->setData(index, comboBox->currentText(), Qt::EditRole);
}

void StatusDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& /*index*/) const {

    editor->setGeometry(option.rect);
}
