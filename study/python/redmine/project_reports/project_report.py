#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import smtplib
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
from datetime import datetime
#from datetime import date
import datetime
from redminelib import Redmine
import sys
import pprint
import calendar

CREDENTIALS_FILE = '../credentials'

projects= {}

users = [
     {"name": "Александр Лещев", "idtask":207556, "uid":195, "email":"alexander.leschev@eltex.loc"}
    ,{"name": "Михаил Тырышкин", "idtask":206624, "uid":134, "email":"mikhail.tyryshkin@eltex.loc"}
    ,{"name": "Роман Рупп", "idtask":206528, "uid":177, "email":"roman.rupp@eltex.loc"}
    ,{"name": "Артем Щерба", "idtask":207688, "uid":838, "email":"artem.shcherba@eltex.loc"}
    ,{"name": "Владимир Алексеев", "idtask":206849, "uid":489, "email":"vladimir.alekseev@eltex.loc"}
    ,{"name": "Игорь Липовцев", "idtask":198854, "uid":1023, "email":"igor.lipovcev@eltex.loc"}
    ,{"name": "Кристина Михед", "idtask":204253, "uid":662, "email":"kristina.mihed@eltex.loc"}
    ,{"name": "Ксения Михед", "idtask":204895, "uid":661, "email":"kseniya.mihed@eltex.loc"}
    ,{"name": "Павел Сазонов", "idtask":205436, "uid":879, "email":"pavel.sazonov@eltex.loc"}
    ,{"name": "Владислав Бражников", "idtask":203796, "uid":1178, "email":"vladislav.brazhnikov@eltex.loc"}
    ,{"name": "Павел Федотов", "idtask":203707, "uid":663, "email":"pavel.fedotov@eltex.loc"}
    ,{"name": "Илья Трухин", "idtask":219603, "uid":1268, "email":"ilya.truhin@eltex.loc"}
    ,{"name": "Кузьма Казыгашев", "idtask":190033, "uid":327, "email":"kuzma.kazygashev@eltex.loc"}
    ,{"name": "Артём Карпов", "idtask":195165, "uid":330, "email":"artem.karpov@eltex.loc"}
    # ,{"name": "Екатерина Маклашкина", "idtask":188480, "uid":-1, "email":"ekaterina.maklashkina@eltex.loc"}
    # ,{"name": "Тимофей Гайдамака", "idtask":214388, "uid":-1, "email":"timofey.gaydamaka@eltex.loc"}
]

def printHelp():
    print("Usage:")
    print("\t<without> current week\n")
    print("\t--help        this message")
    print("\t--week        project time entry from a week")
    print("\t--month       time entry from a month")

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

def prepare_proj(proj):
    if proj in projects.keys():
        return
    else:
        projects[proj] = {}

def prepare_ver(proj, ver):
    if ver in projects[proj].keys():
        return
    else:
        projects[proj][ver] = {}

def prepare_user(proj, ver, user):
    prepare_proj(proj)
    prepare_ver(proj, ver)

    if user in projects[proj][ver].keys():
        return
    else:
        projects[proj][ver][user] = 0

def add_user(proj, ver, user, hrs):
    prepare_user(proj, ver, user)
    projects[proj][ver][user] += hrs;

# @action_info('Format issue')
def formatIssue(user_name, issue):
    name_map = {'SMG-CORAL':'SMG-CORAL',
                'Time tracking':'Time tracking',
                'SBC':'SBC',
                'E-SBC':'E-SBC',
                'SMG-SIGTRAN':'SMG-SIGTRAN',
                'SMG-V52AN':'SMG-V52AN'}

    try:
        task = redmine.issue.get(issue.issue.id)
        project = redmine.project.get(issue.project.id)
    except Exception as ex:
        print("Failed to get task " + str(issue.issue.id) + " error: " + str(ex))
        return

    if issue.project.name in name_map.keys():
        proj_name = issue.project.name
    else:
        if hasattr(project, 'parent'):
            if project.parent.name == 'Digital gateway':
                proj_name = 'SMG gateway'
            else:
                proj_name = project.parent.name
        else:
            proj_name = project.name

    print(user_name + " " + proj_name)

    if hasattr(task, 'fixed_version'):
        add_user(proj_name, task.fixed_version.name, user_name, issue.hours);
    else:
        add_user(proj_name, "-", user_name, issue.hours);

def fillPlans():
    for user in users:
        print("Update " + user["name"])
        issues = redmine.time_entry.all(user_id=user["uid"], from_date=date_from, to_date=date_to)
        if len(issues):
            for issue in issues:
                formatIssue(user["name"], issue)

def writeReport():
    cvsf = open("report/project_report_"+date_from_str+"_"+date_to_str+".csv",'w')
    cvsf.write("Проект;Версия;Пользователь;Часы\n")

    print("----------------------------")
    print("----------------------------")
    print("----------------------------")

    for pkey in projects:
        cvsf.write(pkey + ";");
        first_line = True
        phours = 0
        for pver in projects[pkey]:
            if not first_line:
                cvsf.write(";");

            cvsf.write(pver + ";");

            first_user = True
            vhours = 0
            for pusr in projects[pkey][pver]:
                if not first_user:
                    cvsf.write(";;");

                cvsf.write(pusr + ";{:3.2f}".format(projects[pkey][pver][pusr]) + "\n")
                phours += projects[pkey][pver][pusr]
                vhours += projects[pkey][pver][pusr]
                first_line = False
                first_user = False
            cvsf.write(";Всего:;" + "{:3.2f}".format(vhours) + ";\n")
        cvsf.write("Всего:;" + "{:3.2f}".format(phours) + ";;\n\n")

    cvsf.close()


def date_from_isoweek(iso_year, iso_weeknumber, iso_weekday):
    return datetime.datetime.strptime(
        '{:04d} {:02d} {:d}'.format(iso_year, iso_weeknumber, iso_weekday),
        '%G %V %u').date()
#
#

iso = datetime.date.today().isocalendar()
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
    if arg == "--month":
        #последний месяц
        # date_from = datetime.datetime(datetime.datetime.now().year, int(sys.argv[idx+1])-1, calendar.monthrange(datetime.datetime.now().year, int(sys.argv[idx+1])-1)[1])
        # date_to = datetime.datetime(datetime.datetime.now().year, int(sys.argv[idx+1]), calendar.monthrange(datetime.datetime.now().year, int(sys.argv[idx+1]))[1])

        #за год
        date_from = datetime.datetime(datetime.datetime.now().year, 1, 1)
        date_to = datetime.datetime(datetime.datetime.now().year, 12, 31)

        # date_from = datetime.datetime(datetime.datetime.now().year + int(datetime.datetime.now().month / 12), int(sys.argv[idx+1])-1, calendar.monthrange(datetime.datetime.now().year, int(sys.argv[idx+1])-1)[1])
        # date_to = datetime.datetime(datetime.datetime.now().year + int(datetime.datetime.now().month / 12), int(sys.argv[idx+1]), calendar.monthrange(datetime.datetime.now().year, int(sys.argv[idx+1]))[1])


date_from_str = date_from.strftime("%d.%m.%Y")
date_to_str = date_to.strftime("%d.%m.%Y")

print("\ntime report by - [", date_from_str, " - ", date_to_str, "]\n")

redLogin, redPassword = readCredentials(CREDENTIALS_FILE)
redmine = redmineStart(redLogin, redPassword)


# getUserIDs(redmine)
fillPlans()
writeReport()

