import socket
import threading
import tkinter as tk
from tkinter import simpledialog, scrolledtext, messagebox

class SettingsDialog(simpledialog.Dialog):
    def body(self, master):
        tk.Label(master, text="Server IP:").grid(row=0, column=0, sticky="e")
        tk.Label(master, text="Port:").grid(row=1, column=0, sticky="e")
        tk.Label(master, text="Username:").grid(row=2, column=0, sticky="e")

        self.ip_entry = tk.Entry(master)
        self.ip_entry.insert(0, "127.0.0.1")
        self.ip_entry.grid(row=0, column=1)

        self.port_entry = tk.Entry(master)
        self.port_entry.insert(0, "54000")
        self.port_entry.grid(row=1, column=1)

        self.name_entry = tk.Entry(master)
        self.name_entry.insert(0, "User")
        self.name_entry.grid(row=2, column=1)

        return self.ip_entry

    def apply(self):
        self.result = {
            "ip": self.ip_entry.get(),
            "port": int(self.port_entry.get()),
            "name": self.name_entry.get().strip() or "User"
        }

class ChatClient:
    def __init__(self, master):
        self.master = master
        self.master.title("TCP Chat Client")

        self.chat_area = scrolledtext.ScrolledText(master, state='disabled', width=60, height=20)
        self.chat_area.pack(padx=10, pady=10)

        entry_frame = tk.Frame(master)
        entry_frame.pack(padx=10, pady=(0,10))

        self.entry = tk.Entry(entry_frame, width=50)
        self.entry.pack(side=tk.LEFT, fill=tk.X, expand=True)
        self.entry.bind("<Return>", self.send_message)

        self.send_button = tk.Button(entry_frame, text="Send", command=self.send_message)
        self.send_button.pack(side=tk.LEFT, padx=(5,0))

        self.sock = None
        self.connected = False
        self.username = None

        self.open_settings()

    def open_settings(self):
        dialog = SettingsDialog(self.master, title="Connect to Server")
        if dialog.result:
            self.server_ip = dialog.result["ip"]
            self.server_port = dialog.result["port"]
            self.username = dialog.result["name"]
            self.connect_to_server()
        else:
            self.master.destroy()

    def connect_to_server(self):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            self.sock.connect((self.server_ip, self.server_port))
            self.connected = True
            threading.Thread(target=self.receive_messages, daemon=True).start()
            self.append_message(f"Connected to {self.server_ip}:{self.server_port} as {self.username}")
        except Exception as e:
            messagebox.showerror("Connection Error", f"Cannot connect to server: {e}")
            self.master.destroy()

    def receive_messages(self):
        while self.connected:
            try:
                data = self.sock.recv(1024)
                if not data:
                    break
                message = data.decode('utf-8')
                self.append_message(message)
            except:
                break
        self.append_message("Disconnected from server.")
        self.connected = False

    def send_message(self, event=None):
        msg = self.entry.get().strip()
        if msg == '':
            return
        try:
            full_msg = f"{self.username}: {msg}"
            self.sock.sendall(full_msg.encode('utf-8'))
            self.entry.delete(0, tk.END)
            if msg.lower() == 'exit':
                self.connected = False
                self.sock.close()
                self.master.quit()
        except Exception as e:
            messagebox.showerror("Send Error", f"Failed to send message: {e}")

    def append_message(self, msg):
        self.chat_area.configure(state='normal')
        self.chat_area.insert(tk.END, msg + '\n')
        self.chat_area.configure(state='disabled')
        self.chat_area.yview(tk.END)

if __name__ == "__main__":
    root = tk.Tk()
    client = ChatClient(root)
    root.mainloop()
