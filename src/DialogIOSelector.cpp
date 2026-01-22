#include "DialogIOSelector.h"
#include <QLabel>
#include <QHeaderView>
#include <QPushButton>

DialogIOSelector::DialogIOSelector(const QString &currentIoId, QWidget *parent)
    : QDialog(parent)
    , m_currentIoId(currentIoId)
{
    setWindowTitle(tr("Select IO"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setMinimumSize(500, 400);
    resize(600, 500);

    auto *mainLayout = new QVBoxLayout(this);

    // Filter section
    auto *filterLayout = new QHBoxLayout();
    auto *filterLabel = new QLabel(tr("Filter:"), this);
    m_filterEdit = new ClearLineEdit(this);
    m_filterEdit->setPlaceholderText(tr("Search by IO or Room name..."));
    filterLayout->addWidget(filterLabel);
    filterLayout->addWidget(m_filterEdit);
    mainLayout->addLayout(filterLayout);

    // Tree widget
    m_treeWidget = new QTreeWidget(this);
    m_treeWidget->setHeaderLabels({tr("Name"), tr("Type"), tr("ID")});
    m_treeWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    m_treeWidget->setAlternatingRowColors(true);
    m_treeWidget->header()->setStretchLastSection(true);
    m_treeWidget->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    mainLayout->addWidget(m_treeWidget);

    // Button box
    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    m_buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    mainLayout->addWidget(m_buttonBox);

    // Connections
    connect(m_filterEdit, &QLineEdit::textChanged, this, &DialogIOSelector::onFilterTextChanged);
    connect(m_treeWidget, &QTreeWidget::itemSelectionChanged, this, &DialogIOSelector::onItemSelectionChanged);
    connect(m_treeWidget, &QTreeWidget::itemDoubleClicked, this, &DialogIOSelector::onItemDoubleClicked);
    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    // Populate the tree
    populateTree();

    // Preselect if we have a current IO
    if (!m_currentIoId.isEmpty())
    {
        preselectIO(m_currentIoId);
    }

    // Resize columns to content
    m_treeWidget->resizeColumnToContents(1);
    m_treeWidget->resizeColumnToContents(2);
}

DialogIOSelector::~DialogIOSelector()
{
}

void DialogIOSelector::populateTree()
{
    m_treeWidget->setUpdatesEnabled(false);
    m_treeWidget->clear();

    for (int i = 0; i < ListeRoom::Instance().size(); i++)
    {
        Room *room = ListeRoom::Instance().get_room(i);

        // Create room item
        auto *roomItem = new QTreeWidgetItemRoom(room, m_treeWidget);
        roomItem->setText(0, QString::fromUtf8(room->get_name().c_str()));
        roomItem->setText(1, QString::fromUtf8(room->get_type().c_str()));
        roomItem->setFlags(roomItem->flags() & ~Qt::ItemIsSelectable); // Room items not selectable

        // Make room items visually distinct
        QFont boldFont = roomItem->font(0);
        boldFont.setBold(true);
        roomItem->setFont(0, boldFont);

        // Add inputs
        for (int j = 0; j < room->get_size_in(); j++)
        {
            IOBase *io = room->get_input(j);
            auto *ioItem = new QTreeWidgetItemInput(io, roomItem);
            ioItem->setText(0, QString::fromUtf8(io->get_param("name").c_str()));
            ioItem->setText(1, QString::fromUtf8(io->get_gui_type().c_str()));
            ioItem->setText(2, QString::fromUtf8(io->get_param("id").c_str()));

            // Store room reference for easy retrieval
            ioItem->setData(0, Qt::UserRole + 10, QVariant::fromValue(static_cast<void*>(room)));
        }

        // Add outputs
        for (int j = 0; j < room->get_size_out(); j++)
        {
            IOBase *io = room->get_output(j);
            auto *ioItem = new QTreeWidgetItemOutput(io, roomItem);
            ioItem->setText(0, QString::fromUtf8(io->get_param("name").c_str()));
            ioItem->setText(1, QString::fromUtf8(io->get_gui_type().c_str()));
            ioItem->setText(2, QString::fromUtf8(io->get_param("id").c_str()));

            // Store room reference for easy retrieval
            ioItem->setData(0, Qt::UserRole + 10, QVariant::fromValue(static_cast<void*>(room)));
        }

        // Expand room if it has children
        if (roomItem->childCount() > 0)
        {
            roomItem->setExpanded(true);
        }
    }

    m_treeWidget->setUpdatesEnabled(true);
}

void DialogIOSelector::onFilterTextChanged(const QString &text)
{
    applyFilter(text);
}

void DialogIOSelector::applyFilter(const QString &filter)
{
    QString lowerFilter = filter.trimmed().toLower();

    for (int i = 0; i < m_treeWidget->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *roomItem = m_treeWidget->topLevelItem(i);
        bool roomVisible = false;
        bool roomMatches = lowerFilter.isEmpty() ||
                          roomItem->text(0).toLower().contains(lowerFilter);

        for (int j = 0; j < roomItem->childCount(); j++)
        {
            QTreeWidgetItem *ioItem = roomItem->child(j);
            bool ioMatches = lowerFilter.isEmpty() ||
                            ioItem->text(0).toLower().contains(lowerFilter) ||  // Name
                            ioItem->text(1).toLower().contains(lowerFilter) ||  // Type
                            ioItem->text(2).toLower().contains(lowerFilter) ||  // ID
                            roomMatches;  // Room name matches

            ioItem->setHidden(!ioMatches);
            if (ioMatches) roomVisible = true;
        }

        roomItem->setHidden(!roomVisible);
        if (roomVisible) roomItem->setExpanded(true);
    }
}

void DialogIOSelector::preselectIO(const QString &ioId)
{
    std::string idStr = ioId.toStdString();

    for (int i = 0; i < m_treeWidget->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *roomItem = m_treeWidget->topLevelItem(i);

        for (int j = 0; j < roomItem->childCount(); j++)
        {
            QTreeWidgetItem *ioItem = roomItem->child(j);

            // Check if this is the IO we're looking for
            if (ioItem->text(2) == ioId)
            {
                m_treeWidget->setCurrentItem(ioItem);
                m_treeWidget->scrollToItem(ioItem);
                ioItem->setSelected(true);
                return;
            }
        }
    }
}

void DialogIOSelector::onItemSelectionChanged()
{
    QList<QTreeWidgetItem *> selected = m_treeWidget->selectedItems();

    m_selectedIO = nullptr;
    m_selectedRoom = nullptr;

    if (!selected.isEmpty())
    {
        QTreeWidgetItem *item = selected.first();

        // Try as input
        auto *inputItem = dynamic_cast<QTreeWidgetItemInput *>(item);
        if (inputItem)
        {
            m_selectedIO = inputItem->getInput();
            m_selectedRoom = static_cast<Room*>(item->data(0, Qt::UserRole + 10).value<void*>());
        }
        else
        {
            // Try as output
            auto *outputItem = dynamic_cast<QTreeWidgetItemOutput *>(item);
            if (outputItem)
            {
                m_selectedIO = outputItem->getOutput();
                m_selectedRoom = static_cast<Room*>(item->data(0, Qt::UserRole + 10).value<void*>());
            }
        }
    }

    m_buttonBox->button(QDialogButtonBox::Ok)->setEnabled(m_selectedIO != nullptr);
}

void DialogIOSelector::onItemDoubleClicked(QTreeWidgetItem *item, int)
{
    // Only accept double-click on IO items, not room items
    auto *inputItem = dynamic_cast<QTreeWidgetItemInput *>(item);
    auto *outputItem = dynamic_cast<QTreeWidgetItemOutput *>(item);

    if (inputItem || outputItem)
    {
        accept();
    }
}

IOBase *DialogIOSelector::getSelectedIO() const
{
    return m_selectedIO;
}

Room *DialogIOSelector::getSelectedRoom() const
{
    return m_selectedRoom;
}
