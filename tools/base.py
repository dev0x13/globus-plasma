import os

def get_globus_version():
    current_dir = os.path.dirname(os.path.abspath(__file__))

    with open(os.path.join(current_dir, "..", "VERSION"), "r") as f:
        return f.read().strip()
