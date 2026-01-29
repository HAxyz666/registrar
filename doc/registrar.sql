--
-- PostgreSQL database dump
--

-- Dumped from database version 17.5
-- Dumped by pg_dump version 17.5

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET transaction_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- Name: courses; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.courses (
    id character varying(20) NOT NULL,
    name character varying(200) NOT NULL,
    credit smallint DEFAULT 0
);


ALTER TABLE public.courses OWNER TO postgres;

--
-- Name: enrollments; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.enrollments (
    student_id character varying(20) NOT NULL,
    course_id character varying(20) NOT NULL
);


ALTER TABLE public.enrollments OWNER TO postgres;

--
-- Name: grades; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.grades (
    student_id character varying(20) NOT NULL,
    course_id character varying(20) NOT NULL,
    grade double precision
);


ALTER TABLE public.grades OWNER TO postgres;

--
-- Name: schedules; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.schedules (
    course_id character varying(20) NOT NULL,
    teacher_id character varying(20),
    time_slot character varying(50) NOT NULL,
    classroom character varying(50) NOT NULL
);


ALTER TABLE public.schedules OWNER TO postgres;

--
-- Name: secretaries; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.secretaries (
    id character varying(20) NOT NULL,
    name character varying(100) NOT NULL
);


ALTER TABLE public.secretaries OWNER TO postgres;

--
-- Name: students; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.students (
    id character varying(20) NOT NULL,
    name character varying(100) NOT NULL
);


ALTER TABLE public.students OWNER TO postgres;

--
-- Name: teachers; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.teachers (
    id character varying(20) NOT NULL,
    name character varying(100) NOT NULL
);


ALTER TABLE public.teachers OWNER TO postgres;

--
-- Name: teaching; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.teaching (
    teacher_id character varying(20) NOT NULL,
    course_id character varying(20) NOT NULL
);


ALTER TABLE public.teaching OWNER TO postgres;

--
-- Data for Name: courses; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.courses (id, name, credit) FROM stdin;
CS101	C Programming	3
CS201	Data structure	4
MATH101	Advanced Math	4
CS401	C++	0
CS001	c	0
CS002	linux	0
\.


--
-- Data for Name: enrollments; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.enrollments (student_id, course_id) FROM stdin;
S001	CS101
S002	CS101
S001	CS201
S003	MATH101
S004	CS201
S005	CS101
S003	CS401
S001	CS001
S001	CS002
\.


--
-- Data for Name: grades; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.grades (student_id, course_id, grade) FROM stdin;
S001	CS101	89.5
S003	MATH101	95
S001	CS002	87
\.


--
-- Data for Name: schedules; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.schedules (course_id, teacher_id, time_slot, classroom) FROM stdin;
CS101	T001	周一9点到10点	101
CS201	T002	周二8点到9点	201
MATH101	T003	周三8点到10点	301
CS001	T001	周一14到16	102
CS002	T001	周二10到12点	101
\.


--
-- Data for Name: secretaries; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.secretaries (id, name) FROM stdin;
SEC001	Ms. Wang
SEC002	Mr. Li
\.


--
-- Data for Name: students; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.students (id, name) FROM stdin;
S001	Thomas
S002	Jerry
S003	Baker
S004	Tom
S005	Musk
\.


--
-- Data for Name: teachers; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.teachers (id, name) FROM stdin;
T001	Dr. Smith
T002	Dr. Johnson
T003	Dr. Brown
\.


--
-- Data for Name: teaching; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.teaching (teacher_id, course_id) FROM stdin;
T001	CS101
T002	CS201
T003	MATH101
T001	CS401
T001	CS001
T001	CS002
\.


--
-- Name: courses courses_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.courses
    ADD CONSTRAINT courses_pkey PRIMARY KEY (id);


--
-- Name: enrollments enrollments_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.enrollments
    ADD CONSTRAINT enrollments_pkey PRIMARY KEY (student_id, course_id);


--
-- Name: grades grades_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.grades
    ADD CONSTRAINT grades_pkey PRIMARY KEY (student_id, course_id);


--
-- Name: schedules schedules_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.schedules
    ADD CONSTRAINT schedules_pkey PRIMARY KEY (course_id, time_slot);


--
-- Name: secretaries secretaries_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.secretaries
    ADD CONSTRAINT secretaries_pkey PRIMARY KEY (id);


--
-- Name: students students_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.students
    ADD CONSTRAINT students_pkey PRIMARY KEY (id);


--
-- Name: teachers teachers_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.teachers
    ADD CONSTRAINT teachers_pkey PRIMARY KEY (id);


--
-- Name: teaching teaching_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.teaching
    ADD CONSTRAINT teaching_pkey PRIMARY KEY (teacher_id, course_id);


--
-- Name: enrollments enrollments_course_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.enrollments
    ADD CONSTRAINT enrollments_course_id_fkey FOREIGN KEY (course_id) REFERENCES public.courses(id) ON DELETE CASCADE;


--
-- Name: enrollments enrollments_student_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.enrollments
    ADD CONSTRAINT enrollments_student_id_fkey FOREIGN KEY (student_id) REFERENCES public.students(id) ON DELETE CASCADE;


--
-- Name: grades grades_course_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.grades
    ADD CONSTRAINT grades_course_id_fkey FOREIGN KEY (course_id) REFERENCES public.courses(id) ON DELETE CASCADE;


--
-- Name: grades grades_student_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.grades
    ADD CONSTRAINT grades_student_id_fkey FOREIGN KEY (student_id) REFERENCES public.students(id) ON DELETE CASCADE;


--
-- Name: schedules schedules_course_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.schedules
    ADD CONSTRAINT schedules_course_id_fkey FOREIGN KEY (course_id) REFERENCES public.courses(id) ON DELETE CASCADE;


--
-- Name: schedules schedules_teacher_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.schedules
    ADD CONSTRAINT schedules_teacher_id_fkey FOREIGN KEY (teacher_id) REFERENCES public.teachers(id) ON DELETE CASCADE;


--
-- Name: teaching teaching_course_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.teaching
    ADD CONSTRAINT teaching_course_id_fkey FOREIGN KEY (course_id) REFERENCES public.courses(id) ON DELETE CASCADE;


--
-- Name: teaching teaching_teacher_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.teaching
    ADD CONSTRAINT teaching_teacher_id_fkey FOREIGN KEY (teacher_id) REFERENCES public.teachers(id) ON DELETE CASCADE;


--
-- PostgreSQL database dump complete
--

