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
