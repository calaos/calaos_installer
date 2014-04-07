#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QByteArray>
#include <QTemporaryFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QDomDocument>
#include <QtWidgets>
#include <vector>
#include <QMessageBox>

#include <ListeRoom.h>
#include <ListeRule.h>

using namespace Calaos;
using namespace std;

class IOXmlReader: public QXmlStreamReader
{
        public:
                IOXmlReader();

                bool readFile(QIODevice *device);

        private:
                void readHome();
                void readRoom();
                void readInput(Room *room);
                void readOutput(Room *room);
                void readInternal(Room *room);
                void readCamera(Room *room);
                void readAudio(Room *room);
                void readPlageDay(vector<Horaire> &day);
                void readPlage(Horaire &horaire);
};

class IOXmlWriter: public QXmlStreamWriter
{
        public:
                IOXmlWriter();

                bool writeFile(QIODevice *device);

        private:
                void writeRoom(Room *room);
                void writeInput(IOBase *io);
                void writePlages(vector<Horaire> &day);
                void writeOutput(IOBase *io);
};

class RuleXmlWriter: public QXmlStreamWriter
{
        public:
                RuleXmlWriter();

                bool writeFile(QIODevice *device);

        private:
                void writeRule(Rule *rule);
                void writeCondition(Rule *rule);
                void writeAction(Rule *rule);
};

class ProjectManager
{
        public:

                /*
                  Save ios to a temporary file.
                  file will be filled with the file name
                  returns true if succeded, false otherwise
                */
                static bool saveIOsToFile(QString &file);
                /* save rules the same way as ios */
                static bool saveRulesToFile(QString &file);

                /* Parse and fill ListeRoom/ListeRule from xml files */
                static bool loadIOsFromFile(QString &file);
                static bool loadRulesFromFile(QString &file);

                //Used to know wich wago is 849 or 842.
                static map<string, bool> wagoTypeCache;
};

#endif // PROJECTMANAGER_H
