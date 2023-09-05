#!/usr/bin/python3

from redminelib import Redmine
from optparse import OptionParser

usage = "usage: %prog [options]\n\nRedmine issue reporter"
parser = OptionParser(usage=usage)
parser.add_option("-p", "--project", dest="project", default='e-sbc',
                  help="Redmine project", metavar="NAME")
parser.add_option("-i", "--issue", dest="issue_id", type="int", default=0,
                  help="Issue ID for which the report will be compiled", metavar="ID")
parser.add_option("-u", "--user", dest="red_username", default='',
                  help="Redmine username", metavar="NAME")
parser.add_option("-w", "--password", dest="red_password", default='',
                  help="Redmine password", metavar="PASSWORD")
parser.add_option("-r", "--report", action="store_true", dest="need_to_report", default=False,
                  help="Report to redmine", metavar="True/False")

(options, args) = parser.parse_args()

print(options)

project = options.project
main_issue_id = options.issue_id
red_username = options.red_username
red_password = options.red_password
need_to_report = options.need_to_report

report_delimiter = "h2. Срез по задачам"

completed_issues = []
rejected_issues = []
in_progress_issues = []
not_finished_issues = []
new_issues = []
off_plan_issues = []

redmine = Redmine('http://red.eltex.loc', username=red_username, password=red_password)

all_issues = redmine.issue.filter(project_id=project, status_id='*')

def issue_time_entry_sum(issue):
    time = 0.0
    if hasattr(issue, 'time_entries'):
        for time_entry in issue.time_entries:
            time += time_entry.hours
    return time

def issue_week_tags_str(issue):
    tag_week_str = 'plan_week'
    tags_str = issue.custom_fields[1].value
    tags_list = tags_str.split()
    weeks_list = []

    for tag in tags_list:
        tag_splited = tag.rsplit('_', 1)
        if (tag_splited[0] == tag_week_str):
            weeks_list.append(tag_splited[1])

    return ', '.join([week_num for week_num in weeks_list])

def distribute_issue(issue):
        issue_info = "| #" + str(issue.id) + " | " + str(issue.subject) + " | " + str(issue.assigned_to) + " | "
        issue_info += "%02.2f" % issue_time_entry_sum(issue) + " | " + str(issue.done_ratio) + "% | " + issue_week_tags_str(issue) + " | "

        if str(issue.status) == "New":
            new_issues.append(issue_info)
        elif str(issue.status) == "Resolved" or str(issue.status) == "Closed":
            completed_issues.append(issue_info)
        elif str(issue.status) == "In Progress":
            in_progress_issues.append(issue_info)
        elif str(issue.status) == "Rejected":
            rejected_issues.append(issue_info)
        else:
            not_finished_issues.append(issue_info)


def get_sub_issues(issues, _parent_id):
    for issue in issues:
        if hasattr(issue, 'parent') and issue.parent.id == _parent_id:
            distribute_issue(issue)
            get_sub_issues(issues, issue.id)

def issue_list_print(prefix, issue_list):
    issues_info = "|\\5=. *" + prefix + "* |\n"
    issues_info += "|=. *Номер* |=. *Название* |=. *Исполнитель* |=. *Время* |=. *Готовность* |=. *План(неделя)* |\n"
    for issue_info in issue_list:
        issues_info += issue_info + "\n"

    issues_info += "\n"
    return issues_info


def make_issue_report(issue_id):
    issue = redmine.issue.get(issue_id)
    report = issue.description.split(report_delimiter)
    if len(report) == 1:
        report[0] += "\n\n"
    report = report[0] + report_delimiter + "\n\n---\n\n"
    report += issue_list_print("ГОТОВЫ", completed_issues)
    report += issue_list_print("В РАБОТЕ", in_progress_issues)
    report += issue_list_print("БЫЛИ НАЧАТЫ, НО ЕЩЁ НЕ ЗАКОНЧЕНЫ (ОЖИДАЮТ)", not_finished_issues)
    report += issue_list_print("НОВЫЕ ИЗ ПЛАНА", new_issues)
    report += issue_list_print("НОВЫЕ ВНЕПЛАНОВЫЕ (СТАБИЛИЗАЦИЯ, РЕФАКТОРИНГ)", off_plan_issues)
    if need_to_report == True:
        redmine.issue.update(issue_id, description=report)
    return report

get_sub_issues(all_issues, main_issue_id)

print(make_issue_report(main_issue_id))
