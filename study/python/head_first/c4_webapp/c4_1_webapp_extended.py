#!/usr/bin/python3

from e2c import enter_to_continue
from flask import Flask, render_template, request, redirect
from vsearch import search4letters

app = Flask(__name__)

# @app.route('/')

# def hello() -> '302':
#   return redirect('/entry')

@app.route('/search4', methods = ['POST', 'GET'])

def do_search1() -> 'html':
    title = 'Here are your results:'
    phrase = request.form['phrase']
    letters = request.form['letters']
    results = str(search4letters(phrase, letters))

    return render_template('result.html', the_title = title,
                                          the_phrase = phrase,
                                          the_letters = letters,
                                          the_results = results)

@app.route('/')
@app.route('/entry')

def entry_page() -> 'html':
    return render_template('entry.html', the_title = 'Welcome to search4letters in the web!')

if __name__ == '__main__':
    app.run(debug = True)



