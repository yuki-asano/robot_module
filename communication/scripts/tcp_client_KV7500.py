#!/usr/bin/env python
# -*- coding: utf-8 -*-

import socket

host_ip = '192.168.0.10'  # kv-7500のIPアドレス
host_port = 8501          # 上位リンク通信のポート番号
buffer_size = 1024

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # ソケットクライアントの作成
client.connect((host_ip,host_port)) # サーバに接続

# 上位リンク通信のコマンド(see manual "8-4 コマンド一覧")
command = "?K"   # ?K: 機種の問い合わせ
separator = "\r" # 区切り符号CR
msg = command + separator
print("send: " + msg)

client.send(msg.encode("ascii")) #ASCIIにエンコードして送信
response = client.recv(buffer_size)
response = response.decode("UTF-8") #UTF-8にデコードして受信

client.close()

print("Received: " + response)
