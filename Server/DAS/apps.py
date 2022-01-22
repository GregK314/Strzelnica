from django.apps import AppConfig


class DASConfig(AppConfig):
    default_auto_field = 'django.db.models.BigAutoField'
    name = 'DAS'
    def ready(self):
        from .models import test_control_db
        test_control_db.objects.update_or_create(id=0,ctrl_name="Test active", defaults={'ctrl_stat': -1},)