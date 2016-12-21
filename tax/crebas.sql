/*==============================================================*/
/* DBMS name:      ORACLE Version 11g                           */
/* Created on:     2016/12/3 22:29:22                           */
/*==============================================================*/


alter table "contractprogress"
   drop constraint FK_CONTRACT_REFERENCE_PROJECT;

alter table "incometaxcalc"
   drop constraint FK_INCOMETA_REFERENCE_PROJECT;

alter table "incometaxcalcdetail"
   drop constraint FK_INCOMETA_REFERENCE_PROJECTB;

alter table "inputtaxmonth"
   drop constraint FK_INPUTTAX_REFERENCE_PROJECT;

alter table "monthprogress"
   drop constraint FK_MONTHPRO_REFERENCE_PROJECT;

alter table "organrole"
   drop constraint FK_ORGANROL_REFERENCE_ORGAN;

alter table "organrole"
   drop constraint FK_ORGANROL_REFERENCE_ROLE;

alter table "projectbudget"
   drop constraint FK_PROJECTB_REFERENCE_PROJECT;

alter table "projectbudget1"
   drop constraint FK_PROJECTB_REFERENCE_PROJECT2;

alter table "projectbudget2"
   drop constraint FK_PROJECTB_REFERENCE_PROJECTB;

alter table "projectpayee"
   drop constraint FK_PROJECTP_REFERENCE_PROJECT;

alter table "projectsubpack"
   drop constraint FK_PROJECTS_REFERENCE_PROJECT;

alter table "purchasedetail"
   drop constraint FK_PURCHASE_REFERENCE_PROJECT;

alter table "purchaseimportlist"
   drop constraint FK_PURCHASE_REFERENCE_PROJECT2;

alter table "roleauth"
   drop constraint FK_ROLEAUTH_REFERENCE_ROLE;

alter table "roleauth"
   drop constraint FK_ROLEAUTH_REFERENCE_AUTH;

alter table "supplier"
   drop constraint FK_SUPPLIER_REFERENCE_SUPPLIE5;

alter table "supplierclass2"
   drop constraint FK_SUPPLIER_REFERENCE_SUPPLIE4;

alter table "supplierclass3"
   drop constraint FK_SUPPLIER_REFERENCE_SUPPLIE2;

alter table "supplierclass3tax"
   drop constraint FK_SUPPLIER_REFERENCE_SUPPLIE3;

alter table "suppliersubclass"
   drop constraint FK_SUPPLIER_REFERENCE_SUPPLIE7;

alter table "suppliersubclass"
   drop constraint FK_SUPPLIER_REFERENCE_SUPPLIER;

alter table "suppliersubclass"
   drop constraint FK_SUPPLIER_REFERENCE_SUPPLIE6;

alter table "userinfo"
   drop constraint FK_USERINFO_REFERENCE_ORGAN;

alter table "userinfo"
   drop constraint FK_USERINFO_REFERENCE_ROLE;

alter table "workprogress"
   drop constraint FK_WORKPROG_REFERENCE_PROJECT;

drop table "auth" cascade constraints;

drop table "compositepurchase" cascade constraints;

drop table "contractprogress" cascade constraints;

drop table "engineeringpurchase" cascade constraints;

drop table "goods" cascade constraints;

drop table "goodsimportlist" cascade constraints;

drop table "incometaxcalc" cascade constraints;

drop table "incometaxcalcdetail" cascade constraints;

drop table "inputtaxmonth" cascade constraints;

drop table "monthprogress" cascade constraints;

drop table "organ" cascade constraints;

drop table "organrole" cascade constraints;

drop table "pro_goods" cascade constraints;

drop table "pro_goodsimportlist" cascade constraints;

drop table "project" cascade constraints;

drop table "projectbudget" cascade constraints;

drop table "projectbudget1" cascade constraints;

drop table "projectbudget2" cascade constraints;

drop table "projectpayee" cascade constraints;

drop table "projectsubpack" cascade constraints;

drop table "purchasedetail" cascade constraints;

drop table "purchaseimportlist" cascade constraints;

drop table "role" cascade constraints;

drop table "roleauth" cascade constraints;

drop table "supplier" cascade constraints;

drop table "supplierclass" cascade constraints;

drop table "supplierclass2" cascade constraints;

drop table "supplierclass3" cascade constraints;

drop table "supplierclass3tax" cascade constraints;

drop table "suppliersubclass" cascade constraints;

drop table "sysconfig" cascade constraints;

drop table "userinfo" cascade constraints;

drop table "workprogress" cascade constraints;

drop table 审批表 cascade constraints;

/*==============================================================*/
/* Table: "auth"                                                */
/*==============================================================*/
create table "auth" 
(
   "authorid"           INT                  not null,
   "authorname"         VARCHAR(50)          not null,
   constraint PK_AUTH primary key ("authorid")
);

/*==============================================================*/
/* Table: "compositepurchase"                                   */
/*==============================================================*/
create table "compositepurchase" 
(
   "id"                 INT                  not null,
   "organid"            INT,
   "date"               VARCHAR(10),
   "isproject"          CHAR,
   "projectid"          INT,
   "supplierclass"      INT,
   "classification"     INT,
   "subcategory"        INT,
   "name"               VARCHAR(20),
   "model"              VARCHAR(20),
   "measurementunit"    VARCHAR(20),
   "number"             NUMBER(38,8),
   "notaxprice"         NUMBER(38,8),
   "notaxvalue"         NUMBER(38,8),
   "taxrate"            NUMBER(10,4),
   "taxvalue"           NUMBER(38,8),
   "totalvalue"         NUMBER(38,8),
   "createuser"         INT,
   "createdate"         VARCHAR(20),
   constraint PK_COMPOSITEPURCHASE primary key ("id")
);

/*==============================================================*/
/* Table: "contractprogress"                                    */
/*==============================================================*/
create table "contractprogress" 
(
   "projectid"          INT                  not null,
   "yearmonth"          CHAR(6)              not null,
   "monthvalue"         NUMBER(38,4),
   "createuser"         INT,
   "createtime"         VARCHAR(20),
   constraint PK_CONTRACTPROGRESS primary key ("projectid", "yearmonth")
);

/*==============================================================*/
/* Table: "engineeringpurchase"                                 */
/*==============================================================*/
create table "engineeringpurchase" 
(
   "id"                 INT                  not null,
   "organid"            INT,
   "date"               VARCHAR(10),
   "isproject"          CHAR,
   "projectid"          INT,
   "supplierclass"      INT,
   "classification"     INT,
   "subcategory"        INT,
   "name"               VARCHAR(20),
   "model"              VARCHAR(20),
   "measurementunit"    VARCHAR(20),
   "number"             NUMBER(38,8),
   "notaxprice"         NUMBER(38,8),
   "notaxvalue"         NUMBER(38,8),
   "taxrate"            NUMBER(10,4),
   "taxvalue"           NUMBER(38,8),
   "totalvalue"         NUMBER(38,8),
   "createuser"         INT,
   "createdate"         VARCHAR(20),
   constraint PK_ENGINEERINGPURCHASE primary key ("id")
);

/*==============================================================*/
/* Table: "goods"                                               */
/*==============================================================*/
create table "goods" 
(
   "checkid"            INT,
   "goodsid"            INT                  not null,
   "createmode"         CHAR,
   "goodscode"          VARCHAR(20),
   "goodsname"          VARCHAR(30),
   "typecode"           VARCHAR(20),
   "property"           VARCHAR(20),
   "type"               VARCHAR(20),
   "batchname"          VARCHAR(20),
   "stocknumber"        NUMBER(38,4),
   "measurementunit"    VARCHAR(20),
   "weightunit"         VARCHAR(20),
   "volumeunit"         VARCHAR(20),
   "trademark"          VARCHAR(20),
   "approvedcode"       VARCHAR(20),
   "barcode"            VARCHAR(20),
   "packingmethod"      VARCHAR(20),
   "invoicename"        VARCHAR(20),
   "createuser"         INT,
   "createtime"         VARCHAR(20),
   constraint PK_GOODS primary key ("goodsid")
);

comment on column "goods"."createmode" is
'导入
新建';

/*==============================================================*/
/* Table: "goodsimportlist"                                     */
/*==============================================================*/
create table "goodsimportlist" 
(
   "checkid"            INT                  not null,
   "intime"             VARCHAR(20)          not null,
   "createuser"         INT                  not null,
   "infile"             VARCHAR(50),
   constraint PK_GOODSIMPORTLIST primary key ("checkid")
);

/*==============================================================*/
/* Table: "incometaxcalc"                                       */
/*==============================================================*/
create table "incometaxcalc" 
(
   "projectid"          INT                  not null,
   "notaxvalue"         NUMBER(38,4),
   "taxvalue"           NUMBER(38,4),
   constraint PK_INCOMETAXCALC primary key ("projectid")
);

/*==============================================================*/
/* Table: "incometaxcalcdetail"                                 */
/*==============================================================*/
create table "incometaxcalcdetail" 
(
   "projectid"          INT                  not null,
   "groupid"            INT                  not null,
   "serialnumber"       INT                  not null,
   "materialname"       VARCHAR(30),
   "model"              VARCHAR(15),
   "unit"               VARCHAR(10),
   "number"             NUMBER(38,4),
   "budgetprice"        NUMBER(38,4),
   "total"              NUMBER(38,4),
   "taxrate"            NUMBER(38,4),
   "notaxvalue"         NUMBER(38,4),
   "taxvalue"           NUMBER(38,4),
   constraint PK_INCOMETAXCALCDETAIL primary key ("projectid", "groupid", "serialnumber")
);

/*==============================================================*/
/* Table: "inputtaxmonth"                                       */
/*==============================================================*/
create table "inputtaxmonth" 
(
   "projectid"          INT                  not null,
   "yearmonth"          CHAR(6)              not null,
   "notaxvalue17"       NUMBER(38,8),
   "tax17"              NUMBER(38,8),
   "notaxvalue13"       NUMBER(38,8),
   "tax13"              NUMBER(38,8),
   "notaxvalue11"       NUMBER(38,8),
   "tax11"              NUMBER(38,8),
   "notaxvalue6"        NUMBER(38,8),
   "tax6"               NUMBER(38,8),
   "notaxvalue5"        NUMBER(38,8),
   "tax5"               NUMBER(38,8),
   "notaxvalue3"        NUMBER(38,8),
   "tax3"               NUMBER(38,8),
   "notaxvalue1_5"      NUMBER(38,8),
   "tax1_5"             NUMBER(38,8),
   "notaxvalue"         NUMBER(38,8),
   "taxvalue"           NUMBER(38,8),
   constraint PK_INPUTTAXMONTH primary key ("projectid", "yearmonth")
);

/*==============================================================*/
/* Table: "monthprogress"                                       */
/*==============================================================*/
create table "monthprogress" 
(
   "projectid"          INT                  not null,
   "yearmonth"          VARCHAR(6)           not null,
   "processinfo"        VARCHAR(300),
   "processinfo2"       VARCHAR(300),
   constraint PK_MONTHPROGRESS primary key ("projectid", "yearmonth")
);

/*==============================================================*/
/* Table: "organ"                                               */
/*==============================================================*/
create table "organ" 
(
   "organid"            INT                  not null
      constraint CKC_ORGANID_ORGAN check ("organid" >= 1),
   "uporganid"          INT,
   "name"               VARCHAR2(100)        not null,
   constraint PK_ORGAN primary key ("organid")
);

/*==============================================================*/
/* Table: "organrole"                                           */
/*==============================================================*/
create table "organrole" 
(
   "organid"            INT                 
      constraint CKC_ORGANID_ORGANROL check ("organid" is null or ("organid" >= 1)),
   "roleid"             INT
);

/*==============================================================*/
/* Table: "pro_goods"                                           */
/*==============================================================*/
create table "pro_goods" 
(
   "checkid"            INT                  not null,
   "goodsid"            INT                  not null,
   "createmode"         CHAR,
   "goodscode"          VARCHAR(20),
   "goodsname"          VARCHAR(30),
   "typecode"           VARCHAR(20),
   "property"           VARCHAR(20),
   "type"               VARCHAR(20),
   "batchname"          VARCHAR(20),
   "stocknumber"        NUMBER(38,4),
   "measurementunit"    VARCHAR(20),
   "weightunit"         VARCHAR(20),
   "volumeunit"         VARCHAR(20),
   "trademark"          VARCHAR(20),
   "approvedcode"       VARCHAR(20),
   "barcode"            VARCHAR(20),
   "packingmethod"      VARCHAR(20),
   "invoicename"        VARCHAR(20),
   "createuser"         INT,
   "createtime"         VARCHAR(20),
   constraint PK_PRO_GOODS primary key ("checkid", "goodsid")
);

comment on column "pro_goods"."createmode" is
'导入
新建';

/*==============================================================*/
/* Table: "pro_goodsimportlist"                                 */
/*==============================================================*/
create table "pro_goodsimportlist" 
(
   "checkid"            INT                  not null,
   "intime"             VARCHAR(20)          not null,
   "createuser"         INT                  not null,
   "infile"             VARCHAR(50),
   constraint PK_PRO_GOODSIMPORTLIST primary key ("checkid")
);

/*==============================================================*/
/* Table: "project"                                             */
/*==============================================================*/
create table "project" 
(
   "projectid"          INT                  not null,
   "projectname"        VARCHAR(60)          not null,
   "employer"           VARCHAR(60)          not null,
   "projectmode"        CHAR                 not null,
   "projecttype"        INT,
   "projectmanager"     VARCHAR(20),
   "contractnumber"     VARCHAR(30),
   "contractdate"       CHAR(10),
   "contractenddate"    CHAR(10),
   "permitnumber"       VARCHAR(30),
   "permitdate"         CHAR(10),
   "location"           VARCHAR(30),
   "materialsupplymode" CHAR,
   "contractvalue"      NUMBER(38,4),
   "taxrate"            NUMBER(38,4),
   "contractnottax"     NUMBER(38,4),
   "isectocyst"         CHAR,
   "isgk"               CHAR,
   "createuser"         INT,
   "createtime"         VARCHAR(20),
   "status"             CHAR,
   constraint PK_PROJECT primary key ("projectid")
);

comment on column "project"."projecttype" is
'取自数据字典';

comment on column "project"."materialsupplymode" is
'2种：
部分甲供材
清包工
';

comment on column "project"."isectocyst" is
'是
否';

comment on column "project"."isgk" is
'是
否';

comment on column "project"."status" is
'执行
暂停';

/*==============================================================*/
/* Table: "projectbudget"                                       */
/*==============================================================*/
create table "projectbudget" 
(
   "projectid"          INT                  not null,
   "inprojectname"      VARCHAR(60),
   "intabletime"        VARCHAR(20),
   "createuser"         INT,
   "createtime"         VARCHAR(20),
   constraint PK_PROJECTBUDGET primary key ("projectid")
);

/*==============================================================*/
/* Table: "projectbudget1"                                      */
/*==============================================================*/
create table "projectbudget1" 
(
   "projectid"          INT                  not null,
   "groupid"            INT                  not null,
   "number"             INT                  not null,
   "name"               VARCHAR(20)          not null,
   "value"              NUMBER(38,4)         not null,
   constraint PK_PROJECTBUDGET1 primary key ("groupid", "projectid")
);

/*==============================================================*/
/* Table: "projectbudget2"                                      */
/*==============================================================*/
create table "projectbudget2" 
(
   "projectid"          INT                  not null,
   "groupid"            INT                  not null,
   "number"             INT                  not null,
   "materialname"       VARCHAR(30),
   "type"               VARCHAR(15),
   "unit"               VARCHAR(10),
   "unmber"             NUMBER(38,4),
   "budgetaryprice"     NUMBER(38,4),
   "total"              NUMBER(38,4),
   constraint PK_PROJECTBUDGET2 primary key ("groupid", "projectid", "number")
);

/*==============================================================*/
/* Table: "projectpayee"                                        */
/*==============================================================*/
create table "projectpayee" 
(
   "project"            INT                  not null,
   "yearmonth"          CHAR(6)              not null,
   "value"              NUMBER(38,8),
   constraint PK_PROJECTPAYEE primary key ("project", "yearmonth")
);

/*==============================================================*/
/* Table: "projectsubpack"                                      */
/*==============================================================*/
create table "projectsubpack" 
(
   "subpackid"          INT                  not null,
   "projectid"          INT,
   "subpackname"        VARCHAR(60),
   "subpackunitname"    VARCHAR(60),
   "location"           VARCHAR(60),
   "contractdate"       CHAR(10),
   "contractbegindate"  CHAR(10),
   "projectlocation"    VARCHAR(60),
   "materialtype"       CHAR,
   "contractvalue"      NUMBER(38,4),
   "createuser"         INT,
   "createtime"         VARCHAR(20),
   constraint PK_PROJECTSUBPACK primary key ("subpackid")
);

comment on column "projectsubpack"."materialtype" is
'2种：
自行采购
总包方采购';

/*==============================================================*/
/* Table: "purchasedetail"                                      */
/*==============================================================*/
create table "purchasedetail" 
(
   "projectid"          INT                  not null,
   "yearmonth"          CHAR(6)              not null,
   "number"             INT                  not null,
   "date"               VARCHAR(10),
   "unitname"           VARCHAR(30),
   "goodsname"          VARCHAR(20),
   "goodstype"          VARCHAR(20),
   "measurementunit"    VARCHAR(20),
   "innumber"           NUMBER(38,8),
   "notaxprice"         NUMBER(38,8),
   "notaxvalue"         NUMBER(38,8),
   "taxrate"            NUMBER(10,4),
   "taxvalue"           NUMBER(38,8),
   "totalvalue"         NUMBER(38,8),
   constraint PK_PURCHASEDETAIL primary key ("projectid", "yearmonth", "number")
);

/*==============================================================*/
/* Table: "purchaseimportlist"                                  */
/*==============================================================*/
create table "purchaseimportlist" 
(
   "projectid"          INT                  not null,
   "yearmonth"          CHAR(6)              not null,
   "createuser"         INT,
   "createtime"         VARCHAR(20),
   "infile"             VARCHAR(30),
   constraint PK_PURCHASEIMPORTLIST primary key ("projectid", "yearmonth")
);

/*==============================================================*/
/* Table: "role"                                                */
/*==============================================================*/
create table "role" 
(
   "roleid"             INT                  not null,
   NAME                 VARCHAR(50),
   constraint PK_ROLE primary key ("roleid")
);

/*==============================================================*/
/* Table: "roleauth"                                            */
/*==============================================================*/
create table "roleauth" 
(
   "roleid"             INT                  not null,
   "authorid"           INT
);

/*==============================================================*/
/* Table: "supplier"                                            */
/*==============================================================*/
create table "supplier" 
(
   "supplierid"         INT                  not null,
   "supplierclassid"    INT,
   "taxpayerid"         VARCHAR(30),
   "suppliername"       VARCHAR(30),
   "juridicalperson"    VARCHAR(20),
   "registeredcapital"  NUMBER(38),
   "businessterm"       VARCHAR(10),
   "supplierscope"      VARCHAR(300),
   "location"           VARCHAR(50),
   "phone"              VARCHAR(15),
   "depositbank"        VARCHAR(20),
   "bankaccount"        VARCHAR(20),
   "businesslicensefile" VARCHAR(50),
   "taxcertificatefile" VARCHAR(50),
   "organcodefile"      VARCHAR(50),
   "accountlicensefile" VARCHAR(50),
   "status"             CHAR,
   "createuser"         INT,
   "createdate"         VARCHAR(20),
   constraint PK_SUPPLIER primary key ("supplierid")
);

comment on column "supplier"."status" is
' 启用
停用';

/*==============================================================*/
/* Table: "supplierclass"                                       */
/*==============================================================*/
create table "supplierclass" 
(
   "goodsid"            INT                  not null,
   "goodsname"          VARCHAR(20)          not null,
   "desc"               VARCHAR(50),
   constraint PK_SUPPLIERCLASS primary key ("goodsid")
);

/*==============================================================*/
/* Table: "supplierclass2"                                      */
/*==============================================================*/
create table "supplierclass2" 
(
   "goodsid"            INT,
   "goodsclass"         INT                  not null,
   "name"               VARCHAR(30),
   constraint PK_SUPPLIERCLASS2 primary key ("goodsclass")
);

/*==============================================================*/
/* Table: "supplierclass3"                                      */
/*==============================================================*/
create table "supplierclass3" 
(
   "goodsclasssubid"    INT                  not null,
   "goodsclass"         INT,
   "subname"            VARCHAR(30),
   constraint PK_SUPPLIERCLASS3 primary key ("goodsclasssubid")
);

/*==============================================================*/
/* Table: "supplierclass3tax"                                   */
/*==============================================================*/
create table "supplierclass3tax" 
(
   "goodsclasssubid"    INT,
   "taxrate"            NUMBER(38,4)
);

/*==============================================================*/
/* Table: "suppliersubclass"                                    */
/*==============================================================*/
create table "suppliersubclass" 
(
   "supplierid"         INT                  not null,
   "goodsclassid"       INT                  not null,
   "goodsclasssubid"    INT                  not null,
   constraint PK_SUPPLIERSUBCLASS primary key ("supplierid", "goodsclassid", "goodsclasssubid")
);

/*==============================================================*/
/* Table: "sysconfig"                                           */
/*==============================================================*/
create table "sysconfig" 
(
   "GROUP"              INT                  not null,
   ID                   INT                  not null,
   VALUE                VARCHAR(100)         not null,
   constraint PK_SYSCONFIG primary key ("GROUP", ID)
);

/*==============================================================*/
/* Table: "userinfo"                                            */
/*==============================================================*/
create table "userinfo" 
(
   ID                   VARCHAR(30)          not null,
   PASSWD               VARCHAR(32)          not null,
   USERNAME             VARCHAR(50)          not null,
   ORGANID              INT                  not null,
   "position"           INT                  not null,
   "sex"                CHAR(2),
   "birthday"           VARCHAR(10),
   "fixphone"           VARCHAR(15),
   "mobilephone"        VARCHAR(15),
   "email"              VARCHAR(30),
   "worktime"           VARCHAR(10),
   "roleid"             INT,
   STATUS               CHAR                 not null,
   constraint PK_USERINFO primary key (ID)
);

comment on column "userinfo"."position" is
'0 系统管理员 1 员工 2 经理 3 领导';

comment on column "userinfo".STATUS is
'0 停用 1 启用';

/*==============================================================*/
/* Table: "workprogress"                                        */
/*==============================================================*/
create table "workprogress" 
(
   "projectid"          INT                  not null,
   "yearmonth"          CHAR(6)              not null,
   "monthvalue"         NUMBER(38,4),
   "montharea"          NUMBER(38,4),
   "createuser"         INT,
   "createtime"         VARCHAR(20),
   constraint PK_WORKPROGRESS primary key ("projectid", "yearmonth")
);

/*==============================================================*/
/* Table: 审批表                                                   */
/*==============================================================*/
create table 审批表 
(
   "checkid"            INT                  not null,
   "checktype"          INT                  not null,
   "checkidx"           INT                  not null,
   "applicant"          VARCHAR(30)          not null,
   "intime"             VARCHAR(20),
   "submitdesc"         VARCHAR(500),
   "checkuser"          VARCHAR(30),
   "checktime"          VARCHAR(20),
   "checkdesc"          VARCHAR(500),
   "status"             CHAR                 not null,
   constraint PK_审批表 primary key ("checkid")
);

comment on column 审批表."checkidx" is
'对应审批内容的唯一ID
如审批的是项目，就是项目ID';

alter table "contractprogress"
   add constraint FK_CONTRACT_REFERENCE_PROJECT foreign key ("projectid")
      references "project" ("projectid");

alter table "incometaxcalc"
   add constraint FK_INCOMETA_REFERENCE_PROJECT foreign key ("projectid")
      references "project" ("projectid");

alter table "incometaxcalcdetail"
   add constraint FK_INCOMETA_REFERENCE_PROJECTB foreign key ("groupid", "projectid", "serialnumber")
      references "projectbudget2" ("groupid", "projectid", "number");

alter table "inputtaxmonth"
   add constraint FK_INPUTTAX_REFERENCE_PROJECT foreign key ("projectid")
      references "project" ("projectid");

alter table "monthprogress"
   add constraint FK_MONTHPRO_REFERENCE_PROJECT foreign key ("projectid")
      references "project" ("projectid");

alter table "organrole"
   add constraint FK_ORGANROL_REFERENCE_ORGAN foreign key ("organid")
      references "organ" ("organid");

alter table "organrole"
   add constraint FK_ORGANROL_REFERENCE_ROLE foreign key ("roleid")
      references "role" ("roleid");

alter table "projectbudget"
   add constraint FK_PROJECTB_REFERENCE_PROJECT foreign key ("projectid")
      references "project" ("projectid");

alter table "projectbudget1"
   add constraint FK_PROJECTB_REFERENCE_PROJECT2 foreign key ("projectid")
      references "project" ("projectid");

alter table "projectbudget2"
   add constraint FK_PROJECTB_REFERENCE_PROJECTB foreign key ("groupid", "projectid")
      references "projectbudget1" ("groupid", "projectid");

alter table "projectpayee"
   add constraint FK_PROJECTP_REFERENCE_PROJECT foreign key ("project")
      references "project" ("projectid");

alter table "projectsubpack"
   add constraint FK_PROJECTS_REFERENCE_PROJECT foreign key ("projectid")
      references "project" ("projectid");

alter table "purchasedetail"
   add constraint FK_PURCHASE_REFERENCE_PROJECT foreign key ("projectid")
      references "project" ("projectid");

alter table "purchaseimportlist"
   add constraint FK_PURCHASE_REFERENCE_PROJECT2 foreign key ("projectid")
      references "project" ("projectid");

alter table "roleauth"
   add constraint FK_ROLEAUTH_REFERENCE_ROLE foreign key ("roleid")
      references "role" ("roleid");

alter table "roleauth"
   add constraint FK_ROLEAUTH_REFERENCE_AUTH foreign key ("authorid")
      references "auth" ("authorid");

alter table "supplier"
   add constraint FK_SUPPLIER_REFERENCE_SUPPLIE5 foreign key ("supplierclassid")
      references "supplierclass" ("goodsid");

alter table "supplierclass2"
   add constraint FK_SUPPLIER_REFERENCE_SUPPLIE4 foreign key ("goodsid")
      references "supplierclass" ("goodsid");

alter table "supplierclass3"
   add constraint FK_SUPPLIER_REFERENCE_SUPPLIE2 foreign key ("goodsclass")
      references "supplierclass2" ("goodsclass");

alter table "supplierclass3tax"
   add constraint FK_SUPPLIER_REFERENCE_SUPPLIE3 foreign key ("goodsclasssubid")
      references "supplierclass3" ("goodsclasssubid");

alter table "suppliersubclass"
   add constraint FK_SUPPLIER_REFERENCE_SUPPLIE7 foreign key ("supplierid")
      references "supplier" ("supplierid");

alter table "suppliersubclass"
   add constraint FK_SUPPLIER_REFERENCE_SUPPLIER foreign key ("goodsclassid")
      references "supplierclass2" ("goodsclass");

alter table "suppliersubclass"
   add constraint FK_SUPPLIER_REFERENCE_SUPPLIE6 foreign key ("goodsclasssubid")
      references "supplierclass3" ("goodsclasssubid");

alter table "userinfo"
   add constraint FK_USERINFO_REFERENCE_ORGAN foreign key (ORGANID)
      references "organ" ("organid");

alter table "userinfo"
   add constraint FK_USERINFO_REFERENCE_ROLE foreign key ("roleid")
      references "role" ("roleid");

alter table "workprogress"
   add constraint FK_WORKPROG_REFERENCE_PROJECT foreign key ("projectid")
      references "project" ("projectid");

