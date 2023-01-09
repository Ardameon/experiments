#!/usr/bin/python3

from e2c import enter_to_continue
from flask import Flask, render_template, request, redirect, escape
from vsearch import search4letters

app = Flask(__name__)

log_fname = 'vsearch.log'

def log_request(req: 'flask_request', res: str) -> None:
    with open(log_fname, 'a') as log:
        print(req.form, req.remote_addr, req.user_agent, res, sep = '|', file = log)

@app.route('/search4', methods = ['POST', 'GET'])

def do_search() -> 'html':
    title = 'Here are your results:'
    phrase = request.form['phrase']
    letters = request.form['letters']
    results = str(search4letters(phrase, letters))
    log_request(request, results)

    return render_template('result.html', the_title = title,
                                          the_phrase = phrase,
                                          the_letters = letters,
                                          the_results = results)

@app.route('/')
@app.route('/entry')

def entry_page() -> 'html':
    return render_template('entry.html', the_title = 'Welcome to search4letters in the web!')

@app.route('/viewlog')

def view_the_log() -> str:
    with open(log_fname) as log:
        contents = log.read()
    return escape(contents)

if __name__ == '__main__':
    app.run(debug = True)



