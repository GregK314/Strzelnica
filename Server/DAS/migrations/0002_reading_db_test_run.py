# Generated by Django 4.0 on 2021-12-13 12:22

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('DAS', '0001_initial'),
    ]

    operations = [
        migrations.AddField(
            model_name='reading_db',
            name='test_run',
            field=models.IntegerField(default=0),
            preserve_default=False,
        ),
    ]