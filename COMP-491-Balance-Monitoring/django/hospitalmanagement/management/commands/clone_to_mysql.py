from django.core.management.base import BaseCommand
from django.apps import apps


class Command(BaseCommand):
    help = 'Clones all data from SQLite to MySQL'

    def handle(self, *args, **options):
        for model in apps.get_models():
            for obj in model.objects.using('default').all():
                obj.save(using='mysql')
