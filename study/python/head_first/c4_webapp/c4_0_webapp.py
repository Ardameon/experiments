#!/usr/bin/python3

from e2c import enter_to_continue
from flask import Flask
from vsearch import search4letters

app = Flask(__name__)

@app.route('/')

def hello() -> str:
  return 'Hello world from Flask'

@app.route('/search4')

def do_search() -> str:
  return str(search4letters('live, universe and everything', 'eiru'))

app.run()







