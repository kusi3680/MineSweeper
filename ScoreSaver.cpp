
#include "ScoreSaver.h"

#include <QCoreApplication>
#include <QSettings>
#include <QInputDialog>
#include <QMessageBox>

#include <algorithm>

/*
 * Using Qsettings to save the score
 * Will help by not keeping a text file which can be editted/corrupted by user
 * https://doc.qt.io/qt-5/qsettings.html#details
 */

namespace
{
    const QString key("score");
}

ScoreSaver::ScoreSaver(QString company, QString application, unsigned count, bool moreThan)
    : company(company),
      application(application),
      count(count),
      greater(moreThan)
{
    QCoreApplication::setOrganizationName(company);
    QCoreApplication::setOrganizationDomain("");
    QCoreApplication::setApplicationName(application);

    QSettings s;
    QString defaultValue("NULLVALUE");
    QString key = "score";

    QVariant score = s.value(key, defaultValue);

    QString scoreString(score.toString());

    if(score != defaultValue){
        QStringList list = scoreString.split(";");
        for (QString item: list) {
            QStringList itemList = item.split("#");
            if(itemList.size() == 2){
                long long points = itemList[0].toLongLong();
                QString name = itemList[1];
                map.push_back(Entry{points, name});
            }
        }
    }
}

void ScoreSaver::setResult(long long points)
{
    auto moreThan= [&](const Entry &e1, const Entry &e2)
    {
        return e1.points > e2.points;
    };
    auto lessThan= [&](const Entry &e1, const Entry &e2)
    {
        return e1.points < e2.points;
    };

    if(greater)
        std::sort(map.begin(), map.end(), moreThan);
    else
        std::sort(map.begin(), map.end(), lessThan);

    if((map.size() < count) || (points > map.rbegin()->points)){
        QString name = QInputDialog::getText(0, "Enter your name", "Name");
        map.push_back({points, name});

        if(greater)
            std::sort(map.begin(), map.end(), moreThan);
        else
            std::sort(map.begin(), map.end(), lessThan);

        QString outputTable;
        QString text;
        for(unsigned i = 0; i < count && i < map.size(); ++i){
            const Entry &e = map[i];
            if(i != 0){
                outputTable += ";";
            }
            outputTable += QString("%1#%2").arg(e.points).arg(e.name);
            text += QString("%1. %2 %3\n").arg(i + 1).arg(e.name).arg(e.points);

        }
        QMessageBox::information(0, "score", text); // using messagebox to show highscores

        QSettings s;
        s.setValue(key, outputTable); // saving settings
    }
}
