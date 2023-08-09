# This is an auto-generated Django model module.
# You'll have to do the following manually to clean this up:
#   * Rearrange models' order
#   * Make sure each model has one field with primary_key=True
#   * Make sure each ForeignKey and OneToOneField has `on_delete` set to the desired behavior
#   * Remove `managed = False` lines if you wish to allow Django to create, modify, and delete the table
# Feel free to rename the models, but don't rename db_table values or field names.
from django.db import models


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
    content_type = models.ForeignKey('DjangoContentType', models.DO_NOTHING)
    codename = models.CharField(max_length=100)
    name = models.CharField(max_length=255)

    class Meta:
        managed = False
        db_table = 'auth_permission'
        unique_together = (('content_type', 'codename'),)


class AuthUser(models.Model):
    password = models.CharField(max_length=128)
    last_login = models.DateTimeField(blank=True, null=True)
    is_superuser = models.BooleanField()
    username = models.CharField(unique=True, max_length=150)
    last_name = models.CharField(max_length=150)
    email = models.CharField(max_length=254)
    is_staff = models.BooleanField()
    is_active = models.BooleanField()
    date_joined = models.DateTimeField()
    first_name = models.CharField(max_length=150)

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
    object_id = models.TextField(blank=True, null=True)
    object_repr = models.CharField(max_length=200)
    action_flag = models.PositiveSmallIntegerField()
    change_message = models.TextField()
    content_type = models.ForeignKey('DjangoContentType', models.DO_NOTHING, blank=True, null=True)
    user = models.ForeignKey(AuthUser, models.DO_NOTHING)
    action_time = models.DateTimeField()

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
    doctorid = models.PositiveIntegerField(db_column='doctorId', blank=True, null=True)  # Field name made lowercase.
    appointmentdate = models.DateField(db_column='appointmentDate')  # Field name made lowercase.
    description = models.TextField()
    status = models.BooleanField()
    doctorname = models.CharField(db_column='doctorName', max_length=40, blank=True, null=True)  # Field name made lowercase.
    patientname = models.CharField(db_column='patientName', max_length=40, blank=True, null=True)  # Field name made lowercase.
    patientid = models.PositiveIntegerField(db_column='patientId', blank=True, null=True)  # Field name made lowercase.

    class Meta:
        managed = False
        db_table = 'hospital_appointment'


class HospitalDoctor(models.Model):
    address = models.CharField(max_length=40)
    department = models.CharField(max_length=50)
    status = models.BooleanField()
    user = models.OneToOneField(AuthUser, models.DO_NOTHING)
    profile_pic = models.CharField(max_length=100, blank=True, null=True)
    mobile = models.CharField(max_length=20, blank=True, null=True)

    class Meta:
        managed = False
        db_table = 'hospital_doctor'


class HospitalPatient(models.Model):
    address = models.CharField(max_length=40)
    mobile = models.CharField(max_length=20)
    symptoms = models.CharField(max_length=100)
    status = models.BooleanField()
    user = models.OneToOneField(AuthUser, models.DO_NOTHING)
    admitdate = models.DateField(db_column='admitDate')  # Field name made lowercase.
    profile_pic = models.CharField(max_length=100, blank=True, null=True)
    assigneddoctorid = models.PositiveIntegerField(db_column='assignedDoctorId', blank=True, null=True)  # Field name made lowercase.

    class Meta:
        managed = False
        db_table = 'hospital_patient'


class HospitalPatientdischargedetails(models.Model):
    patientname = models.CharField(db_column='patientName', max_length=40)  # Field name made lowercase.
    assigneddoctorname = models.CharField(db_column='assignedDoctorName', max_length=40)  # Field name made lowercase.
    address = models.CharField(max_length=40)
    mobile = models.CharField(max_length=20, blank=True, null=True)
    symptoms = models.CharField(max_length=100, blank=True, null=True)
    admitdate = models.DateField(db_column='admitDate')  # Field name made lowercase.
    releasedate = models.DateField(db_column='releaseDate')  # Field name made lowercase.
    dayspent = models.PositiveIntegerField(db_column='daySpent')  # Field name made lowercase.
    roomcharge = models.PositiveIntegerField(db_column='roomCharge')  # Field name made lowercase.
    medicinecost = models.PositiveIntegerField(db_column='medicineCost')  # Field name made lowercase.
    doctorfee = models.PositiveIntegerField(db_column='doctorFee')  # Field name made lowercase.
    othercharge = models.PositiveIntegerField(db_column='OtherCharge')  # Field name made lowercase.
    total = models.PositiveIntegerField()
    patientid = models.PositiveIntegerField(db_column='patientId', blank=True, null=True)  # Field name made lowercase.

    class Meta:
        managed = False
        db_table = 'hospital_patientdischargedetails'
