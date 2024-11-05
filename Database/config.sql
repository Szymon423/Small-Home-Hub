BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS "Groups" (
	"GroupID"	INTEGER NOT NULL UNIQUE,
	"Name"	TEXT NOT NULL,
	"Description"	TEXT NOT NULL,
	PRIMARY KEY("GroupID")
);
CREATE TABLE IF NOT EXISTS "Configuration" (
	"Key"	TEXT NOT NULL UNIQUE,
	"Value"	TEXT NOT NULL
);
CREATE TABLE IF NOT EXISTS "Update" (
	"Key"	TEXT NOT NULL UNIQUE,
	"Value"	INTEGER NOT NULL
);
CREATE TABLE IF NOT EXISTS "Users" (
	"UserID"	INTEGER NOT NULL UNIQUE,
	"Name"	TEXT NOT NULL,
	"Surname"	TEXT NOT NULL,
	"Login"	TEXT NOT NULL,
	"Password"	TEXT NOT NULL,
	"Privileges"	INTEGER NOT NULL,
	PRIMARY KEY("UserID")
);
CREATE TABLE IF NOT EXISTS "DeviceTypes" (
	"DeviceTypeID"	INTEGER NOT NULL UNIQUE,
	"Name"	TEXT NOT NULL,
	"Signals"	TEXT NOT NULL,
	PRIMARY KEY("DeviceTypeID")
);
CREATE TABLE IF NOT EXISTS "Devices" (
	"DeviceID"	INTEGER NOT NULL UNIQUE,
	"DeviceTypeID"	INTEGER NOT NULL,
	"Description"	TEXT NOT NULL,
	PRIMARY KEY("DeviceID")
);
CREATE TABLE IF NOT EXISTS "SignalDefinitions" (
	"ID"	INTEGER NOT NULL UNIQUE,
	"GroupID"	INTEGER NOT NULL,
	"Alarms"	TEXT NOT NULL DEFAULT '{}',
	"Labels"	TEXT NOT NULL DEFAULT '{}',
	"Name"	TEXT NOT NULL,
	"Description"	TEXT NOT NULL,
	"DataType"	INTEGER NOT NULL,
	"IsArchived"	BOOLEAN NOT NULL,
	"IsSteerable"	BOOLEAN NOT NULL,
	"DeviceID"	INTEGER NOT NULL,
	PRIMARY KEY("ID")
);
INSERT INTO "DeviceTypes" ("DeviceTypeID","Name","Signals") VALUES (1,'Temperature and Humidity Sensor','[]');
INSERT INTO "DeviceTypes" ("DeviceTypeID","Name","Signals") VALUES (2,'Closing Sensor','[]');
INSERT INTO "DeviceTypes" ("DeviceTypeID","Name","Signals") VALUES (3,'Lights switch','[]');
INSERT INTO "Devices" ("DeviceID","DeviceTypeID","Description") VALUES (1,1,'Temperature and humidity sensor in small room');
INSERT INTO "Devices" ("DeviceID","DeviceTypeID","Description") VALUES (2,3,'Main light in small room');
INSERT INTO "Devices" ("DeviceID","DeviceTypeID","Description") VALUES (3,3,'Desk light in small room');
INSERT INTO "Devices" ("DeviceID","DeviceTypeID","Description") VALUES (4,2,'Doors closing sensor in small room');
INSERT INTO "Devices" ("DeviceID","DeviceTypeID","Description") VALUES (5,2,'Window closing sensor in small room');
INSERT INTO "Devices" ("DeviceID","DeviceTypeID","Description") VALUES (6,1,'Temperature and humidity sensor in corridor');
INSERT INTO "Devices" ("DeviceID","DeviceTypeID","Description") VALUES (7,3,'Main light in corridor');
INSERT INTO "Groups" ("GroupID","Name","Description") VALUES (1,'Small room','Small room signals');
INSERT INTO "Groups" ("GroupID","Name","Description") VALUES (2,'Corridor',' Corridor room signals ');
INSERT INTO "Configuration" ("Key","Value") VALUES ('deviceIp','192.168.0.121');
INSERT INTO "Configuration" ("Key","Value") VALUES ('deviceSubnetMask','255.255.255.0');
INSERT INTO "Configuration" ("Key","Value") VALUES ('deviceName','Kontroler szafy RACK1');
INSERT INTO "Configuration" ("Key","Value") VALUES ('dataRetentionPeriod','31');
INSERT INTO "Configuration" ("Key","Value") VALUES ('backendServerPort','9999');
INSERT INTO "Configuration" ("Key","Value") VALUES ('communicationServerPort','9990');
INSERT INTO "Update" ("Key","Value") VALUES ('timestamp',1730570975);
INSERT INTO "Users" ("UserID","Name","Surname","Login","Password","Privileges") VALUES (1,'serwis','serwis','serwis','serwis',1);
INSERT INTO "Users" ("UserID","Name","Surname","Login","Password","Privileges") VALUES (2,'admin','admin','admin','admin',2);
INSERT INTO "Users" ("UserID","Name","Surname","Login","Password","Privileges") VALUES (3,'Szymon','Wąchała','swachala','swachala',3);
INSERT INTO "SignalDefinitions" ("ID","GroupID","Alarms","Labels","Name","Description","DataType","IsArchived","IsSteerable","DeviceID") VALUES (1,1,'{}','{}','Temperature','Small room temperature sensor',1,1,0,1);
INSERT INTO "SignalDefinitions" ("ID","GroupID","Alarms","Labels","Name","Description","DataType","IsArchived","IsSteerable","DeviceID") VALUES (2,1,'{}','{}','Humidity','Small room humidity sensor',1,1,0,1);
INSERT INTO "SignalDefinitions" ("ID","GroupID","Alarms","Labels","Name","Description","DataType","IsArchived","IsSteerable","DeviceID") VALUES (3,1,'{}','{}','Main light','Small room main light switch',2,1,1,2);
INSERT INTO "SignalDefinitions" ("ID","GroupID","Alarms","Labels","Name","Description","DataType","IsArchived","IsSteerable","DeviceID") VALUES (4,1,'{}','{}','Desk light','Small room desk light switch',2,1,1,3);
INSERT INTO "SignalDefinitions" ("ID","GroupID","Alarms","Labels","Name","Description","DataType","IsArchived","IsSteerable","DeviceID") VALUES (5,1,'{}','{}','Doors','Small room door closing sensor',2,1,0,4);
INSERT INTO "SignalDefinitions" ("ID","GroupID","Alarms","Labels","Name","Description","DataType","IsArchived","IsSteerable","DeviceID") VALUES (6,1,'{}','{}','Windows','Small room windows closing sensor',2,1,0,5);
INSERT INTO "SignalDefinitions" ("ID","GroupID","Alarms","Labels","Name","Description","DataType","IsArchived","IsSteerable","DeviceID") VALUES (7,2,'{}','{}','Main light','Corridor main light switch',2,1,1,7);
INSERT INTO "SignalDefinitions" ("ID","GroupID","Alarms","Labels","Name","Description","DataType","IsArchived","IsSteerable","DeviceID") VALUES (8,2,'{}','{}','Temperature','Small room temperature sensor',1,1,0,6);
INSERT INTO "SignalDefinitions" ("ID","GroupID","Alarms","Labels","Name","Description","DataType","IsArchived","IsSteerable","DeviceID") VALUES (9,0,'{}','{}','Humidity','Small room humidity sensor',1,1,0,6);
CREATE TRIGGER tr_Groups_insert AFTER INSERT ON Groups
BEGIN
    UPDATE "Update" SET Value = (strftime('%s', 'now')) WHERE Key = 'timestamp';
END;
CREATE TRIGGER tr_Groups_update AFTER UPDATE ON Groups
BEGIN
    UPDATE "Update" SET Value = (strftime('%s', 'now')) WHERE Key = 'timestamp';
END;
CREATE TRIGGER tr_Groups_delete AFTER DELETE ON Groups
BEGIN
    UPDATE "Update" SET Value = (strftime('%s', 'now')) WHERE Key = 'timestamp';
END;
CREATE TRIGGER tr_Configuration_insert AFTER INSERT ON Configuration
BEGIN
    UPDATE "Update" SET Value = (strftime('%s', 'now')) WHERE Key = 'timestamp';
END;
CREATE TRIGGER tr_Configuration_update AFTER UPDATE ON Configuration
BEGIN
    UPDATE "Update" SET Value = (strftime('%s', 'now')) WHERE Key = 'timestamp';
END;
CREATE TRIGGER tr_Configuration_delete AFTER DELETE ON Configuration
BEGIN
    UPDATE "Update" SET Value = (strftime('%s', 'now')) WHERE Key = 'timestamp';
END;
CREATE TRIGGER tr_Users_insert AFTER INSERT ON Users
BEGIN
    UPDATE "Update" SET Value = (strftime('%s', 'now')) WHERE Key = 'timestamp';
END;
CREATE TRIGGER tr_Users_update AFTER UPDATE ON Users
BEGIN
    UPDATE "Update" SET Value = (strftime('%s', 'now')) WHERE Key = 'timestamp';
END;
CREATE TRIGGER tr_Users_delete AFTER DELETE ON Users
BEGIN
    UPDATE "Update" SET Value = (strftime('%s', 'now')) WHERE Key = 'timestamp';
END;
CREATE TRIGGER tr_Signals_insert AFTER INSERT ON "SignalDefinitions"
BEGIN
    UPDATE "Update" SET Value = (strftime('%s', 'now')) WHERE Key = 'timestamp';
END;
CREATE TRIGGER tr_Signals_update AFTER UPDATE ON "SignalDefinitions"
BEGIN
    UPDATE "Update" SET Value = (strftime('%s', 'now')) WHERE Key = 'timestamp';
END;
CREATE TRIGGER tr_Signals_delete AFTER DELETE ON "SignalDefinitions"
BEGIN
    UPDATE "Update" SET Value = (strftime('%s', 'now')) WHERE Key = 'timestamp';
END;
COMMIT;
