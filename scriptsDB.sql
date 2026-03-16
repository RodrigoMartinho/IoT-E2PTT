CREATE TABLE RegistroLuzes (id int NOT NULL AUTO_INCREMENT, data_hora timestamp NULL DEFAULT ((now() - interval 3 hour)), status varchar(3) DEFAULT 'OFF', PRIMARY KEY (id));
CREATE TABLE Medicoes (id int NOT NULL auto_increment, data_hora timestamp NULL DEFAULT ((now() - interval 3 hour)), sensor varchar(10), valor decimal(5,2), PRIMARY KEY (id));
