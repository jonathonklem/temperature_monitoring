temperature_monitoring
======================

**Installation**
 - temperatureAlert.ino gets compiled and uploaded to your Arduino device
 - a mysql database needs to be created with the following table
  
 

    CREATE TABLE temperature (
    
    id int(11) NOT NULL AUTO_INCREMENT,
    
    temperature float NOT NULL,
    
    PRIMARY KEY (id)
    
    ) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=3039 ;
    
    
    
 - gettemp.php and settemp.php need to be uploaded to your web server
 - 'include.php' needs to be created and you need to set the proper variables to allow gettemp and settemp to connect to your database
 - modify line 6 of settemp.php to only allow your internal ip address to set the temperature
