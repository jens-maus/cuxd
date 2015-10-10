CREATE table digitemp (
  dtKey int(11) NOT NULL auto_increment,
  time timestamp NOT NULL,
  SerialNumber varchar(17) NOT NULL,
  Fahrenheit decimal(6,2) NOT NULL,
  PRIMARY KEY (dtKey),
  KEY serial_key (SerialNumber),
  KEY time_key (time)
);

