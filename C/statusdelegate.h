#ifndef STATUSDELEGATE_H
#define STATUSDELEGATE_H

#include <QStringList>
#include <QStyledItemDelegate>

class StatusDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit StatusDelegate(QObject* parent = nullptr);

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    void setEditorData(QWidget* editor, const QModelIndex& index) const override;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
    void updateEditorGeometry(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    const QStringList m_status = {"Підтверджено", "Заселено", "Виписано", "Скасовано", "Очікує"};
};

#endif // STATUSDELEGATE_H
