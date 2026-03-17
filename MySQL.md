# Instalação do MySQL no Linux
  
  → sudo apt install mysql-server -y

  → sudo mysql_secure_installation 

Selecione o método de senha 0 - Baixo | 1 - Medio | 2 - Alto e o restante coloquei tudo Y (sim)

  → sudo mysql (abrir o mysql)

# Criar o Database
  CREATE DATABASE IoT;

# Criar usuário e senha
  CREATE USER 'iot'@'localhost' IDENTIFIED BY 'IoT2026#'; 

# Dar permissão para o usuário no banco criado
  GRANT ALL PRIVILEGES ON IoT.* TO 'iot'@'localhost'; 

  FLUSH PRIVILEGES;
  EXIT;

# Acessar o MySQL 
→ mysql -u iot -p (após ENTER, inserir a senha do usuário)

USE IoT;

# Criação das tabelas
\`\`\`sql
CREATE TABLE RegistroLuzes (id int NOT NULL AUTO_INCREMENT, data_hora timestamp NULL DEFAULT ((now() - interval 3 hour)), status varchar(3) DEFAULT 'OFF', PRIMARY KEY (id));
CREATE TABLE Medicoes (id int NOT NULL auto_increment, data_hora timestamp NULL DEFAULT ((now() - interval 3 hour)), sensor varchar(10), valor decimal(5,2), PRIMARY KEY (id));
\`\`\`
