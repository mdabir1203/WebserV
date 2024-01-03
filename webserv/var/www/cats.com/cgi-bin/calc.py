#!/usr/bin/env python3
import cgi

# Включаем отладочный вывод
import cgitb
cgitb.enable()

# Получаем данные из формы
form = cgi.FieldStorage()
num1 = form.getvalue('num1')
num2 = form.getvalue('num2')

# Проверяем, что оба числа были переданы
if num1 is None or num2 is None:
    print("Content-type: text/html\n")
    print("<html><body>Error: Please provide both numbers.</body></html>")
else:
    # Преобразуем числа в целые значения и вычисляем сумму
    try:
        num1 = int(num1)
        num2 = int(num2)
        result = num1 + num2
        # Выводим результат в формате JSON
        print("Content-type: application/json\n")
        print(f'{{ "sum": {result} }}')
    except ValueError:
        print("Content-type: text/html\n")
        print("<html><body>Error: Invalid input. Please enter valid numbers.</body></html>")
