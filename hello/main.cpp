#include "mainwindow.h"

#include <QApplication>
#include <QProcess>
#include <QLabel>
#include <QPlainTextEdit>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Create the main window
    QMainWindow window;
    window.setWindowTitle("QProcess Example");

    // Create a QLabel to display the output
    //QLabel *outputLabel = new QLabel("Output will be displayed here.");
    QPlainTextEdit *textEdit = new QPlainTextEdit("here we go...");
    textEdit->setReadOnly(true);

    // Create a QProcess to run the "ls" command
    QProcess process;
    process.start("sh", QStringList() << "-c" << "hyprctl clients -j");
    process.waitForFinished(-1);
    QByteArray output = process.readAllStandardOutput();
    //outputLabel->setText(output);

    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(output, &parseError);

    if (jsonDoc.isNull()) {
        qDebug() << "Error parsing JSON:" << parseError.errorString();
        return 1;
    }

    // Check if the top-level structure is an array
    if (jsonDoc.isArray()) {
        QJsonArray jsonArray = jsonDoc.array();

        // Iterate through the array
        for (const QJsonValue &jsonValue : jsonArray) {
            // Check if the array element is an object
            if (jsonValue.isObject()) {
                QJsonObject jsonObject = jsonValue.toObject();
                QJsonObject wspObject = jsonObject.value("workspace").toObject();

                textEdit->appendPlainText(wspObject.value("id").toVariant().toString());
                textEdit->appendPlainText(jsonObject.value("address").toVariant().toString());
                textEdit->appendPlainText(jsonObject.value("class").toVariant().toString());
                textEdit->appendPlainText(jsonObject.value("title").toVariant().toString());

                //qDebug() << wspObject.value("id");
                //qDebug() << jsonObject.value("address");
                //qDebug() << jsonObject.value("class");
                //qDebug() << jsonObject.value("title");

                // Iterate through the keys and values of the object
                //qDebug() << "JSON Object:";
                //for (const QString &key : jsonObject.keys()) {
                //    qDebug() << key << ":" << jsonObject.value(key).toVariant();
                //}
            } else {
                qDebug() << "Array element is not a JSON object.";
            }
        }
    } else {
        qDebug() << "Top-level structure is not a JSON array.";
        return 1;
    }

    // Set the QLabel as the central widget of the QMainWindow
    window.setCentralWidget(textEdit);

    // Show the main window
    window.show();

    // Run the application event loop
    return app.exec();
}
