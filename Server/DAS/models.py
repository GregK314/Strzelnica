import datetime

from django.db import models
from django.utils import timezone

class Reading_db(models.Model):
    sensor_id = models.IntegerField()
    sensor_reading = models.FloatField()
    sensor_tick = models.IntegerField()
    test_run = models.IntegerField()