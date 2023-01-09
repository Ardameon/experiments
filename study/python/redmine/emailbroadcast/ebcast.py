#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
import smtplib
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText

CREDENTIALS_FILE = '../credentials'

users = [
     {"name": "Александр Лещев", "email":"alexander.leschev@eltex.loc"}
     ,{"name": "Михаил Тырышкин", "email":"mikhail.tyryshkin@eltex.loc"}
    #,{"name": "Роман Рупп", "email":"roman.rupp@eltex.loc"}
    #,{"name": "Артем Щерба", "email":"artem.shcherba@eltex.loc"}
    #,{"name": "Владимир Алексеев", "email":"vladimir.alekseev@eltex.loc"}
    #,{"name": "Игорь Липовцев", "email":"igor.lipovcev@eltex.loc"}
    #,{"name": "Кристина Михед", "email":"kristina.mihed@eltex.loc"}
    #,{"name": "Ксения Михед", "email":"kseniya.mihed@eltex.loc"}
    ## ,{"name": "Павел Сазонов", "email":"pavel.sazonov@eltex.loc"}
    #,{"name": "Владислав Бражников", "email":"vladislav.brazhnikov@eltex.loc"}
    ## ,{"name": "Федотов Павел", "email":"pavel.fedotov@eltex.loc"}
    #,{"name": "Илья Трухин", "email":"ilya.truhin@eltex.loc"}
    ## ,{"name": "Кузьма Казыгашев", "email":"kuzma.kazygashev@eltex.loc"}
    # ,{"name": "Артём Карпов", "email":"artem.karpov@eltex.loc"}
]

def printHelp():
    print("Usage:")
    print("\t<without> current week\n")
    print("\t--help    this message")
    print("\t--hdr     email header message")
    print("\t--body    email body message")

def readCredentials(credFile):
    with open(credFile, 'rt') as cf:
        credTuple = cf.readline().strip().split(' ')
    return credTuple

def sendMailNotify(login, passwd, hdr, body):
    try:
        smtp = smtplib.SMTP('mail.eltex.loc', 587)
        smtp.starttls()
        smtp.login(login + "@eltex.loc", passwd)

        for user in users:
            msg = MIMEMultipart()
            msg['From'] = login + "@eltex.loc"
            msg['Subject'] = hdr
            msg['To'] = user['email']

            msg.attach(MIMEText(body, 'text'))

            print("Send notify to '" + user['email'])
            smtp.sendmail( login + "@eltex.loc", user['email'], msg.as_string())

        smtp.quit()

    except Exception as ex:
        print("Something went wrong....",ex)

hdr = str()
body = str()

for idx, arg in enumerate(sys.argv):
    if arg == "--help":
        printHelp()
        sys.exit()
    if arg == "--hdr":
        hdr = sys.argv[idx+1]
    if arg == "--body":
        body = sys.argv[idx+1]

if len(hdr) == 0:
    print("email header not set")
    sys.exit()

if len(body) == 0:
    print("email body not set")
    sys.exit()

redLogin, redPassword = readCredentials(CREDENTIALS_FILE)

sendMailNotify(redLogin, redPassword, hdr, body)

