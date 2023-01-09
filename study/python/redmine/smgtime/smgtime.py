#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import smtplib
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
from datetime import datetime
from datetime import date
from redminelib import Redmine
import sys

CREDENTIALS_FILE = '../credentials'
MIN_TIME = 35
MIN_TIME_STUD = 16
TASK_HOUR_IDX = 6

users = [
     {"name": "Александр Лещев", "idtask":207556, "uid":195, "plan":list(), "hours":0, "minnorm":MIN_TIME, "email":"alexander.leschev@eltex.loc"}
    # ,{"name": "Михаил Тырышкин", "idtask":206624, "uid":134, "plan":list(), "hours":0, "minnorm":MIN_TIME, "email":"mikhail.tyryshkin@eltex.loc"}
    ,{"name": "Роман Рупп", "idtask":206528, "uid":177, "plan":list(), "hours":0, "minnorm":MIN_TIME, "email":"roman.rupp@eltex.loc"}
    ,{"name": "Артем Щерба", "idtask":207688, "uid":838, "plan":list(), "hours":0, "minnorm":MIN_TIME, "email":"artem.shcherba@eltex.loc"}
    ,{"name": "Владимир Алексеев", "idtask":206849, "uid":489, "plan":list(), "hours":0, "minnorm":MIN_TIME, "email":"vladimir.alekseev@eltex.loc"}
    ,{"name": "Игорь Липовцев", "idtask":198854, "uid":1023, "plan":list(), "hours":0, "minnorm":MIN_TIME, "email":"igor.lipovcev@eltex.loc"}
    ,{"name": "Кристина Михед", "idtask":204253, "uid":662, "plan":list(), "hours":0, "minnorm":MIN_TIME, "email":"kristina.mihed@eltex.loc"}
    ,{"name": "Ксения Михед", "idtask":204895, "uid":661, "plan":list(), "hours":0, "minnorm":MIN_TIME, "email":"kseniya.mihed@eltex.loc"}
    # ,{"name": "Павел Сазонов", "idtask":205436, "uid":879, "plan":list(), "hours":0, "minnorm":MIN_TIME, "email":"pavel.sazonov@eltex.loc"}
    ,{"name": "Владислав Бражников", "idtask":203796, "uid":1178, "plan":list(), "hours":0, "minnorm":MIN_TIME, "email":"vladislav.brazhnikov@eltex.loc"}
    # ,{"name": "Павел Федотов", "idtask":203707, "uid":663, "plan":list(), "hours":0, "minnorm":MIN_TIME, "email":"pavel.fedotov@eltex.loc"}
    ,{"name": "Илья Трухин", "idtask":219603, "uid":1268, "plan":list(), "hours":0, "minnorm":MIN_TIME, "email":"ilya.truhin@eltex.loc"}
    # ,{"name": "Кузьма Казыгашев", "idtask":190033, "uid":327, "plan":list(), "hours":0, "minnorm":MIN_TIME, "email":"kuzma.kazygashev@eltex.loc"}
    # ,{"name": "Артём Карпов", "idtask":195165, "uid":330, "plan":list(), "hours":0, "minnorm":MIN_TIME, "email":"artem.karpov@eltex.loc"}
]

def printHelp():
    print("Usage:")
    print("\t<without> current week\n")
    print("\t--help        this message")
    print("\t--week        time entry from a week")
    print("\t--prev, -p    time entry from a prev week")

def action_info(info):
    ''' Decorator which put 'what doing now' text in console before function
    call and prints 'done' after function ends. This code:

    @action_info('Calculate sum')
    def sum(a,b):
        return a+b

    Will print:

    Calculate sum ... done.
    '''
    def decorator(func):
        def wrapper(*args, **kwargs):
            print(info, '... ', end='')
            result = func(*args, **kwargs)
            print('done.')
            return result
        return wrapper
    return decorator

@action_info('Read Redmine credentials')
def readCredentials(credFile):
    with open(credFile, 'rt') as cf:
        credTuple = cf.readline().strip().split(' ')
    return credTuple

@action_info('Redmine login')
def redmineStart(login, passwd):
    return Redmine('http://red.eltex.loc/', username=login, password=passwd)

@action_info('Read IDs')
def getUserIDs(red):
    for user in users:
        issue = red.issue.get(user['idtask'])
        user["uid"] = issue.assigned_to.id

# @action_info('Format issue')
def formatIssue(issue):
    uissue = list()

    # print(list(issue))

    task = redmine.issue.get(issue.issue.id)

    uissue.append(issue.project.name)

    #time-tracking tasks does not have a version
    if hasattr(task, 'fixed_version'):
        uissue.append(task.fixed_version.name)
    else:
        uissue.append("-")

    uissue.append(issue.activity.name)
    uissue.append(issue.issue.id)
    uissue.append(task.subject)
    uissue.append(issue.comments)
    uissue.append(issue.hours)

    return uissue

def fillPlans():
    for user in users:
        print("Update " + user["name"])
        issues = redmine.time_entry.all(user_id=user["uid"], from_date=date_from, to_date=date_to)
        if len(issues):
            for issue in issues:
                task_list = list()
                task = formatIssue(issue)
                task_list.append(task)
                user["plan"].extend(task_list)
                user["hours"] += task[TASK_HOUR_IDX]

def writeReport():
    cvsf = open("report/time_report_"+date_from_str+"_"+date_to_str+".csv",'w')
    cvsf.write("hours;user;project;version;activity;ref;task;comments;\n")

    print("----------------------------")
    print("----------------------------")
    print("----------------------------")

    for user in users:
        task_count = len(user["plan"])
        if task_count == 0:
            print("----", user["name"])
        else:
            if user["hours"] < user["minnorm"]:
                print("(!) %.1f " % user["hours"], user["minnorm"], user["name"])
            else:
                print("%.1f" % user["hours"], user["minnorm"], user["name"])

            for task in user["plan"]:
                cvsf.write("{:3.2f}".format(task[TASK_HOUR_IDX]) + ";" + user["name"] + ";")
                length = len(task)
                for i in range(length):
                    if i == TASK_HOUR_IDX:
                        continue;
                    else:
                        cvsf.write(str(task[i]) + ";")
                cvsf.write("\n")
        if user["hours"] < user["minnorm"]:
            cvsf.write("{:3.2f}".format(user["hours"]) + " < " + str(user["minnorm"]) + "!")
        else:
            cvsf.write("{:3.2f}".format(user["hours"]))
        cvsf.write(";")
        cvsf.write(user["name"] + ";TOTAL;TOTAL;TOTAL;TOTAL;TOTAL;TOTAL;\n")
        cvsf.write(" ; ; ; ; ; ; ; ;\n")
    cvsf.close()

def date_from_isoweek(iso_year, iso_weeknumber, iso_weekday):
    return datetime.strptime(
        '{:04d} {:02d} {:d}'.format(iso_year, iso_weeknumber, iso_weekday),
        '%G %V %u').date()

def sendMailNotify(login, passwd):
    try: 
        smtp = smtplib.SMTP('mail.eltex.loc', 587)
        smtp.starttls()
        smtp.login(login + "@eltex.loc", passwd)

        for user in users:
            html_templ = """\
            <html>
              <p style="color:gray;">Сообщение сгенерировано автоматически</p>
              <h1 style="color: red;">Не проставлены трудозатраты за неделю</h1>
              <body>
                <p>Вы получили это сообщение потому, что трудозатраты по задачам за период """ + date_from_str + " - " + date_to_str + """ меньше """ + str(user["minnorm"]) + """ часов.<br>
                   Проставьте трудозатраты по задачам.<br><br>
                   Ваши текущие трудозатраты за период """ + date_from_str + " - " + date_to_str + """<br>"""

            # html_templ = """\
            # <html>
            #   <p style="color:gray;">Сообщение сгенерировано автоматически</p>
            #   <h1 style="color: red;">Очередной конец месяца в середине недели. </h1>
            #   <h1">До конца дня нужно проставить актуальные трудозатраты на текущую дату. </h1>
            #   <h1">На сегодня, за эту неделю, должно быть в районе """ + str(MIN_TIME) + """ч. </h1>
            #   <body>
            #     <p>Вы получили это сообщение потому, что трудозатраты по задачам за период """ + date_from_str + " - " + date_to_str + """ меньше """ + str(user["minnorm"]) + """ часов.<br>
            #        Проставьте трудозатраты по задачам.<br><br>
            #        Ваши текущие трудозатраты за период """ + date_from_str + " - " + date_to_str + """<br>"""

            if user["hours"] < user["minnorm"]:
                msg = MIMEMultipart()
                msg['From'] = login + "@eltex.loc"
                msg['Subject'] = "[" + date_from_str + " - " + date_to_str + "] Трудозатраты за неделю"
                # msg['Subject'] = "[" + date_from_str + " - " + date_to_str + "][Конец месяца] Трудозатраты за неделю"

                task_count = len(user["plan"])

                html = html_templ

                html += """\
                    Задачи:  """ + str(task_count) + """<br>"""

                for task in user["plan"]:
                    html += "[" + "{:3.2f}".format(task[TASK_HOUR_IDX]) + "ч] "
                    html += "<a href=http://red.eltex.loc/issues/" + str(task[3]) + "> " + str(task[3]) + "</a> "  + str(task[0]) + " ("+str(task[1])+") " + str(task[4])
                    if len(task[5]) > 0:
                        html += " (" + task[5] + ")"
                    html += "<br>"

                html += "<br>Всего: " + "{:3.2f}".format(user["hours"]) + "ч</p>"

                html += """<hr style="width:50%;text-align:left;margin-left:0">"""
                html += """P.S. Если вы получили это сообщение по ошибке - свяжитесь со мной в ответном сообщении.</p>
                  </body>
                </html>
                """

                msg.attach(MIMEText(html, 'html'))
                msg['To'] = user['email']
                print("Send notify to '" + user['email'] + "' [" + "{:3.2f}".format(user["hours"]) + "]")
                smtp.sendmail( login + "@eltex.loc", user['email'], msg.as_string())

        smtp.quit()

    except Exception as ex: 
        print("Something went wrong....",ex)

#
#

iso = datetime.today().isocalendar()
date_from = date_from_isoweek(iso[0], iso[1], 1)
date_to = date_from_isoweek(iso[0], iso[1], 7)
mail_notify = str()

for idx, arg in enumerate(sys.argv):
    if arg == "--help":
        printHelp()
        sys.exit()
    if arg == "--week":
        date_from = date_from_isoweek(iso[0], int(sys.argv[idx+1]), 1)
        date_to = date_from_isoweek(iso[0], int(sys.argv[idx+1]), 7)
    if arg == "--prev" or arg == "-p":
        date_from = date_from_isoweek(iso[0], iso[1] - 1, 1)
        date_to = date_from_isoweek(iso[0], iso[1] - 1, 7)
    if arg == "--mail":
        mail_notify = "yes"
    if arg == "--time":
        MIN_TIME = int(sys.argv[idx+1])

date_from_str = date_from.strftime("%d.%m.%Y")
date_to_str = date_to.strftime("%d.%m.%Y")

print("\ntime report by - [", date_from_str, " - ", date_to_str, "] [<" + str(MIN_TIME) + "]\n")

redLogin, redPassword = readCredentials(CREDENTIALS_FILE)
redmine = redmineStart(redLogin, redPassword)


#getUserIDs(redmine)
fillPlans()
writeReport()


if len(mail_notify) > 0:
    sendMailNotify(redLogin, redPassword)
else:
    print("\nUse '--mail' for e-mail notification")
