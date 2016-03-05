# Vrijestudent

A news site with a cms.    

### Installation

You need Python, Pip and Virtualenv installed globally:

```sh
$ git clone https://github.com/ayegemen/vrije.git
$ cd vrije
$ virtualenv env
$ source env/bin/activate
(env)$ pip install -r requirements.txt
(env)$ python manage.py migrate
(env)$ python manage.py loaddata initial_data.json
(env)$ python manage.py runserver
```