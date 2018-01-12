#include "task.h"
#include "ui_task.h"

Task::Task(const QString& name, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Task),
    workMenu(new Work),
    notes(new Notes)
{
    ui->setupUi(this);
    setName(name);


    // Buttons
    connect(ui->editButton, &QPushButton::clicked, this, &Task::rename);
    connect(ui->checkbox, &QCheckBox::clicked, this, &Task::checked);
    connect(ui->workButton, &QPushButton::clicked, workMenu, &Work::show);

    connect(ui->removeButton, &QPushButton::clicked, [this] {
        emit removed(this);
    });
    connect(ui->notesButton, &QPushButton::clicked, [this] {
        updateTimeSpent();
        notes->show();
    });
    connect(ui->upButton, &QPushButton::clicked, [this] {
        emit taskIndexChanged(this, -1);
    });
    connect(ui->downButton, &QPushButton::clicked, [this] {
        emit taskIndexChanged(this, 1);
    });

    // Signals
    connect(workMenu, &Work::dataChanged, this, &Task::workOrNotesDataChanged);
    connect(notes, &Notes::dataChanged, this, &Task::workOrNotesDataChanged);
    connect(workMenu, &Work::completed, this, &Task::completed);
}

/*
 * Getters (and statistics update)
*/

QString Task::name() const
{
    return ui->checkbox->text();
}

bool Task::isCompleted() const
{
    return ui->checkbox->isChecked();
}

void Task::updateTimeSpent()
{
    int minutesSpent = workMenu->minutesSpent();
    notes->displayMinutesSpent(minutesSpent);

}

/*
 * Functions that change data
*/

void Task::setName(const QString& name)
{
    ui->checkbox->setText(name);
    workMenu->setWindowTitle("Work: " + name);
    notes->setWindowTitle("Notes: " + name);
    emit dataChanged();
}


void Task::checked(bool checked)
{
    QFont font (ui->checkbox->font());
    font.setStrikeOut(checked);
    ui->checkbox->setFont(font);
    ui->workButton->setEnabled(false);
    ui->editButton->setEnabled(false);
    ui->checkbox->setEnabled(false);
    emit statusChanged();
    emit dataChanged();
}

/*
 * Slots
*/

void Task::rename()
{
    bool ok;
    QString value = QInputDialog::getText(this, tr("Edit task"),
                                          tr("Task name:"),
                                          QLineEdit::Normal,
                                          this->name(), &ok);
    if (ok && !value.isEmpty()) {
        setName(value);
    }
}

void Task::completed()
{
    ui->checkbox->click();
}

void Task::workOrNotesDataChanged()
{
    emit dataChanged();
}

/*
 * Serialization and destructor
*/

QVariant Task::toVariant() const
{
    QVariantMap map;
    map.insert("name", name());
    map.insert("isCompleted", isCompleted());
    map.insert("notes", notes->toVariant());
    map.insert("work", workMenu->toVariant());

    return map;
}

void Task::fromVariant(const QVariant& variant)
{
    QVariantMap map = variant.toMap();
    setName(QString(map.value("name").toString()));

    const QVariant &notesData = map.value("notes");
    notes -> fromVariant(notesData);

    const QVariant &workData = map.value("work");
    workMenu -> fromVariant(workData);

    if(map.value("isCompleted").toBool()) {
        ui->checkbox->click();
    }
}

Task::~Task()
{
    delete ui;
    delete workMenu;
    delete notes;
}
