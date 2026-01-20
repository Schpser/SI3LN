import subprocess
import json
import threading

class CppGameBridge:
    """Bridge entre Python (API) et C++ (jeu)"""
    
    def __init__(self, cpp_executable_path="./SI3LN_CPP/build/SI3LN"):
        self.cpp_process = None
        self.cpp_executable = cpp_executable_path
        
    def start_game(self, level="1"):
        """Démarre le jeu C++ en arrière-plan"""
        self.cpp_process = subprocess.Popen(
            [self.cpp_executable, "--level", level, "--headless"],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE
        )
        
    def send_command(self, command):
        """Envoie une commande au jeu C++"""
        if self.cpp_process:
            # Écrire commande JSON dans stdin du processus C++
            command_json = json.dumps(command) + "\n"
            self.cpp_process.stdin.write(command_json.encode())
            self.cpp_process.stdin.flush()
            
    def get_game_state(self):
        """Lit l'état du jeu depuis le C++"""
        if self.cpp_process:
            # Lire la dernière ligne de stdout
            line = self.cpp_process.stdout.readline()
            return json.loads(line.decode())
        return None
    