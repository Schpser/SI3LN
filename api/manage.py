#!/usr/bin/env python
"""Django's command-line utility for administrative tasks."""
import os
import sys
import threading
import time
import webbrowser
import socket


def main():
    """Run administrative tasks."""
    os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'si3ln_api.settings')
    try:
        from django.core.management import execute_from_command_line
    except ImportError as exc:
        raise ImportError(
            "Couldn't import Django. Are you sure it's installed and "
            "available on your PYTHONPATH environment variable? Did you "
            "forget to activate a virtual environment?"
        ) from exc

    # If running the development server, open the API docs in the browser
    def _open_browser_when_ready(url, host='127.0.0.1', port=8000, timeout=10, interval=0.5):
        deadline = time.time() + timeout
        while time.time() < deadline:
            try:
                with socket.create_connection((host, port), timeout=1):
                    webbrowser.open(url)
                    return
            except OSError:
                time.sleep(interval)

    if 'runserver' in sys.argv:
        t = threading.Thread(
            target=_open_browser_when_ready,
            args=('http://127.0.0.1:8000/api/',),
            daemon=True,
        )
        t.start()

    execute_from_command_line(sys.argv)


if __name__ == '__main__':
    main()
