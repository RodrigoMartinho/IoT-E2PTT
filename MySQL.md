# Instalação do MySQL no Linux
```console
  sudo apt install mysql-server -y
```

# Segurança no MySQL  
```console
  sudo mysql_secure_installation 
```
Selecione o método de senha 0 - Baixo | 1 - Medio | 2 - Alto e o restante coloquei tudo Y (sim)

# Executar o MySQL
```console
  sudo mysql
```

# Criar o Database
```sql
  CREATE DATABASE IoT;
```

# Criar usuário e senha
```sql
  CREATE USER 'iot'@'localhost' IDENTIFIED BY 'IoT2026#'; 
```

# Dar permissão para o usuário no banco criado
```sql  
  GRANT ALL PRIVILEGES ON IoT.* TO 'iot'@'localhost';

  FLUSH PRIVILEGES;

  EXIT;
```

# Acessar o MySQL 
```console
  mysql -u iot -p
```
após o comando acima, será solicitada a senha. Insira a senha.

```sql
  USE IoT;
```

# Criação das tabelas
```sql
  CREATE TABLE Historico (id int NOT NULL AUTO_INCREMENT, data_hora timestamp NULL DEFAULT ((now() - interval 3 hour)), sensor varchar(10), status varchar(3) DEFAULT 'OFF', PRIMARY KEY (id));
  CREATE TABLE Medicoes (id int NOT NULL auto_increment, data_hora timestamp NULL DEFAULT ((now() - interval 3 hour)), sensor varchar(10), valor decimal(5,2), PRIMARY KEY (id));
```
