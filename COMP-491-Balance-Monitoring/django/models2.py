# This is an auto-generated Django model module.
# You'll have to do the following manually to clean this up:
#   * Rearrange models' order
#   * Make sure each model has one field with primary_key=True
#   * Make sure each ForeignKey and OneToOneField has `on_delete` set to the desired behavior
#   * Remove `managed = False` lines if you wish to allow Django to create, modify, and delete the table
# Feel free to rename the models, but don't rename db_table values or field names.
from django.db import models


class Admint(models.Model):
    adminid = models.AutoField(db_column='AdminID', primary_key=True)  # Field name made lowercase.
    aname = models.CharField(db_column='AName', max_length=20, blank=True, null=True)  # Field name made lowercase.
    e_mail = models.CharField(db_column='E-mail', max_length=40, blank=True, null=True)  # Field name made lowercase. Field renamed to remove unsuitable characters.
    phonenumber = models.CharField(db_column='PhoneNumber', max_length=15, blank=True, null=True)  # Field name made lowercase.
    apassword = models.CharField(db_column='APassword', max_length=15, blank=True, null=True)  # Field name made lowercase.

    class Meta:
        managed = False
        db_table = 'ADMINT'


class Surveyresult(models.Model):
    surveydate = models.DateTimeField(db_column='SurveyDate', primary_key=True)  # Field name made lowercase.
    functionalscore = models.FloatField(db_column='FunctionalScore', blank=True, null=True)  # Field name made lowercase.
    physicalscore = models.FloatField(db_column='PhysicalScore', blank=True, null=True)  # Field name made lowercase.
    emotionalscore = models.FloatField(db_column='EmotionalScore', blank=True, null=True)  # Field name made lowercase.
    overallscore = models.FloatField(db_column='OverallScore', blank=True, null=True)  # Field name made lowercase.
    patientid = models.ForeignKey('HospitalPatient', models.DO_NOTHING, db_column='PatientID')  # Field name made lowercase.

    class Meta:
        managed = False
        db_table = 'SurveyResult'
        unique_together = (('surveydate', 'patientid'),)


class Testresult(models.Model):
    testdate = models.DateTimeField(db_column='TestDate', primary_key=True)  # Field name made lowercase.
    pathlength = models.FloatField(db_column='PathLength', blank=True, null=True)  # Field name made lowercase.
    pathlengthcor = models.FloatField(db_column='PathLengthCor', blank=True, null=True)  # Field name made lowercase.
    pathlengthsag = models.FloatField(db_column='PathLengthSag', blank=True, null=True)  # Field name made lowercase.
    normalizedpathlength = models.FloatField(db_column='NormalizedPathLength', blank=True, null=True)  # Field name made lowercase.
    jerk = models.FloatField(db_column='Jerk', blank=True, null=True)  # Field name made lowercase.
    jerkcor = models.FloatField(db_column='JerkCor', blank=True, null=True)  # Field name made lowercase.
    jerksag = models.FloatField(db_column='JerkSag', blank=True, null=True)  # Field name made lowercase.
    meanvel = models.FloatField(db_column='MeanVel', blank=True, null=True)  # Field name made lowercase.
    meanvelcor = models.FloatField(db_column='MeanVelCor', blank=True, null=True)  # Field name made lowercase.
    meanvelsag = models.FloatField(db_column='MeanVelSag', blank=True, null=True)  # Field name made lowercase.
    accx = models.JSONField(db_column='AccX', blank=True, null=True)  # Field name made lowercase.
    accz = models.JSONField(db_column='AccZ', blank=True, null=True)  # Field name made lowercase.
    patientid = models.ForeignKey('HospitalPatient', models.DO_NOTHING, db_column='PatientID', blank=True, null=True)  # Field name made lowercase.
    swayarea = models.FloatField(db_column='SwayArea', blank=True, null=True)  # Field name made lowercase.

    class Meta:
        managed = False
        db_table = 'TESTRESULT'


class Appointment(models.Model):
    patientid = models.PositiveIntegerField(db_column='patientId', blank=True, null=True)  # Field name made lowercase.
    doctorid = models.PositiveIntegerField(db_column='doctorId', blank=True, null=True)  # Field name made lowercase.
    patientname = models.CharField(db_column='patientName', max_length=40, blank=True, null=True)  # Field name made lowercase.
    doctorname = models.CharField(db_column='doctorName', max_length=40, blank=True, null=True)  # Field name made lowercase.
    appointmentdate = models.DateTimeField(db_column='appointmentDate', blank=True, null=True)  # Field name made lowercase.
    description = models.TextField(blank=True, null=True)
    status = models.IntegerField(blank=True, null=True)

    class Meta:
        managed = False
        db_table = 'appointment'


class AuthGroup(models.Model):
    name = models.CharField(unique=True, max_length=150)

    class Meta:
        managed = False
        db_table = 'auth_group'


class AuthGroupPermissions(models.Model):
    group = models.ForeignKey(AuthGroup, models.DO_NOTHING)
    permission = models.ForeignKey('AuthPermission', models.DO_NOTHING)

    class Meta:
        managed = False
        db_table = 'auth_group_permissions'
        unique_together = (('group', 'permission'),)


class AuthPermission(models.Model):
    name = models.CharField(max_length=255)
    content_type = models.ForeignKey('DjangoContentType', models.DO_NOTHING)
    codename = models.CharField(max_length=100)

    class Meta:
        managed = False
        db_table = 'auth_permission'
        unique_together = (('content_type', 'codename'),)


class AuthUser(models.Model):
    password = models.CharField(max_length=128)
    last_login = models.DateTimeField(blank=True, null=True)
    is_superuser = models.IntegerField()
    username = models.CharField(unique=True, max_length=150)
    first_name = models.CharField(max_length=150)
    last_name = models.CharField(max_length=150)
    email = models.CharField(max_length=254)
    is_staff = models.IntegerField()
    is_active = models.IntegerField()
    date_joined = models.DateTimeField()

    class Meta:
        managed = False
        db_table = 'auth_user'


class AuthUserGroups(models.Model):
    user = models.ForeignKey(AuthUser, models.DO_NOTHING)
    group = models.ForeignKey(AuthGroup, models.DO_NOTHING)

    class Meta:
        managed = False
        db_table = 'auth_user_groups'
        unique_together = (('user', 'group'),)


class AuthUserUserPermissions(models.Model):
    user = models.ForeignKey(AuthUser, models.DO_NOTHING)
    permission = models.ForeignKey(AuthPermission, models.DO_NOTHING)

    class Meta:
        managed = False
        db_table = 'auth_user_user_permissions'
        unique_together = (('user', 'permission'),)


class DjangoAdminLog(models.Model):
    action_time = models.DateTimeField()
    object_id = models.TextField(blank=True, null=True)
    object_repr = models.CharField(max_length=200)
    action_flag = models.PositiveSmallIntegerField()
    change_message = models.TextField()
    content_type = models.ForeignKey('DjangoContentType', models.DO_NOTHING, blank=True, null=True)
    user = models.ForeignKey(AuthUser, models.DO_NOTHING)

    class Meta:
        managed = False
        db_table = 'django_admin_log'


class DjangoContentType(models.Model):
    app_label = models.CharField(max_length=100)
    model = models.CharField(max_length=100)

    class Meta:
        managed = False
        db_table = 'django_content_type'
        unique_together = (('app_label', 'model'),)


class DjangoMigrations(models.Model):
    app = models.CharField(max_length=255)
    name = models.CharField(max_length=255)
    applied = models.DateTimeField()

    class Meta:
        managed = False
        db_table = 'django_migrations'


class DjangoSession(models.Model):
    session_key = models.CharField(primary_key=True, max_length=40)
    session_data = models.TextField()
    expire_date = models.DateTimeField()

    class Meta:
        managed = False
        db_table = 'django_session'


class HospitalAppointment(models.Model):
    patientid = models.PositiveIntegerField(db_column='patientId', blank=True, null=True)  # Field name made lowercase.
    doctorid = models.PositiveIntegerField(db_column='doctorId', blank=True, null=True)  # Field name made lowercase.
    patientname = models.CharField(db_column='patientName', max_length=40, blank=True, null=True)  # Field name made lowercase.
    doctorname = models.CharField(db_column='doctorName', max_length=40, blank=True, null=True)  # Field name made lowercase.
    appointmentdate = models.DateTimeField(db_column='appointmentDate', blank=True, null=True)  # Field name made lowercase.
    description = models.TextField(blank=True, null=True)
    status = models.IntegerField(blank=True, null=True)

    class Meta:
        managed = False
        db_table = 'hospital_appointment'


class HospitalDoctor(models.Model):
    address = models.CharField(max_length=40)
    department = models.CharField(max_length=50)
    status = models.IntegerField()
    user = models.OneToOneField(AuthUser, models.DO_NOTHING)
    profile_pic = models.CharField(max_length=100, blank=True, null=True)
    mobile = models.CharField(max_length=20, blank=True, null=True)

    class Meta:
        managed = False
        db_table = 'hospital_doctor'


class HospitalPatient(models.Model):
    patientid = models.AutoField(db_column='PatientID', primary_key=True)  # Field name made lowercase.
    pname = models.CharField(db_column='PName', max_length=20, blank=True, null=True)  # Field name made lowercase.
    email = models.CharField(db_column='Email', unique=True, max_length=40, blank=True, null=True)  # Field name made lowercase.
    phonenumber = models.CharField(db_column='PhoneNumber', max_length=15, blank=True, null=True)  # Field name made lowercase.
    gender = models.CharField(db_column='Gender', max_length=1, blank=True, null=True)  # Field name made lowercase.
    age = models.IntegerField(db_column='Age', blank=True, null=True)  # Field name made lowercase.
    weight = models.IntegerField(db_column='Weight', blank=True, null=True)  # Field name made lowercase.
    height = models.IntegerField(db_column='Height', blank=True, null=True)  # Field name made lowercase.
    ppassword = models.CharField(db_column='PPassword', max_length=40, blank=True, null=True)  # Field name made lowercase.
    assigneddoctorid = models.IntegerField(db_column='assignedDoctorId', blank=True, null=True)  # Field name made lowercase.
    admitdate = models.DateTimeField(db_column='admitDate', blank=True, null=True)  # Field name made lowercase.
    status = models.IntegerField(blank=True, null=True)
    symptoms = models.CharField(max_length=100)
    profile_pic = models.CharField(max_length=255, blank=True, null=True)
    address = models.CharField(max_length=40)
    user_id = models.IntegerField(blank=True, null=True)
    id = models.IntegerField(blank=True, null=True)
    mobile = models.CharField(max_length=20, blank=True, null=True)

    class Meta:
        managed = False
        db_table = 'hospital_patient'


class HospitalPatientdischargedetails(models.Model):
    patientid = models.IntegerField(db_column='patientId', blank=True, null=True)  # Field name made lowercase.
    patientname = models.CharField(db_column='patientName', max_length=40, blank=True, null=True)  # Field name made lowercase.
    assigneddoctorname = models.CharField(db_column='assignedDoctorName', max_length=40, blank=True, null=True)  # Field name made lowercase.
    address = models.CharField(max_length=40, blank=True, null=True)
    mobile = models.CharField(max_length=20, blank=True, null=True)
    symptoms = models.CharField(max_length=100, blank=True, null=True)
    admitdate = models.DateField(db_column='admitDate')  # Field name made lowercase.
    releasedate = models.DateField(db_column='releaseDate')  # Field name made lowercase.
    dayspent = models.IntegerField(db_column='daySpent')  # Field name made lowercase.
    roomcharge = models.IntegerField(db_column='roomCharge')  # Field name made lowercase.
    medicinecost = models.IntegerField(db_column='medicineCost')  # Field name made lowercase.
    doctorfee = models.IntegerField(db_column='doctorFee')  # Field name made lowercase.
    othercharge = models.IntegerField(db_column='OtherCharge')  # Field name made lowercase.
    total = models.IntegerField()

    class Meta:
        managed = False
        db_table = 'hospital_patientdischargedetails'
