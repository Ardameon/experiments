#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import smtplib
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
from datetime import datetime
from datetime import date
from redminelib import Redmine
import sys
from prettytable import PrettyTable

CREDENTIALS_FILE = '../credentials'

users = [
     {"name": "Александр Лещев", "idtask":207556, "uid":195, "plan":list(), "email":"alexander.leschev@eltex.loc"}
    # ,{"name": "Михаил Тырышкин", "idtask":206624, "uid":134, "plan":list(), "email":"mikhail.tyryshkin@eltex.loc"}
    ,{"name": "Роман Рупп", "idtask":206528, "uid":177, "plan":list(), "email":"roman.rupp@eltex.loc"}
    ,{"name": "Артем Щерба", "idtask":207688, "uid":838, "plan":list(), "email":"artem.shcherba@eltex.loc"}
    ,{"name": "Владимир Алексеев", "idtask":206849, "uid":489, "plan":list(), "email":"vladimir.alekseev@eltex.loc"}
    ,{"name": "Игорь Липовцев", "idtask":198854, "uid":1023, "plan":list(), "email":"igor.lipovcev@eltex.loc"}
    ,{"name": "Кристина Михед", "idtask":204253, "uid":662, "plan":list(), "email":"kristina.mihed@eltex.loc"}
    ,{"name": "Ксения Михед", "idtask":204895, "uid":661, "plan":list(), "email":"kseniya.mihed@eltex.loc"}
    # ,{"name": "Павел Сазонов", "idtask":205436, "uid":879, "plan":list(), "email":"pavel.sazonov@eltex.loc"}
    ,{"name": "Владислав Бражников", "idtask":203796, "uid":1178, "plan":list(), "email":"vladislav.brazhnikov@eltex.loc"}
    # ,{"name": "Павел Федотов", "idtask":203707, "uid":663, "plan":list(), "email":"pavel.fedotov@eltex.loc"}
    ,{"name": "Илья Трухин", "idtask":219603, "uid":1268, "plan":list(), "email":"ilya.truhin@eltex.loc"}
    # ,{"name": "Кузьма Казыгашев", "idtask":190033, "uid":327, "plan":list(), "email":"kuzma.kazygashev@eltex.loc"}
    # ,{"name": "Артём Карпов", "idtask":195165, "uid":330, "plan":list(), "email":"artem.karpov@eltex.loc"}
]

version = {}

def printHelp():
    print("Usage:")
    print("\t<without> current month, week\n")
    print("\t--help    this message")
    print("\t--month   month for tag")
    print("\t--week    week for tag")

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

def add_version(ver):
    if ver in version.keys():
        version[ver] += 1;
        return
    else:
        version[ver] = {}

    version[ver] = 1

# @action_info('Format issue')
def formatIssue(issue):
    uissue = list()
    hours = 0;

    uissue.append(issue.project.name)
    uissue.append(issue.fixed_version.name)
    uissue.append(issue.id)
    uissue.append(issue.status.name)
    uissue.append(issue.subject)
    uissue.append(issue.custom_fields[1].value.split())

    time_entry = redmine.time_entry.filter(issue_id=issue.id)
    if len(time_entry):
        for time_e in time_entry:
            hours += time_e.hours

    uissue.append(hours)
    uissue.append(issue.priority.name)

    add_version(issue.fixed_version.name);

    return uissue

def fillPlans():
    for user in users:
        print("Update " + user["name"])
        issues = redmine.issue.filter(project_id='voip-smg', assigned_to_id=user["uid"], status_id=2) #In Progress
        if len(issues):
            for issue in issues:
                task_list = list()
                tags = issue.custom_fields[1].value.split()
                task_list.append(formatIssue(issue))
                user["plan"].extend(task_list)

def writeReport():
    cvsf = open("report/in_progress_"+date_from_str+"_"+date_to_str+".csv",'w')
    cvsf.write("user;project;version;ref;status;subject;tags;\n")

    print("----------------------------")
    print("----------------------------")
    print("----------------------------")

    for user in users:
        printTrable = PrettyTable(['Project', 'Version', 'Ref', 'Status', 'Prio', 'Time', 'Subject', 'Tags'])
        printTrable._min_width = {"Project":11, "Status":11, "Version":29, "Time":6}
        printTrable._max_width = {"Subject":70, "Version":29}
        printTrable.align = "l"

        task_count = len(user["plan"])
        if task_count == 0:
            print("--", user["name"])
            cvsf.write(user["name"] + ";-;;-;-;-;-;-;\n")
        else:
            print("%02i" % task_count, user["name"])
            for task in user["plan"]:

                printTrable.add_row([task[0], task[1], task[2], task[3], task[7], "{:3.2f}".format(task[6]), task[4], task[5]])

                cvsf.write(user["name"] + ";")
                [cvsf.write(str(i) + ";") for i in task]
                cvsf.write(";\n")
            print(printTrable)
            print("\n")
    cvsf.close()

    print("\n")

    versionTrable = PrettyTable(['Версия', 'Кол-во задач'])
    versionTrable.align = "l"
    for pver in version:
        versionTrable.add_row([pver, version[pver]])
    print(versionTrable)

def date_from_isoweek(iso_year, iso_weeknumber, iso_weekday):
    return datetime.strptime(
        '{:04d} {:02d} {:d}'.format(iso_year, iso_weeknumber, iso_weekday),
        '%G %V %u').date()

def sendMailNotify(login, passwd):
    try: 
        smtp = smtplib.SMTP('mail.eltex.loc', 587)
        smtp.starttls()
        smtp.login(login + "@eltex.loc", passwd)

        msg = MIMEMultipart()
        msg['From'] = login + "@eltex.loc"
        msg['Subject'] = "[" + date_from_str + " - " + date_to_str + "] Нет задач в работе"
        html = """\
        <html>
          <p style="color:gray;">Сообщение сгенерировано автоматически</p>
          <h1 style="color: red;">Нет задач в 'In Progress'</h1>
          <body>
            <p>Вы получили это сообщение потому, что нет ни в одной задачи в состоянии 'In Progress'<br></p>"""
        html += """<hr style="width:50%;text-align:left;margin-left:0">"""
        html += """<p>P.S. Если вы получили это сообщение по ошибке - свяжитесь со мной в ответном сообщении</p>
          </body>
        </html>
        """

        msg.attach(MIMEText(html, 'html'))

        for user in users:
            if len(user["plan"]) == 0:
                msg['To'] = user['email']
                smtp.sendmail(login + "@eltex.loc", user['email'], msg.as_string())
                print("Send notify to '" + user['email'] + "'")

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
    if arg == "--mail":
        mail_notify = "yes"

date_from_str = date_from.strftime("%d.%m.%Y")
date_to_str = date_to.strftime("%d.%m.%Y")

redLogin, redPassword = readCredentials(CREDENTIALS_FILE)
redmine = redmineStart(redLogin, redPassword)

# getUserIDs(redmine)
fillPlans()
writeReport()

if len(mail_notify) > 0:
    sendMailNotify(redLogin, redPassword)
else:
    print("\nUse '--mail' for e-mail notification")

