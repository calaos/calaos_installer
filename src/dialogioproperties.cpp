#include "dialogioproperties.h"
#include "ui_dialogioproperties.h"

DialogIOProperties::DialogIOProperties(const Params &p, int t, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::DialogIOProperties),
        params(p),
        type(t),
        current_item(NULL),
        modified(false)
{
        ui->setupUi(this);

        QStringList headers;
        headers << QString::fromUtf8("Propriétés") << QString::fromUtf8("Valeur");
        ui->treeProperties->setHeaderLabels(headers);

        for (int i = 0;i < params.size();i++)
        {
                string key, value;
                params.get_item(i, key, value);

                QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeProperties);
                item->setData(0, Qt::DisplayRole, QString::fromUtf8(key.c_str()));
                item->setData(1, Qt::DisplayRole, QString::fromUtf8(value.c_str()));

                item->setData(0, Qt::DecorationRole, QIcon(":/img/document-properties.png"));

                if (i == 0)
                        item->setSelected(true);
        }

        if (type == OBJ_ROOM || type == OBJ_RULE)
        {
                ui->addButton->setDisabled(true);
                ui->delButton->setDisabled(true);
        }

        ui->treeProperties->resizeColumnToContents(0);
        ui->treeProperties->resizeColumnToContents(1);
}

DialogIOProperties::~DialogIOProperties()
{
        delete ui;
}

void DialogIOProperties::changeEvent(QEvent *e)
{
        QDialog::changeEvent(e);
        switch (e->type())
        {
          case QEvent::LanguageChange:
                ui->retranslateUi(this);
                break;
          default:
                break;
        }
}

void DialogIOProperties::on_addButton_clicked()
{
        bool ok;
        QString text = QInputDialog::getText(this, QString::fromUtf8("Nouveau paramètre"),
                                          QString::fromUtf8("Entrez un nom de paramètre"), QLineEdit::Normal,
                                          QString(), &ok);

        if (ok && !text.isEmpty())
        {
                string key = text.toUtf8().data();

                if (params.Exists(key))
                {
                        QMessageBox::warning(this, tr("Calaos Installer"), QString::fromUtf8("Ce paramètre existe déjà !"));

                        return;
                }

                params.Add(key, "");

                QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeProperties);
                item->setData(0, Qt::DisplayRole, QString::fromUtf8(key.c_str()));
                item->setData(1, Qt::DisplayRole, QString());
                item->setData(0, Qt::DecorationRole, QIcon(":/img/document-properties.png"));
                ui->treeProperties->setCurrentItem(item);

                on_modifyButton_clicked();

                modified = true;
        }
}

void DialogIOProperties::on_delButton_clicked()
{
        if (current_item)
        {
                string key = current_item->text(0).toUtf8().data();

                if (key == "type" || key == "name")
                {
                        QMessageBox::warning(this, tr("Calaos Installer"), QString::fromUtf8("Ce paramètre ne peut pas être supprimé !"));
                        return;
                }

                params.Delete(key);
                delete current_item;

                modified = true;
        }
}

void DialogIOProperties::on_modifyButton_clicked()
{
        if (!current_item) return;

        string key, value;

        key = current_item->text(0).toUtf8().data();
        value = params[key];

        //TODO: for now user can't change an object type. In the future being able to
        //      change type on the fly could be great.
        if (key == "type" && type != OBJ_RULE)
        {
                QMessageBox::warning(this, tr("Calaos Installer"), QString::fromUtf8("Ce paramètre ne peut pas être modifié !"));
                return;
        }

        bool ok;
        QString text = QInputDialog::getText(this, QString::fromUtf8("Modifier la valeur"),
                                          QString::fromUtf8("Modifier le paramètre: \"%1\"").arg(QString::fromUtf8(key.c_str())), QLineEdit::Normal,
                                          QString::fromUtf8(value.c_str()), &ok);
        if (ok && !text.isEmpty())
        {
                params.Add(key, text.toUtf8().data());
                current_item->setData(1, Qt::DisplayRole, text);

                modified = true;
        }
}

void DialogIOProperties::on_treeProperties_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem*)
{
        current_item = current;
}

void DialogIOProperties::on_buttonBox_accepted()
{
        if (modified && type == OBJ_ROOM)
        {
                Room *room = ListeRoom::Instance().searchRoomByName(params["name"], params["type"]);
                if (room && to_string(room->get_hits()) == params["hits"])
                {
                        QMessageBox::warning(this, tr("Calaos Installer"), QString::fromUtf8("Cette pièce existe déjà !"));
                        return;
                }
        }

        accept();
}

void DialogIOProperties::on_treeProperties_itemDoubleClicked(QTreeWidgetItem *, int)
{
        on_modifyButton_clicked();
}
