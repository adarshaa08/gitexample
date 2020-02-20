#-------------------------------------------------
#
# Project created by QtCreator 2019-01-21T12:58:51
#
#-------------------------------------------------

QT       += core gui

TARGET = fpb
TEMPLATE = app

DEPENDPATH += . forms include src include/openssl #include/libxml
DEPENDPATH += . forms include src include/libxml
INCLUDEPATH += . include include/openssl #include/libxml
INCLUDEPATH += . include include/libxml

QT += sql
OBJECTS_DIR = intermediate/obj
MOC_DIR = intermediate/moc

UI_HEADERS_DIR = include/
RCC_DIR = intermediate

#LIBS += -L /u02/tools/rootfs/usr/lib -lX11 -ljpeg -L./lib  -lauth_app -lgl11pos -lcurl -lcrypto -lbiometric -lopenjpeg -lwsq  -lusb-0.1 -lprotobuf-c -lbfd-aadhar -lbfd -lbclib -lscanner_status -lpinpad -ltfm -lQtSql ./lib/libbclib.a  ./lib/libsmart.a -O2 -lssl  #./lib/libauth_app.a
#LIBS += -L /home/user/Desktop/GL11-GNOME-SDK-3.1/APP/Selfdaig-SDK-1.0/lib/ -lX11 -ljpeg -L./lib  -lauth_app -lgl11pos -lcurl -lcrypto -lbiometric -lopenjpeg -lwsq  -lusb-0.1 -lprotobuf-c -lbfd-aadhar -lbfd -lbclib -lscanner_status -lpinpad -ltfm -lQtSql ./lib/libbclib.a  ./lib/libsmart.a -O2 -lssl  #./lib/libauth_app.a
LIBS += -L /home/user/Desktop/GL11-GNOME-SDK-3.1/APP/Selfdaig-SDK-1.0/lib/ -lX11 -ljpeg -L./lib  -lauth_app -lgl11pos -lcurl -lcrypto -lbiometric -lopenjpeg -lwsq  -lusb-0.1 -lprotobuf-c -lbfd-aadhar -lbfd -lbclib -lscanner_status -ltfm -lQtSql -lpinpad ./lib/libbclib.a  ./lib/libsmart.a -O2 -lssl
#LIBS += -L /u02/tools/rootfs/usr/lib -lX11 -ljpeg -L./lib  -lauth_app -lgl11pos -lcurl -lcrypto -lbiometric -lopenjpeg -lwsq  -lusb-0.1 -lprotobuf-c -lbfd-aadhar -lbfd -lbclib -lscanner_status -lpinpad -ltfm -lQtSql ./lib/libbclib.a  ./lib/libsmart.a -O2 -lssl  #./lib/libauth_app.a


SOURCES += main.cpp\
        FPB.cpp \
    src/Login.cpp \
    src/HostComm.cpp \
    src/Display_Message.cpp \
    src/finomisc.cpp \
    src/glPPP_API.c \
    src/json.cpp \
    src/JsonParsing.cpp \
    src/getprofile.cpp \
    src/database.cpp \
    src/Global.cpp \
    src/bankmaster.cpp \
    src/popmsgdlg.cpp \
    src/aepsbalance.cpp \
    src/uid_auth.c \
    src/aadhaar.c \
    src/fino_print.cpp \
    src/print.cpp \
    src/entrydlg.cpp \
    src/aepswthdraw.cpp \
    src/aepsmini.cpp \
    src/lts.cpp \
    src/aepslts.cpp \
    src/json_VT.cpp \
    src/userprofile.cpp \
    src/fetchextrabenelist.cpp \
    src/WalkingCustomer.cpp \
    src/WalkinLimit.cpp \
    src/otpimpsnft.cpp \
    src/FindCust.cpp \
    src/makerequest.cpp \
    src/jsoncpp.cpp \
    src/curl.cpp \
    src/MerDashboard.cpp \
    src/selectscanner.cpp \
    src/FetchDeviceDetails.cpp \
    src/eAgreement.cpp \
    src/DateCalculation.cpp \
    src/imps.cpp \
    src/IMPSBeneVerif.cpp \
    src/IMPS_RegisteredBENE.cpp \
    src/Bill_Payment.cpp \
    src/Reversal.cpp \
    src/Transaction_StatusWdRRN.cpp \
    src/Ministatement.cpp

HEADERS  += FPB.h \
    include/Login.h \
    include/glHeaderGSM.h \
    include/gl11.h \
    include/finomisc.h \
    include/HostComm.h \
    include/displaymessage.h \
    include/CHeader.h \
    include/json.h \
    include/getprofile.h \
    include/database.h \
    include/bankmaster.h \
    include/auth_1.6.pb-c.h \
    include/http_client.h \
    include/popmsgdlg.h \
    include/aepsbalance.h \
    include/uid_auth.h \
    include/openssl/x509v3.h \
    include/openssl/x509.h \
    include/openssl/x509_vfy.h \
    include/openssl/ui.h \
    include/openssl/ui_compat.h \
    include/openssl/txt_db.h \
    include/openssl/tmdiff.h \
    include/openssl/tls1.h \
    include/openssl/symhacks.h \
    include/openssl/store.h \
    include/openssl/stack.h \
    include/openssl/ssl23.h \
    include/openssl/ssl3.h \
    include/openssl/ssl2.h \
    include/openssl/ssl.h \
    include/openssl/sha.h \
    include/openssl/seed.h \
    include/openssl/safestack.h \
    include/openssl/rsa.h \
    include/openssl/ripemd.h \
    include/openssl/rc5.h \
    include/openssl/rc4.h \
    include/openssl/rc2.h \
    include/openssl/rand.h \
    include/openssl/pqueue.h \
    include/openssl/pq_compat.h \
    include/openssl/pkcs12.h \
    include/openssl/pkcs7.h \
    include/openssl/pem2.h \
    include/openssl/pem.h \
    include/openssl/ossl_typ.h \
    include/openssl/opensslv.h \
    include/openssl/opensslconf.h \
    include/openssl/ocsp.h \
    include/openssl/objects.h \
    include/openssl/obj_mac.h \
    include/openssl/mdc2.h \
    include/openssl/md5.h \
    include/openssl/md4.h \
    include/openssl/md2.h \
    include/openssl/lhash.h \
    include/openssl/kssl.h \
    include/openssl/krb5_asn.h \
    include/openssl/idea.h \
    include/openssl/hmac.h \
    include/openssl/evp.h \
    include/openssl/err.h \
    include/openssl/engine.h \
    include/openssl/ecdsa.h \
    include/openssl/ecdh.h \
    include/openssl/ec.h \
    include/openssl/ebcdic.h \
    include/openssl/e_os2.h \
    include/openssl/dtls1.h \
    include/openssl/dso.h \
    include/openssl/dsa.h \
    include/openssl/dh.h \
    include/openssl/des.h \
    include/openssl/des_old.h \
    include/openssl/crypto.h \
    include/openssl/conf.h \
    include/openssl/conf_api.h \
    include/openssl/comp.h \
    include/openssl/cms.h \
    include/openssl/cast.h \
    include/openssl/camellia.h \
    include/openssl/buffer.h \
    include/openssl/bn.h \
    include/openssl/blowfish.h \
    include/openssl/bio.h \
    include/openssl/asn1t.h \
    include/openssl/asn1.h \
    include/openssl/asn1_mac.h \
    include/openssl/aes.h \
    include/aadhaar.h \
    include/print.h \
    include/fino_print.h \
    include/ui_entrydlg.h \
    include/entrydlg.h \
    include/aepswthdraw.h \
    include/aepsmini.h \
    include/lts.h \
    include/aepslts.h \
    include/jsonforwards_VT.h \
    include/json_VT.h \
    include/userprofile.h \
    include/impscommon.h \
    include/findcust.h \
    include/selectscanner.h \
    include/impsbill.h \
    include/Bill_Payment.h \
    include/Reversal.h

FORMS    += FPB.ui

RESOURCES += \
    fino.qrc
