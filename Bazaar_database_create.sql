-- Created by Vertabelo (http://vertabelo.com)
-- Last modification date: 2019-04-05 09:04:30.462

-- tables
-- Table: _ACCOUNT
CREATE TABLE _ACCOUNT (
    id integer NOT NULL CONSTRAINT _ACCOUNT_pk PRIMARY KEY,
    login text NOT NULL,
    password text NOT NULL,
    is_admin boolean NOT NULL
);

-- Table: _BROKER
CREATE TABLE _BROKER (
    id integer NOT NULL CONSTRAINT id PRIMARY KEY,
    last_name text NOT NULL,
    address text NOT NULL,
    DOB date NOT NULL,
    _ACCOUNT_id integer NOT NULL,
    CONSTRAINT _BROKER__ACCOUNT FOREIGN KEY (_ACCOUNT_id)
    REFERENCES _ACCOUNT (id)
);

-- Table: _DEALS
CREATE TABLE _DEALS (
    _BROKER_id integer NOT NULL,
    _PRODUCT_id integer NOT NULL,
    deal_id integer NOT NULL CONSTRAINT id PRIMARY KEY,
    transaction_date date NOT NULL,
    count integer NOT NULL,
    wholesaler_buyer text NOT NULL,
    CONSTRAINT _DEALS__BROKER FOREIGN KEY (_BROKER_id)
    REFERENCES _BROKER (id),
    CONSTRAINT _DEALS__PRODUCT FOREIGN KEY (_PRODUCT_id)
    REFERENCES _PRODUCT (id)
);

-- Table: _PRODUCT
CREATE TABLE _PRODUCT (
    id integer NOT NULL CONSTRAINT id PRIMARY KEY,
    name text NOT NULL,
    type text NOT NULL,
    cost double NOT NULL,
    wholesale_supplier text NOT NULL,
    shelf_life date NOT NULL,
    number_of_units_delivered integer NOT NULL
);

-- Table: _STATISTICS
CREATE TABLE _STATISTICS (
    id integer NOT NULL CONSTRAINT _STATISTICS_pk PRIMARY KEY,
    _BROKER_id integer NOT NULL,
    count integer NOT NULL,
    total_amount integer NOT NULL,
    CONSTRAINT _STATISTICS__BROKER FOREIGN KEY (_BROKER_id)
    REFERENCES _BROKER (id)
);

-- End of file.

