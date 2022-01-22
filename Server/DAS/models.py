import datetime

from django.db import models
from django.utils import timezone


class Reading_db(models.Model):
    sensor_id = models.IntegerField()
    sensor_reading = models.FloatField()
    sensor_tick = models.IntegerField()
    test_run = models.IntegerField()


class Sensor_report_db(models.Model):
    sensor_id = models.IntegerField()
    status = models.IntegerField()
    time_s = models.IntegerField()
    anglex = models.FloatField()
    angley = models.FloatField()
    anglez = models.FloatField()
    message = models.CharField(blank=True, max_length=100)


class test_control_db(models.Model):
    ctrl_name = models.CharField(blank=True, max_length=100)
    ctrl_stat = models.IntegerField()

    def __str__(self):
        return '{}'.format(self.ctrl_stat)

