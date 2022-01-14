# Generated by Django 4.0 on 2021-12-22 11:35

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('DAS', '0005_remove_sensor_report_db_status'),
    ]

    operations = [
        migrations.AddField(
            model_name='sensor_report_db',
            name='message',
            field=models.CharField(blank=True, max_length=100),
        ),
        migrations.AddField(
            model_name='sensor_report_db',
            name='status',
            field=models.IntegerField(default=1),
            preserve_default=False,
        ),
    ]