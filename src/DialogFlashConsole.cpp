#include "DialogFlashConsole.h"
#include "ui_DialogFlashConsole.h"
#include <QCloseEvent>
#include <QClipboard>
#include <QApplication>
#include <QMessageBox>
#include <QTextBlock>

QRegularExpression DialogFlashConsole::s_cursorUp(QStringLiteral("\x1b\\[(\\d*)A"));
QRegularExpression DialogFlashConsole::s_eraseLine(QStringLiteral("\x1b\\[\\d*K"));

DialogFlashConsole::DialogFlashConsole(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogFlashConsole)
    , m_process(new QProcess(this))
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    // Set monospace font for console
    QFont monoFont("Monospace", 9);
    monoFont.setStyleHint(QFont::TypeWriter);
    ui->consoleOutput->setFont(monoFont);

    // Dark background for console
    QPalette p = ui->consoleOutput->palette();
    p.setColor(QPalette::Base, QColor(30, 30, 30));
    p.setColor(QPalette::Text, QColor(220, 220, 220));
    ui->consoleOutput->setPalette(p);

    connect(m_process, &QProcess::readyReadStandardOutput,
            this, &DialogFlashConsole::onReadyReadStdout);
    connect(m_process, &QProcess::readyReadStandardError,
            this, &DialogFlashConsole::onReadyReadStderr);
    connect(m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &DialogFlashConsole::onProcessFinished);

    connect(ui->buttonCopyLog, &QPushButton::clicked,
            this, &DialogFlashConsole::onCopyLog);
    connect(ui->buttonCancel, &QPushButton::clicked,
            this, &DialogFlashConsole::onCancelClicked);
}

DialogFlashConsole::~DialogFlashConsole()
{
    if (m_process->state() != QProcess::NotRunning)
    {
        m_process->kill();
        m_process->waitForFinished(3000);
    }
    delete ui;
}

void DialogFlashConsole::startFlash(const QString &program, const QStringList &arguments,
                                    const QString &boardName)
{
    QList<QPair<QString, QStringList>> commands;
    commands.append(qMakePair(program, arguments));
    startFlashSequence(commands, boardName);
}

void DialogFlashConsole::startFlashSequence(const QList<QPair<QString, QStringList>> &commands,
                                            const QString &boardName)
{
    setWindowTitle(tr("Flashing %1").arg(boardName));
    m_commands = commands;
    m_currentCommand = 0;
    m_flashing = true;
    m_success = false;

    startNextCommand();
}

void DialogFlashConsole::startNextCommand()
{
    if (m_currentCommand >= m_commands.size())
    {
        // All commands completed successfully
        m_flashing = false;
        m_success = true;
        showBanner(true, tr("Flash successful. The device is rebooting."));
        ui->buttonCancel->setText(tr("Close"));
        emit flashFinished(true);
        return;
    }

    const auto &cmd = m_commands.at(m_currentCommand);
    QString program = cmd.first;
    QStringList args = cmd.second;

    // Print command being executed
    QString cmdLine = program;
    for (const QString &arg : args)
        cmdLine += " " + arg;
    appendOutput("$ " + cmdLine + "\n");

    m_process->setProcessChannelMode(QProcess::SeparateChannels);
    m_process->start(program, args);
    m_process->closeWriteChannel();
}

void DialogFlashConsole::onReadyReadStdout()
{
    QString text = QString::fromUtf8(m_process->readAllStandardOutput());
    appendOutput(text);
}

void DialogFlashConsole::onReadyReadStderr()
{
    QString text = QString::fromUtf8(m_process->readAllStandardError());
    appendOutput(text);
}

void DialogFlashConsole::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitStatus);

    if (exitCode != 0)
    {
        m_flashing = false;
        m_success = false;
        showBanner(false, tr("Flash failed (exit code %1).").arg(exitCode));
        ui->buttonCancel->setText(tr("Close"));
        emit flashFinished(false);
        return;
    }

    // Move to next command in sequence
    m_currentCommand++;
    startNextCommand();
}

void DialogFlashConsole::onCopyLog()
{
    QApplication::clipboard()->setText(ui->consoleOutput->toPlainText());
}

void DialogFlashConsole::onCancelClicked()
{
    if (m_flashing)
    {
        if (confirmCancel())
        {
            m_process->kill();
            m_process->waitForFinished(3000);
            m_flashing = false;
            reject();
        }
    }
    else
    {
        accept();
    }
}

void DialogFlashConsole::closeEvent(QCloseEvent *event)
{
    if (m_flashing)
    {
        if (confirmCancel())
        {
            m_process->kill();
            m_process->waitForFinished(3000);
            m_flashing = false;
            event->accept();
        }
        else
        {
            event->ignore();
        }
    }
    else
    {
        event->accept();
    }
}

void DialogFlashConsole::appendOutput(const QString &text)
{
    // Pre-process cursor movement sequences that QAnsiTextEdit does not handle:
    // \033[nA (cursor up), \033[K / \033[2K (erase line), \r (carriage return)
    // Color codes within text segments are handled by appendAnsiText().

    int pos = 0;
    while (pos < text.size())
    {
        // Find the next cursor-movement sequence or \r
        auto matchUp = s_cursorUp.match(text, pos);
        auto matchErase = s_eraseLine.match(text, pos);
        int crPos = text.indexOf('\r', pos);

        // Determine earliest special position
        int nextSpecial = text.size();
        if (matchUp.hasMatch() && matchUp.capturedStart() < nextSpecial)
            nextSpecial = matchUp.capturedStart();
        if (matchErase.hasMatch() && matchErase.capturedStart() < nextSpecial)
            nextSpecial = matchErase.capturedStart();
        if (crPos >= 0 && crPos < nextSpecial)
            nextSpecial = crPos;

        // Insert any text before the next special sequence (may contain color codes)
        if (nextSpecial > pos)
        {
            QString segment = text.mid(pos, nextSpecial - pos);
            ui->consoleOutput->moveCursor(QTextCursor::End);
            ui->consoleOutput->insertAnsiText(segment);
        }

        if (nextSpecial >= text.size())
            break;

        // Handle the special sequence at nextSpecial
        if (crPos >= 0 && crPos == nextSpecial)
        {
            // \r: erase current line content, subsequent text overwrites it
            eraseCurrentLine();
            pos = crPos + 1;
            // Skip \n right after \r
            if (pos < text.size() && text[pos] == '\n')
                pos++;
        }
        else if (matchUp.hasMatch() && matchUp.capturedStart() == nextSpecial)
        {
            int n = matchUp.captured(1).isEmpty() ? 1 : matchUp.captured(1).toInt();
            if (n < 1) n = 1;
            cursorUp(n);
            pos = matchUp.capturedEnd();
        }
        else if (matchErase.hasMatch() && matchErase.capturedStart() == nextSpecial)
        {
            eraseCurrentLine();
            pos = matchErase.capturedEnd();
        }
        else
        {
            pos = nextSpecial + 1;
        }
    }

    ui->consoleOutput->moveCursor(QTextCursor::End);
}

void DialogFlashConsole::eraseCurrentLine()
{
    QTextCursor cursor = ui->consoleOutput->textCursor();
    cursor.movePosition(QTextCursor::End);
    cursor.movePosition(QTextCursor::StartOfBlock, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
}

void DialogFlashConsole::cursorUp(int n)
{
    QTextCursor cursor = ui->consoleOutput->textCursor();
    cursor.movePosition(QTextCursor::End);
    for (int i = 0;i < n;i++)
    {
        cursor.movePosition(QTextCursor::StartOfBlock, QTextCursor::KeepAnchor);
        if (cursor.position() > 0)
            cursor.movePosition(QTextCursor::PreviousBlock, QTextCursor::KeepAnchor);
        cursor.movePosition(QTextCursor::StartOfBlock, QTextCursor::KeepAnchor);
    }
    cursor.removeSelectedText();
}

void DialogFlashConsole::showBanner(bool success, const QString &message)
{
    ui->labelBanner->setVisible(true);
    ui->labelBanner->setText(message);

    if (success)
    {
        ui->labelBanner->setStyleSheet(
            "QLabel { background-color: #2e7d32; color: white; "
            "padding: 8px; border-radius: 4px; }");
    }
    else
    {
        ui->labelBanner->setStyleSheet(
            "QLabel { background-color: #c62828; color: white; "
            "padding: 8px; border-radius: 4px; }");
    }
}

bool DialogFlashConsole::confirmCancel()
{
    return QMessageBox::warning(this, tr("Cancel Flashing"),
               tr("Flashing is in progress. Stopping now may leave the device "
                  "in an inconsistent state. Are you sure you want to cancel?"),
               QMessageBox::Yes | QMessageBox::No,
               QMessageBox::No) == QMessageBox::Yes;
}
