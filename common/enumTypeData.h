#ifndef ENUMTYPEDATA_H
#define ENUMTYPEDATA_H

#include <QObject>
#include <QString>

enum class AppMessageType{
    NOMESSAGECODE,
    WARNINGCODE,
    ERRORCODE,
    EXCELEXCEPTION,
    QUESTIONCODE,
    SAVETOBASE
};

struct StageData
{
  QString time = "";
  QString punkt = "";
  QString text = "";
  
//  StageData(QString t, QString p, QString txt){
//        time = t;
//        punkt = p;
//        text = txt;
//    }

  void setStageData(QString t, QString p, QString txt){
      time = t;
      punkt = p;
      text = txt;
  }
};

struct UserData{
    int userID = 0;
    QString name="";

    void setUserData(int id, QString n){
        userID = id;
        name = n;
    }

};


Q_DECLARE_METATYPE(AppMessageType);
Q_DECLARE_METATYPE(StageData);
Q_DECLARE_METATYPE(UserData);


#endif // ENUMTYPEDATA_H
