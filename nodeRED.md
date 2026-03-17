# Instalação e configuração
```console
  sudo apt install nodejs npm
  sudo npm install -g --unsafe-perm node-red
```
  Liberar a porta 1880 no Firewall do Google

# Segurança no node-RED

  → node-red admin hash-pw 
   digite uma senha, ele vai gerar o hash, então copie esse hash gerado

  → sudo nano ~/.node-red/settings.js

  descomente as linhas de adminAuth: { type: "credentials", users: [{ username: "usuario", // O nome de usuário que você desejar password: "$2b$10$SuaSequenciaCopiadaAqui...", // Cole o hash que você gerou no passo 1 permissions: "*" }] },

  e em username, informe o seu usuário e password o hash copiado.

# Execute o NodeRED
→ node-red 


# Iniciar o Node-Red automaticamente

  → which node-red

  → sudo npm install pm2 -g

  → pm2 start /usr/local/bin/node-red -- -v

  → pm2 startup systemd (copiar o comando que ele gera e executar)
