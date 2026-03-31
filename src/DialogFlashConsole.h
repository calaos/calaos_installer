#ifndef DIALOGFLASHCONSOLE_H
#define DIALOGFLASHCONSOLE_H

#include <QDialog>
#include <QProcess>
#include <QStringList>
#include <QRegularExpression>

namespace Ui {
class DialogFlashConsole;
}

class DialogFlashConsole : public QDialog
{
    Q_OBJECT

public:
    explicit DialogFlashConsole(QWidget *parent = nullptr);
    ~DialogFlashConsole();

    // Start flashing with a single command
    void startFlash(const QString &program, const QStringList &arguments,
                    const QString &boardName);

    // Start flashing with sequential commands (for Rockchip: firmware + config)
    void startFlashSequence(const QList<QPair<QString, QStringList>> &commands,
                            const QString &boardName);

    bool wasSuccessful() const { return m_success; }

signals:
    void flashFinished(bool success);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onReadyReadStdout();
    void onReadyReadStderr();
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onCopyLog();
    void onCancelClicked();

private:
    void appendOutput(const QString &text);
    void eraseCurrentLine();
    void cursorUp(int n);
    void showBanner(bool success, const QString &message);
    bool confirmCancel();
    void startNextCommand();

    static QRegularExpression s_cursorUp;
    static QRegularExpression s_eraseLine;

    Ui::DialogFlashConsole *ui;
    QProcess *m_process;
    bool m_flashing = false;
    bool m_success = false;

    // For sequential commands
    QList<QPair<QString, QStringList>> m_commands;
    int m_currentCommand = 0;
};

#endif // DIALOGFLASHCONSOLE_H
