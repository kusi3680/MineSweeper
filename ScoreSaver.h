
#ifndef SCORESAVER_H
#define SCORESAVER_H

#include <QString>
#include <vector>

struct Entry
{
    long long points;
    QString name;
};

class ScoreSaver
{
    std::vector<Entry> map;
    QString company;
    QString application;
    const unsigned count;
    const bool greater;
public:
    ScoreSaver(QString company, QString application, unsigned count=5, bool greater = true);
    void setResult(long long points);
};

#endif // SCORESAVER_H
