#include "notes.h"
#include "ui_notes.h"
#include "mainwindow.h"

Notes::Notes(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Notes)
{
    ui->setupUi(this);

    // Buttons
    connect(ui->saveButton, &QPushButton::clicked, [this] {
        ui->notesText->setReadOnly(true);
        ui->editButton->setEnabled(true);
        ui->saveButton->setEnabled(false);
        emit dataChanged();
    });
    connect(ui->undoButton, &QPushButton::clicked, [this] {
        ui->notesText->undo();
    });
    connect(ui->editButton, &QPushButton::clicked, [this] {
        ui->notesText->setReadOnly(false);
        ui->editButton->setEnabled(false);
        ui->saveButton->setEnabled(true);
    });
}

void Notes::displayMinutesSpent(int& minutesSpent)
{
    const QString text = "Minutes spent on task: " + QString::number(minutesSpent);
    ui->timeSpentLabel->setText(text);
}

/*
 * Serialization and destructor
*/

QVariant Notes::toVariant() const
{
    QVariantMap map;
    map.insert("text", ui->notesText->document()->toPlainText());
    return map;
}

void Notes::fromVariant(const QVariant& variant)
{
    QVariantMap map = variant.toMap();
    QString text = QString(map.value("text").toString());
    QTextDocument *document = new QTextDocument();
    document->setPlainText(text);
    ui->notesText->setDocument(document);
}

Notes::~Notes()
{
    delete ui;
}
