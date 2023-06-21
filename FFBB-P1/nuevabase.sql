SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'LATIN9';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

--
-- Name: plpgsql; Type: EXTENSION; Schema: -; Owner: 
--

CREATE EXTENSION IF NOT EXISTS plpgsql WITH SCHEMA pg_catalog;


--
-- Name: EXTENSION plpgsql; Type: COMMENT; Schema: -; Owner: 
--

COMMENT ON EXTENSION plpgsql IS 'PL/pgSQL procedural language';


SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: customers; Type: TABLE; Schema: public; Owner: alumnodb
--

CREATE TABLE public.customers (
    customernumber integer NOT NULL,
    customername character varying(50) NOT NULL,
    contactlastname character varying(50) NOT NULL,
    contactfirstname character varying(50) NOT NULL,
    phone character varying(50) NOT NULL,
    addressline1 character varying(50) NOT NULL,
    addressline2 character varying(50) DEFAULT NULL::character varying,
    city character varying(50) NOT NULL,
    state character varying(50) DEFAULT NULL::character varying,
    postalcode character varying(15) DEFAULT NULL::character varying,
    country character varying(50) NOT NULL,
    creditlimit numeric(10,2) DEFAULT NULL::numeric
);
--He quitado salesrepemployeenumber ya que esa información está en la tabla orders


ALTER TABLE public.customers OWNER TO alumnodb;

--
-- Name: employees; Type: TABLE; Schema: public; Owner: alumnodb
--

CREATE TABLE public.employees (
    employeenumber integer NOT NULL,
    lastname character varying(50) NOT NULL,
    firstname character varying(50) NOT NULL,
    extension character varying(10) NOT NULL,
    email character varying(100) NOT NULL,
    officecode character varying(10) NOT NULL,
    reportsto integer,
    jobtitle character varying(50) NOT NULL
);


ALTER TABLE public.employees OWNER TO alumnodb;

--
-- Name: offices; Type: TABLE; Schema: public; Owner: alumnodb
--

CREATE TABLE public.offices (
    officecode character varying(10) NOT NULL,
    city character varying(50) NOT NULL,
    phone character varying(50) NOT NULL,
    addressline1 character varying(50) NOT NULL,
    addressline2 character varying(50) DEFAULT NULL::character varying,
    state character varying(50) DEFAULT NULL::character varying,
    country character varying(50) NOT NULL,
    postalcode character varying(15) NOT NULL,
    territory character varying(10) NOT NULL
);


ALTER TABLE public.offices OWNER TO alumnodb;

--
-- Name: orderdetails; Type: TABLE; Schema: public; Owner: alumnodb
--

CREATE TABLE public.orderdetails (
    ordernumber integer NOT NULL,
    productcode character varying(15) NOT NULL,
    quantityordered integer NOT NULL,
    priceeach numeric(10,2) NOT NULL,
    orderlinenumber smallint NOT NULL
);


ALTER TABLE public.orderdetails OWNER TO alumnodb;

--
-- Name: orders; Type: TABLE; Schema: public; Owner: alumnodb
--

CREATE TABLE public.orders (
    ordernumber integer NOT NULL,
    employeenumber integer NOT NULL,
    orderdate date NOT NULL,
    requireddate date NOT NULL,
    shippeddate date,
    status character varying(15) NOT NULL,
    comments text,
    customernumber integer NOT NULL
);


ALTER TABLE public.orders OWNER TO alumnodb;

--
-- Name: payments; Type: TABLE; Schema: public; Owner: alumnodb
--

CREATE TABLE public.payments (
    ordernumber integer NOT NULL,
    checknumber character varying(50) NOT NULL,
    paymentdate date NOT NULL,
    amount numeric(10,2) NOT NULL
);

ALTER TABLE public.payments OWNER TO alumnodb;

---
---Name EmployeeOffice; TABLE; Schema: public; Owner: alumnodb
---

CREATE TABLE public.EmployeeOffice (
    id integer NOT NULL,
    employeenumber integer NOT NULL,
    officecode character varying(50) NOT NULL,
    dateIn date NOT NULL,
    dateOut date
);


ALTER TABLE public.EmployeeOffice OWNER TO alumnodb;

--
-- Name: productlines; Type: TABLE; Schema: public; Owner: alumnodb
--

CREATE TABLE public.productlines (
    productline character varying(50) NOT NULL,
    textdescription character varying(4000) DEFAULT NULL::character varying,
    htmldescription character varying(4000),
    image character varying
);


ALTER TABLE public.productlines OWNER TO alumnodb;

--
-- Name: products; Type: TABLE; Schema: public; Owner: alumnodb
--

CREATE TABLE public.products (
    productcode character varying(15) NOT NULL,
    productname character varying(70) NOT NULL,
    productline character varying(50) NOT NULL,
    productscale character varying(10) NOT NULL,
    productvendor character varying(50) NOT NULL,
    productdescription text NOT NULL,
    quantityinstock smallint NOT NULL,
    buyprice numeric(10,2) NOT NULL,
    msrp numeric(10,2) NOT NULL
);


ALTER TABLE public.products OWNER TO alumnodb;


--
-- Name: customers customers_pkey; Type: CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.customers
    ADD CONSTRAINT customers_pkey PRIMARY KEY (customernumber);


--
-- Name: employees employees_pkey; Type: CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.employees
    ADD CONSTRAINT employees_pkey PRIMARY KEY (employeenumber);


--
-- Name: offices offices_pkey; Type: CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.offices
    ADD CONSTRAINT offices_pkey PRIMARY KEY (officecode);

--
-- Name: Employeeoffice Employeeoffice_pkey; Type: CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.EmployeeOffice
    ADD CONSTRAINT Employeeoffice_pkey PRIMARY KEY (id);

--
-- Name: orderdetails orderdetails_pkey; Type: CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.orderdetails
    ADD CONSTRAINT orderdetails_pkey PRIMARY KEY (ordernumber, productcode);


--
-- Name: orders orders_pkey; Type: CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.orders
    ADD CONSTRAINT orders_pkey PRIMARY KEY (ordernumber);


--
-- Name: payments payments_pkey; Type: CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.payments
    ADD CONSTRAINT payments_pkey PRIMARY KEY (ordernumber);


--
-- Name: productlines productlines_pkey; Type: CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.productlines
    ADD CONSTRAINT productlines_pkey PRIMARY KEY (productline);


--
-- Name: products products_pkey; Type: CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.products
    ADD CONSTRAINT products_pkey PRIMARY KEY (productcode);


--
-- Name: customernumber; Type: INDEX; Schema: public; Owner: alumnodb
--

CREATE INDEX customernumber ON public.orders USING btree (customernumber);


--
-- Name: officecode; Type: INDEX; Schema: public; Owner: alumnodb
--

CREATE INDEX officecode ON public.employees USING btree (officecode);


--
-- Name: productcode; Type: INDEX; Schema: public; Owner: alumnodb
--

CREATE INDEX productcode ON public.orderdetails USING btree (productcode);


--
-- Name: reportsto; Type: INDEX; Schema: public; Owner: alumnodb
--

CREATE INDEX reportsto ON public.employees USING btree (reportsto);


--
-- Name: salesrepemployeenumber; Type: INDEX; Schema: public; Owner: alumnodb
--


--CREATE INDEX salesrepemployeenumber ON public.customers USING btree (salesrepemployeenumber);


--
-- Name: customers customers_ibfk_1; Type: FK CONSTRAINT; Schema: public; Owner: alumnodb
--
---------------------------------------------------------------------------------------------------------------------------------------------------

--
-- Name: employees employees_ibfk_1; Type: FK CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.employees
    ADD CONSTRAINT employees_ibfk_1 FOREIGN KEY (reportsto) REFERENCES public.employees(employeenumber);


--
-- Name: employees employees_ibfk_2; Type: FK CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.employees
    ADD CONSTRAINT employees_ibfk_2 FOREIGN KEY (officecode) REFERENCES public.offices(officecode);


--
-- Name: orderdetails orderdetails_ibfk_1; Type: FK CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.orderdetails
    ADD CONSTRAINT orderdetails_ibfk_1 FOREIGN KEY (ordernumber) REFERENCES public.orders(ordernumber);


--
-- Name: orderdetails orderdetails_ibfk_2; Type: FK CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.orderdetails
    ADD CONSTRAINT orderdetails_ibfk_2 FOREIGN KEY (productcode) REFERENCES public.products(productcode);


--
-- Name: orders orders_ibfk_1; Type: FK CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.orders
    ADD CONSTRAINT orders_ibfk_1 FOREIGN KEY (customernumber) REFERENCES public.customers(customernumber);

--
-- Name: orders orders_ibfk_2; Type: FK CONSTRAINT; Schema: public; Owner: alumnodb
--
ALTER TABLE ONLY public.orders
    ADD CONSTRAINT orders_ibfk_2 FOREIGN KEY (employeenumber) REFERENCES public.employees(employeenumber);

--
-- Name: orders EmployeeOffice_ibfk_1; Type: FK CONSTRAINT; Schema: public; Owner: alumnodb
--
ALTER TABLE ONLY public.EmployeeOffice
    ADD CONSTRAINT EmployeeOffice_ibfk_1 FOREIGN KEY (employeenumber) REFERENCES public.employees(employeenumber);

--
-- Name: orders EmployeeOffice_ibfk_2; Type: FK CONSTRAINT; Schema: public; Owner: alumnodb
--
ALTER TABLE ONLY public.EmployeeOffice
    ADD CONSTRAINT EmployeeOffice_ibfk_2 FOREIGN KEY (officecode) REFERENCES public.offices(officecode);

--
-- Name: payments payments_ibfk_1; Type: FK CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.payments
    ADD CONSTRAINT payments_ibfk_1 FOREIGN KEY (ordernumber) REFERENCES public.orders(ordernumber);


--
-- Name: products products_ibfk_1; Type: FK CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.products
    ADD CONSTRAINT products_ibfk_1 FOREIGN KEY (productline) REFERENCES public.productlines(productline);

