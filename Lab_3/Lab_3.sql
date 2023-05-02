--user: system
CREATE PROFILE CUSTOMER_PROFILE LIMIT PASSWORD_LIFE_TIME 180 SESSIONS_PER_USER 20 FAILED_LOGIN_ATTEMPTS 7 PASSWORD_LOCK_TIME 1 PASSWORD_REUSE_TIME 10 PASSWORD_GRACE_TIME DEFAULT CONNECT_TIME 180 IDLE_TIME 30;

DROP PROFILE CUSTOMER_PROFILE CASCADE;
DROP USER CUSTOMER CASCADE;

CREATE USER customer IDENTIFIED BY qwerty
    DEFAULT TABLESPACE USERS
    PROFILE CUSTOMER_PROFILE
    QUOTA UNLIMITED ON USERS;

grant connect to customer;
DROP ROLE CUSTOMER_ROLE;

CREATE ROLE CUSTOMER_ROLE;
Alter session set container = cdb$root;

commit;

Grant connect, resource to customer;
grant customer_role to customer;
grant customer_role to customer_2;

-- DBLINK по схеме USER1 - USER2
--1
CREATE DATABASE LINK anotherdb
   CONNECT TO customer
   IDENTIFIED BY qwerty
   USING 'NAT-PC:1521/XE';

DROP DATABASE LINK anotherdb;
--grant create database link to customer;
GRANT CREATE DATABASE LINK TO CUSTOMER;

-- user: CUSTOMER
create table MKV (
    MKV_id number(10) generated as identity(start with 1 increment by 1),
    MKV_name varchar2(30) not null,
    MKV_number number(3) not null,
    CONSTRAINT MKV_pk PRIMARY KEY (MKV_id)
);
commit;
insert into MKV (MKV_name, MKV_number) values ('Kristina', 20);

select * from MKV;
drop table MKV;

-- user: system
SELECT * FROM SNA@anotherdb;    -- подключение и просмотр информации в таблице SNA сервера удаленной базе данных


--2
SELECT * FROM SNA@anotherdb;
INSERT INTO SNA@anotherdb(MKV_name, MKV_number) values('NewRow', 20);
UPDATE SNA@anotherdb SET MKV_NUMBER = 13 WHERE MKV_NAME = 'NewRow';
DELETE SNA@anotherdb where MKV_NAME = 'NewRow';
commit;


--3
BEGIN
  DELETE SNA@anotherdb;
  UPDATE SNA@anotherdb set MKV_NAME ='New' where MKV_NUMBER=1;
END;

BEGIN
  SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
  UPDATE SNA@anotherdb set MKV_NAME ='New' where MKV_NUMBER=1;
  SYS.DBMS_LOCK.SLEEP(60);
  COMMIT;
END;